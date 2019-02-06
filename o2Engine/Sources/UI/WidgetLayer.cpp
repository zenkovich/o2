#include "stdafx.h"
#include "WidgetLayer.h"

#include "UI/Widget.h"
#include "UI/WidgetLayout.h"
#include "Scene/Scene.h"

namespace o2
{
	UIWidgetLayer::UIWidgetLayer() :
		mDepth(0.0f), name((String)Math::Random<UInt>(0, UINT_MAX)),
		interactableLayout(Vec2F(), Vec2F(1.0f, 1.0f), Vec2F(), Vec2F()), mDrawable(nullptr)
	{
	}

	UIWidgetLayer::UIWidgetLayer(const UIWidgetLayer& other) :
		mDepth(other.mDepth), name(other.name), layout(other.layout), mTransparency(other.mTransparency),
		mResTransparency(1.0f), interactableLayout(other.interactableLayout), mParent(nullptr), mOwnerWidget(nullptr),
		mDrawable(nullptr), depth(this), transparency(this)
	{
		if (other.mDrawable)
			mDrawable = other.mDrawable->CloneAs<IRectDrawable>();

		for (auto child : other.mChildren)
			AddChild(child->CloneAs<UIWidgetLayer>());
	}

	UIWidgetLayer::~UIWidgetLayer()
	{
		delete mDrawable;

		for (auto child : mChildren)
			delete child;
	}

	UIWidgetLayer& UIWidgetLayer::operator=(const UIWidgetLayer& other)
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
			AddChild(child->CloneAs<UIWidgetLayer>());

		SetTransparency(other.mTransparency);

		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();

