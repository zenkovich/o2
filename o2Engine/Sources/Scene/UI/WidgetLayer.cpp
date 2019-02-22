#include "stdafx.h"
#include "WidgetLayer.h"

#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/Scene.h"

namespace o2
{
	WidgetLayer::WidgetLayer() :
		layout(this), mDepth(0.0f), name((String)Math::Random<UInt>(0, UINT_MAX)),
		interactableLayout(Vec2F(), Vec2F(1.0f, 1.0f), Vec2F(), Vec2F()), mDrawable(nullptr)
	{}

	WidgetLayer::WidgetLayer(const WidgetLayer& other) :
		mDepth(other.mDepth), name(other.name), layout(this, other.layout), mTransparency(other.mTransparency),
		mResTransparency(1.0f), interactableLayout(other.interactableLayout), mParent(nullptr), mOwnerWidget(nullptr),
		mDrawable(nullptr), depth(this), transparency(this)
	{
		if (other.mDrawable)
			mDrawable = other.mDrawable->CloneAs<IRectDrawable>();

		for (auto child : other.mChildren)
			AddChild(child->CloneAs<WidgetLayer>());
	}

	WidgetLayer::~WidgetLayer()
	{
		if (mParent)
			mParent->RemoveChild(this, false);
		else if (mOwnerWidget)
			mOwnerWidget->RemoveLayer(this, false);

		delete mDrawable;

		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			child->mOwnerWidget = nullptr;

			delete child;
		}

#if IS_EDITOR
		Scene::UnregEditableObject(this);
		o2Scene.OnObjectDestroyed(this);
#endif
	}

	WidgetLayer& WidgetLayer::operator=(const WidgetLayer& other)
	{
		delete mDrawable;
		for (auto child : mChildren)
			delete child;

		mChildren.Clear();
		mDrawable = nullptr;

		mDepth = other.mDepth;
		name = other.name;

		if (other.mDrawable)
			mDrawable = other.mDrawable->CloneAs<IRectDrawable>();

		for (auto child : other.mChildren)
			AddChild(child->CloneAs<WidgetLayer>());

		SetTransparency(other.mTransparency);

		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();

		return *this;
	}

	Widget* WidgetLayer::GetOwnerWidget() const
	{
		if (mOwnerWidget)
			return mOwnerWidget;

		return mParent->GetOwnerWidget();
	}

	void WidgetLayer::SetDrawable(IRectDrawable* drawable)
	{
		mDrawable = drawable;

		if (mOwnerWidget)
		{
			mOwnerWidget->UpdateLayersDrawingSequence();
			mOwnerWidget->UpdateTransform();
		}
	}

	IRectDrawable* WidgetLayer::GetDrawable() const
	{
		return mDrawable;
	}

	void WidgetLayer::Draw()
	{
		if (mEnabled)
			mDrawable->Draw();
	}

	void WidgetLayer::Update(float dt)
	{}

	bool WidgetLayer::IsEnabled() const
	{
		return mEnabled;
	}

	bool WidgetLayer::IsEnabledInHierarchy() const
	{
		bool parentEnabled = mOwnerWidget ? mOwnerWidget->IsEnabledInHierarchy() : mParent->IsEnabledInHierarchy();
		return mEnabled && parentEnabled;
	}

	void WidgetLayer::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	WidgetLayer* WidgetLayer::AddChild(WidgetLayer* node)
	{
		if (node->GetParent())
			node->GetParent()->RemoveChild(node, false);

		node->mParent = this;
		node->mOwnerWidget = mOwnerWidget;

		mChildren.Add(node);

		OnChildAdded(node);

#if IS_EDITOR
		o2Scene.OnObjectChanged(this);
		o2Scene.OnObjectChanged(node);
#endif

		return node;
	}

	bool WidgetLayer::RemoveChild(WidgetLayer* node, bool release /*= true*/)
	{
		node->mParent = nullptr;

		auto lastOwnerWidget = node->mOwnerWidget;
		node->mOwnerWidget = nullptr;

		mChildren.Remove(node);

		if (release && node)
			delete node;

		if (lastOwnerWidget)
			lastOwnerWidget->UpdateLayersDrawingSequence();

#if IS_EDITOR
		o2Scene.OnObjectChanged(this);
#endif

		return true;
	}

	void WidgetLayer::RemoveAllChilds()
	{
		for (auto child : mChildren)
		{
			if (child)
				delete child;
		}

		mChildren.Clear();

#if IS_EDITOR
		o2Scene.OnObjectChanged(this);
#endif
	}

	void WidgetLayer::SetParent(WidgetLayer* parent)
	{
		if (parent)
		{
			parent->AddChild(this);
		}
		else
		{
			if (mParent)
				mParent->RemoveChild(this, false);

			mParent = nullptr;
			mOwnerWidget = nullptr;
		}
	}

	WidgetLayer* WidgetLayer::GetParent() const
	{
		return mParent;
	}

	WidgetLayer::ChildrenVec& WidgetLayer::GetChilds()
	{
		return mChildren;
	}

	const WidgetLayer::ChildrenVec& o2::WidgetLayer::GetChilds() const
	{
		return mChildren;
	}

	WidgetLayer* WidgetLayer::AddChildLayer(const String& name, IRectDrawable* drawable,
												const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mOwnerWidget->mDrawingLayers.Count();

		WidgetLayer* layer = mnew WidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->mDrawable = drawable;
		layer->layout = layout;

		return AddChild(layer);
	}

	WidgetLayer* WidgetLayer::GetChild(const String& path)
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParent;
				else
					return mParent->GetChild(path.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mChildren)
		{
			if (child->name == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	WidgetLayer* WidgetLayer::FindChild(const String& name)
	{
		for (auto child : mChildren)
		{
			if (child->name == name)
				return child;

			WidgetLayer* layer = child->FindChild(name);
			if (layer)
				return layer;
		}

		return nullptr;
	}

	LayersVec WidgetLayer::GetAllChilds() const
	{
		LayersVec res = mChildren;
		for (auto child : mChildren)
		{
			res.Add(child->GetAllChilds());
		}

		return res;
	}

	void WidgetLayer::SetDepth(float depth)
	{
		mDepth = depth;
		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();
	}

	float WidgetLayer::GetDepth() const
	{
		return mDepth;
	}

	void WidgetLayer::SetTransparency(float transparency)
	{
		mTransparency = transparency;
		UpdateResTransparency();
	}

	float WidgetLayer::GetTransparency()
	{
		return mTransparency;
	}

	float WidgetLayer::GetResTransparency() const
	{
		return mResTransparency;
	}

	bool WidgetLayer::IsUnderPoint(const Vec2F& point)
	{
		return mInteractableArea.IsInside(point);
	}

	const RectF& WidgetLayer::GetRect() const
	{
		return mAbsolutePosition;
	}

	void WidgetLayer::OnDeserialized(const DataNode& node)
	{
		for (auto child : mChildren)
		{
			child->mParent = this;
			child->mOwnerWidget = mOwnerWidget;
		}
	}

	void WidgetLayer::SetOwnerWidget(Widget* owner)
	{
		mOwnerWidget = owner;

		for (auto child : mChildren)
			child->SetOwnerWidget(owner);

		UpdateResTransparency();

#if IS_EDITOR
		if (Scene::IsSingletonInitialzed() && mOwnerWidget && mOwnerWidget->IsHieararchyOnScene())
			o2Scene.mEditableObjects.Add(this);
#endif
	}

	void WidgetLayer::OnChildAdded(WidgetLayer* child)
	{
		child->SetOwnerWidget(mOwnerWidget);

		if (mOwnerWidget)
		{
			mOwnerWidget->OnLayerAdded(child);
			mOwnerWidget->UpdateLayersDrawingSequence();
		}
	}

	void WidgetLayer::OnLayoutChanged()
	{
		if (mUpdatingLayout)
			return;

		mUpdatingLayout = true;

		if (mOwnerWidget)
		{
			mOwnerWidget->UpdateLayersLayouts();
			mOwnerWidget->OnChanged();
		}

		mUpdatingLayout = false;
	}

	void WidgetLayer::UpdateLayout()
	{
		if (mParent)
			mAbsolutePosition = layout.Calculate(mParent->mAbsolutePosition);
		else
			mAbsolutePosition = layout.Calculate(mOwnerWidget->layout->GetWorldRect());

		mInteractableArea = interactableLayout.Calculate(mAbsolutePosition);

		if (mDrawable)
			mDrawable->SetRect(mAbsolutePosition);

		for (auto child : mChildren)
			child->UpdateLayout();
	}

	void WidgetLayer::UpdateResTransparency()
	{
		if (mParent)
			mResTransparency = transparency*mParent->mResTransparency;
		else
			mResTransparency = transparency*mOwnerWidget->mResTransparency;

		if (mDrawable)
			mDrawable->SetTransparency(mResTransparency);

		for (auto child : mChildren)
			child->UpdateResTransparency();
	}

	void WidgetLayer::OnIncludeInScene()
	{
#if IS_EDITOR
		o2Scene.mEditableObjects.Add(this);
#endif

		for (auto layer : mChildren)
			layer->OnIncludeInScene();
	}

	void WidgetLayer::OnExcludeFromScene()
	{
#if IS_EDITOR
		Scene::UnregEditableObject(this);
#endif

		for (auto layer : mChildren)
			layer->OnIncludeInScene();
	}

	Dictionary<String, WidgetLayer*> WidgetLayer::GetAllChildLayers()
	{
		Dictionary<String, WidgetLayer*> res;
		for (auto layer : mChildren)
			res.Add(layer->name, layer);

		return res;
	}

#if IS_EDITOR

	bool WidgetLayer::IsOnScene() const
	{
		if (mParent)
			return mParent->IsOnScene();

		if (mOwnerWidget)
			return mOwnerWidget->IsOnScene();

		return true;
	}

	SceneUID WidgetLayer::GetID() const
	{
		return mUID;
	}

	void WidgetLayer::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();

		if (childs)
		{
			for (auto child : mChildren)
				child->GenerateNewID(true);
		}
	}

	String WidgetLayer::GetName() const
	{
		return name;
	}

	void WidgetLayer::SetName(const String& name)
	{
		this->name = name;
	}

	Vector<SceneEditableObject*> WidgetLayer::GetEditablesChildren() const
	{
		return mChildren.Select<SceneEditableObject*>([](WidgetLayer* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	SceneEditableObject* WidgetLayer::GetEditableParent() const
	{
		if (mParent)
			return dynamic_cast<SceneEditableObject*>(mParent);

		return &mOwnerWidget->layersEditable;
	}

	void WidgetLayer::SetEditableParent(SceneEditableObject* object)
	{
		if (auto layer = dynamic_cast<WidgetLayer*>(object))
			layer->AddChild(this);
		else if (auto widget = dynamic_cast<Widget*>(object))
			widget->AddLayer(this);
		else if (auto layers = dynamic_cast<Widget::LayersEditable*>(object))
			layers->AddEditableChild(this);
	}

	void WidgetLayer::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (auto layer = dynamic_cast<WidgetLayer*>(object))
			AddChild(layer);
		else if (auto actor = dynamic_cast<Actor*>(object))
			mOwnerWidget->AddEditableChild(object, idx);
	}

	void WidgetLayer::SetIndexInSiblings(int idx)
	{
		if (mParent)
		{
			int lastIdx = mParent->mChildren.Find(this);
			mParent->mChildren.Insert(this, idx);

			if (idx <= lastIdx)
				lastIdx++;

			mParent->mChildren.RemoveAt(lastIdx);
		}
		else
		{
			int lastIdx = mOwnerWidget->mLayers.Find(this);
			mOwnerWidget->mLayers.Insert(this, idx);

			if (idx <= lastIdx)
				lastIdx++;

			mOwnerWidget->mLayers.RemoveAt(lastIdx);
		}
	}

	bool WidgetLayer::CanBeParentedTo(const Type& parentType)
	{
		return parentType.IsBasedOn(TypeOf(Widget::LayersEditable));
	}

	bool WidgetLayer::IsSupportsDisabling() const
	{
		return true;
	}

	bool WidgetLayer::IsSupportsLocking() const
	{
		return false;
	}

	bool WidgetLayer::IsLocked() const
	{
		return mIsLocked;
	}

	bool WidgetLayer::IsLockedInHierarchy() const
	{
		bool lockedParent = mOwnerWidget ? mOwnerWidget->IsLockedInHierarchy() : mParent->IsLockedInHierarchy();
		return mIsLocked && lockedParent;
	}

	void WidgetLayer::SetLocked(bool locked)
	{
		mIsLocked = locked;
	}

	bool WidgetLayer::IsSupportsTransforming() const
	{
		return true;
	}

	Basis WidgetLayer::GetTransform() const
	{
		return Basis(mAbsolutePosition.LeftBottom(), Vec2F::Right()*mAbsolutePosition.Width(), Vec2F::Up()*mAbsolutePosition.Height());
	}

	void WidgetLayer::SetTransform(const Basis& transform)
	{
		Basis thisTransform = GetTransform();
		layout.offsetMin += transform.origin - thisTransform.origin;
		layout.offsetMax += transform.origin - thisTransform.origin +
			Vec2F(transform.xv.Length() - thisTransform.xv.Length(),
				  transform.yv.Length() - thisTransform.yv.Length());
	}

	void WidgetLayer::UpdateTransform()
	{
		if (mOwnerWidget)
		{
			mOwnerWidget->UpdateTransform();
			mOwnerWidget->OnChanged();
		}
	}

	bool WidgetLayer::IsSupportsPivot() const
	{
		return false;
	}

	void WidgetLayer::SetPivot(const Vec2F& pivot)
	{}

	Vec2F WidgetLayer::GetPivot() const
	{
		return Vec2F();
	}

	bool WidgetLayer::IsSupportsLayout() const
	{
		return true;
	}

	Layout WidgetLayer::GetLayout() const
	{
		return layout;
	}

	void WidgetLayer::SetLayout(const Layout& layout)
	{
		this->layout = layout;
	}

	void WidgetLayer::OnChanged()
	{
		if (mOwnerWidget)
			mOwnerWidget->OnChanged();
	}

#endif // IS_EDITOR

}

DECLARE_CLASS(o2::WidgetLayer);
