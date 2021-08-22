#include "o2/stdafx.h"
#include "WidgetLayer.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	WidgetLayer::WidgetLayer():
		layout(this), interactableLayout(Vec2F(), Vec2F(1.0f, 1.0f), Vec2F(), Vec2F()), mUID(Math::Random())
	{}

	WidgetLayer::WidgetLayer(const WidgetLayer& other):
		mDepth(other.mDepth), name(other.name), layout(this, other.layout), mTransparency(other.mTransparency),
		interactableLayout(other.interactableLayout), mUID(Math::Random()), depth(this), transparency(this)
	{
		if (other.mCopyVisitor)
			other.mCopyVisitor->OnCopy(&other, this);

		if (other.mDrawable)
		{
			mDrawable = other.mDrawable->CloneAs<IRectDrawable>();
			mDrawable->SetSerializeEnabled(false);
		}

		for (auto child : other.mChildren)
		{
			child->mCopyVisitor = other.mCopyVisitor;

			AddChild(child->CloneAs<WidgetLayer>());

			child->mCopyVisitor = nullptr;
		}
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
			child->SetOwnerWidget(nullptr);
			delete child;
		}

		mChildren.Clear();
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
		{
			mDrawable = other.mDrawable->CloneAs<IRectDrawable>();
			mDrawable->SetSerializeEnabled(false);
		}

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

	const WidgetLayer* WidgetLayer::GetPrototypeLink() const
	{
		return mPrototypeLink;
	}

	void WidgetLayer::SetDrawable(IRectDrawable* drawable)
	{
		mDrawable = drawable;
		
		if (mDrawable)
			mDrawable->SetSerializeEnabled(false);

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
		if (mEnabled && mResTransparency > FLT_EPSILON)
			mDrawable->Draw();
	}

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

	WidgetLayer* WidgetLayer::AddChild(WidgetLayer* layer)
	{
		if (layer->mParent)
			layer->mParent->RemoveChild(layer, false);
		else if (layer->mOwnerWidget)
			layer->mOwnerWidget->RemoveLayer(layer, false);

		layer->mParent = this;
		mChildren.Add(layer);

		layer->SetOwnerWidget(mOwnerWidget);

		if (mOwnerWidget)
		{
			mOwnerWidget->OnLayerAdded(layer);
			mOwnerWidget->UpdateLayersDrawingSequence();
		}

		if constexpr (IS_EDITOR)
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.OnObjectChanged(layer);
		}

		return layer;
	}

	void WidgetLayer::RemoveChild(WidgetLayer* layer, bool release /*= true*/)
	{
		if (!layer)
			return;

		layer->mParent = nullptr;
		mChildren.Remove(layer);

		auto lastOwnerWidget = layer->mOwnerWidget;
		layer->SetOwnerWidget(nullptr);

		if (release)
			delete layer;

		if (lastOwnerWidget)
			lastOwnerWidget->UpdateLayersDrawingSequence();

		if constexpr (IS_EDITOR)
			o2Scene.OnObjectChanged(this);
	}

	void WidgetLayer::RemoveAllChildren()
	{
		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			child->SetOwnerWidget(nullptr);
			delete child;
		}

		mChildren.Clear();

		if constexpr (IS_EDITOR)
			o2Scene.OnObjectChanged(this);
	}

	void WidgetLayer::SetParent(WidgetLayer* parent)
	{
		if (parent)
			parent->AddChild(this);
		else
		{
			if (mParent)
				mParent->RemoveChild(this, false);

			mParent = nullptr;
			SetOwnerWidget(nullptr);
		}
	}

	WidgetLayer* WidgetLayer::GetParent() const
	{
		return mParent;
	}

	Vector<WidgetLayer*>& WidgetLayer::GetChildren()
	{
		return mChildren;
	}

	const Vector<WidgetLayer*>& o2::WidgetLayer::GetChildren() const
	{
		return mChildren;
	}

	void WidgetLayer::SerializeBasicOverride(DataValue& node) const
	{
		if (mPrototypeLink)
			SerializeWithProto(node);
		else
			SerializeRaw(node);

		OnSerialize(node);
	}

	void WidgetLayer::DeserializeBasicOverride(const DataValue& node)
	{
		if (node.FindMember("PrototypeLink"))
			DeserializeWithProto(node);
		else
			DeserializeRaw(node);

		OnDeserialized(node);
	}

	void WidgetLayer::SerializeRaw(DataValue& node) const
	{
		SerializeBasic(node);

		if (!mChildren.IsEmpty())
			node["Children"] = mChildren;
	}

	void WidgetLayer::DeserializeRaw(const DataValue& node)
	{
		DeserializeBasic(node);

		if (auto childrenNode = node.FindMember("Children"))
		{
			for (auto& childNode : *childrenNode)
			{
				auto layer = mnew WidgetLayer();
				AddChild(layer);
				layer->Deserialize(childNode["Value"]);
			}
		}
	}

	void WidgetLayer::SerializeWithProto(DataValue& node) const
	{
		SerializeDelta(node, *mPrototypeLink);

		node["PrototypeLink"] = mPrototypeLink->mUID;

		if (!mChildren.IsEmpty())
		{
			auto& childrenNode = node.AddMember("Children");
			childrenNode.SetArray();

			for (auto child : mChildren)
				child->Serialize(childrenNode.AddElement());
		}
	}

	void WidgetLayer::DeserializeWithProto(const DataValue& node)
	{
		if (auto protoNode = node.FindMember("PrototypeLink"))
		{
			SceneUID protoUID = *protoNode;
			if (mParent && mParent->mPrototypeLink)
			{
				for (auto protoChild : mParent->mPrototypeLink->mChildren)
				{
					if (protoChild->mUID == protoUID)
					{
						mPrototypeLink = protoChild;
						break;
					}
				}
			}
			else if (mOwnerWidget->mPrototypeLink)
			{
				for (auto protoChild : dynamic_cast<Widget*>(mOwnerWidget->mPrototypeLink.Get())->mLayers)
				{
					if (protoChild->mUID == protoUID)
					{
						mPrototypeLink = protoChild;
						break;
					}
				}
			}
		}

		if (mPrototypeLink)
			DeserializeDelta(node, *mPrototypeLink);
		else
			DeserializeBasic(node);

		if (auto childrenNode = node.FindMember("Children"))
		{
			for (auto& childNode : *childrenNode)
			{
				auto layer = mnew WidgetLayer();
				AddChild(layer);
				layer->Deserialize(childNode);
			}
		}
	}

	void WidgetLayer::OnDeserialized(const DataValue& node)
	{
		for (auto child : mChildren)
		{
			child->mParent = this;
			child->mOwnerWidget = mOwnerWidget;
		}

		if (mDrawable)
			mDrawable->SetSerializeEnabled(false);
	}

	void WidgetLayer::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		OnDeserialized(node);
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

	Vector<WidgetLayer*> WidgetLayer::GetAllChilds() const
	{
		Vector<WidgetLayer*> res = mChildren;
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

	void WidgetLayer::SetOwnerWidget(Widget* owner)
	{
		mOwnerWidget = owner;

		if constexpr (IS_EDITOR)
		{
			if (Scene::IsSingletonInitialzed())
			{
				if (mOwnerWidget && mOwnerWidget->IsOnScene())
					o2Scene.AddEditableObjectToScene(this);
				else
					o2Scene.RemoveEditableObjectFromScene(this);
			}
		}

		for (auto child : mChildren)
			child->SetOwnerWidget(owner);

		UpdateResTransparency();
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
		else if (mOwnerWidget)
			mResTransparency = transparency*mOwnerWidget->mResTransparency;
		else
			mResTransparency = mTransparency;

		if (mDrawable)
			mDrawable->SetTransparency(mResTransparency);

		for (auto child : mChildren)
			child->UpdateResTransparency();
	}

	void WidgetLayer::OnAddToScene()
	{
		if constexpr (IS_EDITOR)
			o2Scene.AddEditableObjectToScene(this);

		for (auto layer : mChildren)
			layer->OnAddToScene();
	}

	void WidgetLayer::OnRemoveFromScene()
	{
		if constexpr (IS_EDITOR)
			o2Scene.RemoveEditableObjectFromScene(this);

		for (auto layer : mChildren)
			layer->OnRemoveFromScene();
	}

	Map<String, WidgetLayer*> WidgetLayer::GetAllChildLayers()
	{
		Map<String, WidgetLayer*> res;
		for (auto layer : mChildren)
			res.Add(layer->name, layer);

		return res;
	}

	void WidgetLayer::InstantiatePrototypeCloneVisitor::OnCopy(const WidgetLayer* source, WidgetLayer* target)
	{
		target->mPrototypeLink = source;
	}

	void WidgetLayer::MakePrototypeCloneVisitor::OnCopy(const WidgetLayer* source, WidgetLayer* target)
	{
		target->mPrototypeLink = source->mPrototypeLink;
		const_cast<WidgetLayer*>(source)->mPrototypeLink = target;
	}

#if IS_EDITOR

	bool WidgetLayer::IsOnScene() const
	{
		if (mOwnerWidget)
			return mOwnerWidget->IsOnScene();

		return false;
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

	const String& WidgetLayer::GetName() const
	{
		return name;
	}

	void WidgetLayer::SetName(const String& name)
	{
		this->name = name;
	}

	Vector<SceneEditableObject*> WidgetLayer::GetEditableChildren() const
	{
		return mChildren.DynamicCast<SceneEditableObject*>();
	}
	
	const SceneEditableObject* o2::WidgetLayer::GetEditableLink() const
	{
		return mPrototypeLink;
	}

	void WidgetLayer::BeginMakePrototype() const
	{
		mCopyVisitor = mnew MakePrototypeCloneVisitor();
	}

	void WidgetLayer::BeginInstantiatePrototype() const
	{
		mCopyVisitor = mnew InstantiatePrototypeCloneVisitor();
	}

	SceneEditableObject* WidgetLayer::GetEditableParent() const
	{
		if (mParent)
			return mParent;

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
			int lastIdx = mParent->mChildren.IndexOf(this);
			mParent->mChildren.Insert(this, idx);

			if (idx <= lastIdx)
				lastIdx++;

			mParent->mChildren.RemoveAt(lastIdx);
		}
		else
		{
			int lastIdx = mOwnerWidget->mLayers.IndexOf(this);
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