		return *this;
	}

	UIWidget* UIWidgetLayer::GetOwnerWidget() const
	{
		if (mOwnerWidget)
			return mOwnerWidget;

		return mParent->GetOwnerWidget();
	}

	void UIWidgetLayer::SetDrawable(IRectDrawable* drawable)
	{
		mDrawable = drawable;

		if (mOwnerWidget)
		{
			mOwnerWidget->UpdateLayersDrawingSequence();
			mOwnerWidget->UpdateTransform();
		}
	}

	IRectDrawable* UIWidgetLayer::GetDrawable() const
	{
		return mDrawable;
	}

	void UIWidgetLayer::Draw()
	{
		if (mEnabled)
			mDrawable->Draw();
	}

	void UIWidgetLayer::Update(float dt)
	{}

	bool UIWidgetLayer::IsEnabled() const
	{
		return mEnabled;
	}

	bool UIWidgetLayer::IsEnabledInHierarchy() const
	{
		bool parentEnabled = mOwnerWidget ? mOwnerWidget->IsEnabledInHierarchy() : mParent->IsEnabledInHierarchy();
		return mEnabled && parentEnabled;
	}

	void UIWidgetLayer::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	UIWidgetLayer* UIWidgetLayer::AddChild(UIWidgetLayer* node)
	{
		if (node->GetParent())
			node->GetParent()->RemoveChild(node, false);

		node->mParent = this;
		node->mOwnerWidget = mOwnerWidget;

		mChildren.Add(node);

		OnChildAdded(node);

		return node;
	}

	bool UIWidgetLayer::RemoveChild(UIWidgetLayer* node, bool release /*= true*/)
	{
		node->mParent = nullptr;
		node->mOwnerWidget = nullptr;

		mChildren.Remove(node);

		if (release && node)
			delete node;

		return true;
	}

	void UIWidgetLayer::RemoveAllChilds()
	{
		for (auto child : mChildren)
		{
			if (child)
				delete child;
		}

		mChildren.Clear();
	}

	void UIWidgetLayer::SetParent(UIWidgetLayer* parent)
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

	UIWidgetLayer* UIWidgetLayer::GetParent() const
	{
		return mParent;
	}

	UIWidgetLayer::ChildrenVec& UIWidgetLayer::GetChilds()
	{
		return mChildren;
	}

	const UIWidgetLayer::ChildrenVec& o2::UIWidgetLayer::GetChilds() const
	{
		return mChildren;
	}

	UIWidgetLayer* UIWidgetLayer::AddChildLayer(const String& name, IRectDrawable* drawable,
												const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mOwnerWidget->mDrawingLayers.Count();

		UIWidgetLayer* layer = mnew UIWidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->mDrawable = drawable;
		layer->layout = layout;

		return AddChild(layer);
	}

	UIWidgetLayer* UIWidgetLayer::GetChild(const String& path)
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

	UIWidgetLayer* UIWidgetLayer::FindChild(const String& name)
	{
		for (auto child : mChildren)
		{
			if (child->name == name)
				return child;

			UIWidgetLayer* layer = child->FindChild(name);
			if (layer)
				return layer;
		}

		return nullptr;
	}

	LayersVec UIWidgetLayer::GetAllChilds() const
	{
		LayersVec res = mChildren;
		for (auto child : mChildren)
		{
			res.Add(child->GetAllChilds());
		}

		return res;
	}

	void UIWidgetLayer::SetDepth(float depth)
	{
		mDepth = depth;
		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();
	}

	float UIWidgetLayer::GetDepth() const
	{
		return mDepth;
	}

	void UIWidgetLayer::SetTransparency(float transparency)
	{
		mTransparency = transparency;
		UpdateResTransparency();
	}

	float UIWidgetLayer::GetTransparency()
	{
		return mTransparency;
	}

	float UIWidgetLayer::GetResTransparency() const
	{
		return mResTransparency;
	}

	bool UIWidgetLayer::IsUnderPoint(const Vec2F& point)
	{
		return mInteractableArea.IsInside(point);
	}

	const RectF& UIWidgetLayer::GetRect() const
	{
		return mAbsolutePosition;
	}

	void UIWidgetLayer::OnDeserialized(const DataNode& node)
	{
		for (auto child : mChildren)
		{
			child->mParent = this;
			child->mOwnerWidget = mOwnerWidget;
		}
	}

	void UIWidgetLayer::SetOwnerWidget(UIWidget* owner)
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

	void UIWidgetLayer::OnChildAdded(UIWidgetLayer* child)
	{
		child->SetOwnerWidget(mOwnerWidget);

		if (mOwnerWidget)
		{
			mOwnerWidget->OnLayerAdded(child);
			mOwnerWidget->UpdateLayersDrawingSequence();
		}
	}

	void UIWidgetLayer::UpdateLayout()
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

	void UIWidgetLayer::UpdateResTransparency()
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

	void UIWidgetLayer::OnIncludeInScene()
	{
#if IS_EDITOR
		o2Scene.mEditableObjects.Add(this);
#endif

		for (auto layer : mChildren)
			layer->OnIncludeInScene();
	}

	void UIWidgetLayer::OnExcludeFromScene()
	{
#if IS_EDITOR
		o2Scene.mEditableObjects.Remove(this);
#endif

		for (auto layer : mChildren)
			layer->OnIncludeInScene();
	}

	Dictionary<String, UIWidgetLayer*> UIWidgetLayer::GetAllChildLayers()
	{
		Dictionary<String, UIWidgetLayer*> res;
		for (auto layer : mChildren)
			res.Add(layer->name, layer);

		return res;
	}

#if IS_EDITOR

	bool UIWidgetLayer::IsOnScene() const
	{
		if (mParent)
			return mParent->IsOnScene();

		if (mOwnerWidget)
			return mOwnerWidget->IsOnScene();

		return true;
	}

	SceneUID UIWidgetLayer::GetID() const
	{
		return mUID;
	}

	void UIWidgetLayer::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();

		if (childs)
		{
			for (auto child : mChildren)
				child->GenerateNewID(true);
		}
	}

	String UIWidgetLayer::GetName() const
	{
		return name;
	}

	void UIWidgetLayer::SetName(const String& name)
	{
		this->name = name;
	}

	Vector<SceneEditableObject*> UIWidgetLayer::GetEditablesChildren() const
	{
		return mChildren.Select<SceneEditableObject*>([](UIWidgetLayer* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	SceneEditableObject* UIWidgetLayer::GetEditableParent() const
	{
		if (mParent)
			return dynamic_cast<SceneEditableObject*>(mParent);

		return &mOwnerWidget->layersEditable;
	}

	void UIWidgetLayer::SetEditableParent(SceneEditableObject* object)
	{
		if (auto layer = dynamic_cast<UIWidgetLayer*>(object))
			layer->AddChild(this);
		else if (auto widget = dynamic_cast<UIWidget*>(object))
			widget->AddLayer(this);
		else if (auto layers = dynamic_cast<UIWidget::LayersEditable*>(object))
			layers->AddEditableChild(this);
	}

	void UIWidgetLayer::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (auto layer = dynamic_cast<UIWidgetLayer*>(object))
			AddChild(layer);
		else if (auto actor = dynamic_cast<Actor*>(object))
			mOwnerWidget->AddEditableChild(object, idx);
	}

	void UIWidgetLayer::SetIndexInSiblings(int idx)
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

	bool UIWidgetLayer::CanBeParentedTo(const Type& parentType)
	{
		return parentType.IsBasedOn(TypeOf(UIWidget::LayersEditable));
	}

	bool UIWidgetLayer::IsSupportsDisabling() const
	{
		return true;
	}

	bool UIWidgetLayer::IsSupportsLocking() const
	{
		return false;
	}

	bool UIWidgetLayer::IsLocked() const
	{
		return mIsLocked;
	}

	bool UIWidgetLayer::IsLockedInHierarchy() const
	{
		bool lockedParent = mOwnerWidget ? mOwnerWidget->IsLockedInHierarchy() : mParent->IsLockedInHierarchy();
		return mIsLocked && lockedParent;
	}

	void UIWidgetLayer::SetLocked(bool locked)
	{
		mIsLocked = locked;
	}

	bool UIWidgetLayer::IsSupportsTransforming() const
	{
		return true;
	}

	Basis UIWidgetLayer::GetTransform() const
	{
		return Basis(mAbsolutePosition.LeftBottom(), Vec2F::Right()*mAbsolutePosition.Width(), Vec2F::Up()*mAbsolutePosition.Height());
	}

	void UIWidgetLayer::SetTransform(const Basis& transform)
	{
		Basis thisTransform = GetTransform();
		layout.offsetMin += transform.origin - thisTransform.origin;
		layout.offsetMax += transform.origin - thisTransform.origin +
			Vec2F(transform.xv.Length() - thisTransform.xv.Length(),
				  transform.yv.Length() - thisTransform.yv.Length());
	}

	void UIWidgetLayer::UpdateTransform()
	{
		if (mOwnerWidget)
		{
			mOwnerWidget->UpdateTransform();
			mOwnerWidget->OnChanged();
		}
	}

	bool UIWidgetLayer::IsSupportsPivot() const
	{
		return false;
	}

	void UIWidgetLayer::SetPivot(const Vec2F& pivot)
	{}

	Vec2F UIWidgetLayer::GetPivot() const
	{
		return Vec2F();
	}

	bool UIWidgetLayer::IsSupportsLayout() const
	{
		return true;
	}

	Layout UIWidgetLayer::GetLayout() const
	{
		return layout;
	}

	void UIWidgetLayer::SetLayout(const Layout& layout)
	{
		this->layout = layout;
	}

	void UIWidgetLayer::OnChanged()
	{
		if (mOwnerWidget)
			mOwnerWidget->OnChanged();
	}

#endif // IS_EDITOR

}

DECLARE_CLASS(o2::UIWidgetLayer);
