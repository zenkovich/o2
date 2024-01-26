#include "o2/stdafx.h"
#include "WidgetLayer.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	WidgetLayer::WidgetLayer() :
		layout(this), interactableLayout(Vec2F(), Vec2F(1.0f, 1.0f), Vec2F(), Vec2F()), mUID(Math::Random())
	{}

	WidgetLayer::WidgetLayer(const WidgetLayer& other) :
		mDepth(other.mDepth), name(other.name), layout(this, other.layout), mTransparency(other.mTransparency),
		interactableLayout(other.interactableLayout), mUID(Math::Random()), depth(this), transparency(this)
	{
		if (other.mCopyVisitor)
			other.mCopyVisitor->OnCopy(&other, this);

		if (other.mDrawable) {
			mDrawable = other.mDrawable->CloneAsRef<IRectDrawable>();
			mDrawable->SetSerializeEnabled(false);
		}

		for (auto child : other.mChildren) {
			child->mCopyVisitor = other.mCopyVisitor;

			AddChild(child->CloneAsRef<WidgetLayer>());

			child->mCopyVisitor = nullptr;
		}
	}

	WidgetLayer::~WidgetLayer()
	{
		if (mParent)
			mParent.Lock()->RemoveChild(this);
		else if (mOwnerWidget)
			mOwnerWidget.Lock()->RemoveLayer(this);

		mDrawable = nullptr;

		for (auto child : mChildren) {
			child->mParent = nullptr;
			child->SetOwnerWidget(nullptr);
		}

		mChildren.Clear();
	}

	WidgetLayer& WidgetLayer::operator=(const WidgetLayer& other)
	{
		mChildren.Clear();
		mDrawable = nullptr;

		mDepth = other.mDepth;
		name = other.name;

		if (other.mDrawable) {
			mDrawable = other.mDrawable->CloneAsRef<IRectDrawable>();
			mDrawable->SetSerializeEnabled(false);
		}

		for (auto child : other.mChildren)
			AddChild(child->CloneAsRef<WidgetLayer>());

		SetTransparency(other.mTransparency);

		if (mOwnerWidget)
			mOwnerWidget.Lock()->UpdateLayersDrawingSequence();

		return *this;
	}

	const WeakRef<Widget>& WidgetLayer::GetOwnerWidget() const
	{
		if (mOwnerWidget)
			return mOwnerWidget;

		return mParent.Lock()->GetOwnerWidget();
	}

	const WeakRef<WidgetLayer>& WidgetLayer::GetPrototypeLink() const
	{
		return mPrototypeLink;
	}

	void WidgetLayer::SetDrawable(const Ref<IRectDrawable>& drawable)
	{
		mDrawable = drawable;

		if (mDrawable)
			mDrawable->SetSerializeEnabled(false);

		if (auto ownerWidget = mOwnerWidget.Lock()) {
			ownerWidget->UpdateLayersDrawingSequence();
			ownerWidget->UpdateTransform();
		}
	}

	const Ref<IRectDrawable>& WidgetLayer::GetDrawable() const
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
		bool parentEnabled = mOwnerWidget ? mOwnerWidget.Lock()->IsEnabledInHierarchy() : mParent.Lock()->IsEnabledInHierarchy();
		return mEnabled && parentEnabled;
	}

	void WidgetLayer::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	Ref<WidgetLayer> WidgetLayer::AddChild(const Ref<WidgetLayer>& layer)
	{
		if (layer->mParent)
			layer->mParent.Lock()->RemoveChild(layer.Get());
		else if (layer->mOwnerWidget)
			layer->mOwnerWidget.Lock()->RemoveLayer(layer);

		layer->mParent = Ref(this);
		mChildren.Add(layer);

		layer->SetOwnerWidget(mOwnerWidget.Lock());

		if (auto ownerWidget = mOwnerWidget.Lock()) {
			ownerWidget->OnLayerAdded(layer);
			ownerWidget->UpdateLayersDrawingSequence();
		}

#if IS_EDITOR
		o2Scene.OnObjectChanged(Ref(this));
		o2Scene.OnObjectChanged(layer);
#endif


		return layer;
	}

	void WidgetLayer::RemoveChild(WidgetLayer* layer)
	{
		if (!layer)
			return;

		layer->mParent = nullptr;
		mChildren.RemoveFirst([&](auto& x) { return x == layer; });

		auto lastOwnerWidget = layer->mOwnerWidget;
		layer->SetOwnerWidget(nullptr);

		if (lastOwnerWidget)
			lastOwnerWidget.Lock()->UpdateLayersDrawingSequence();

#if IS_EDITOR
		o2Scene.OnObjectChanged(Ref(this));
#endif
	}

	void WidgetLayer::RemoveAllChildren()
	{
		for (auto child : mChildren) {
			child->mParent = nullptr;
			child->SetOwnerWidget(nullptr);
		}

		mChildren.Clear();

#if IS_EDITOR
		o2Scene.OnObjectChanged(Ref(this));
#endif
	}

	void WidgetLayer::SetParent(const Ref<WidgetLayer>& parent)
	{
		if (parent)
			parent->AddChild(Ref(this));
		else {
			if (mParent)
				mParent.Lock()->RemoveChild(this);

			mParent = nullptr;
			SetOwnerWidget(nullptr);
		}
	}

	const WeakRef<WidgetLayer>& WidgetLayer::GetParent() const
	{
		return mParent;
	}

	Vector<Ref<WidgetLayer>>& WidgetLayer::GetChildren()
	{
		return mChildren;
	}

	const Vector<Ref<WidgetLayer>>& WidgetLayer::GetChildren() const
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

		if (auto childrenNode = node.FindMember("Children")) {
			for (auto& childNode : *childrenNode) {
				auto layer = mmake<WidgetLayer>();
				AddChild(layer);
				layer->Deserialize(childNode["Value"]);
			}
		}
	}

	void WidgetLayer::SerializeWithProto(DataValue& node) const
	{
		SerializeDelta(node, *mPrototypeLink);

		node["PrototypeLink"] = mPrototypeLink->mUID;

		if (!mChildren.IsEmpty()) {
			auto& childrenNode = node.AddMember("Children");
			childrenNode.SetArray();

			for (auto child : mChildren)
				child->Serialize(childrenNode.AddElement());
		}
	}

	void WidgetLayer::DeserializeWithProto(const DataValue& node)
	{
		if (auto protoNode = node.FindMember("PrototypeLink")) {
			SceneUID protoUID = *protoNode;
			if (mParent && mParent.Lock()->mPrototypeLink) {
				for (auto protoChild : mParent.Lock()->mPrototypeLink->mChildren) {
					if (protoChild->mUID == protoUID) {
						mPrototypeLink = protoChild;
						break;
					}
				}
			}
			else if (mOwnerWidget.Lock()->mPrototypeLink) {
				for (auto protoChild : dynamic_cast<Widget*>(mOwnerWidget.Lock()->mPrototypeLink.Get())->mLayers) {
					if (protoChild->mUID == protoUID) {
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

		if (auto childrenNode = node.FindMember("Children")) {
			for (auto& childNode : *childrenNode) {
				auto layer = mmake<WidgetLayer>();
				AddChild(layer);
				layer->Deserialize(childNode);
			}
		}
	}

	void WidgetLayer::OnDeserialized(const DataValue& node)
	{
		for (auto child : mChildren) {
			child->mParent = Ref(this);
			child->mOwnerWidget = mOwnerWidget;
		}

		if (mDrawable)
			mDrawable->SetSerializeEnabled(false);
	}

	void WidgetLayer::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		OnDeserialized(node);
	}

	Ref<WidgetLayer> WidgetLayer::AddChildLayer(const String& name, const Ref<IRectDrawable>& drawable,
												const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mOwnerWidget.Lock()->mDrawingLayers.Count();

		auto layer = mmake<WidgetLayer>();
		layer->depth = depth;
		layer->name = name;
		layer->mDrawable = drawable;
		layer->layout = layout;

		return AddChild(layer);
	}

	Ref<WidgetLayer> WidgetLayer::GetChild(const String& path)
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		if (pathPart == "..") {
			if (mParent) {
				if (delPos == -1)
					return mParent.Lock();
				else
					return mParent.Lock()->GetChild(path.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mChildren) {
			if (child->name == pathPart) {
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	Ref<WidgetLayer> WidgetLayer::FindChild(const String& name)
	{
		for (auto child : mChildren) {
			if (child->name == name)
				return child;

			auto layer = child->FindChild(name);
			if (layer)
				return layer;
		}

		return nullptr;
	}

	Vector<Ref<WidgetLayer>> WidgetLayer::GetAllChilds() const
	{
		Vector<Ref<WidgetLayer>> res = mChildren;
		for (auto child : mChildren) {
			res.Add(child->GetAllChilds());
		}

		return res;
	}

	void WidgetLayer::SetDepth(float depth)
	{
		mDepth = depth;
		if (mOwnerWidget)
			mOwnerWidget.Lock()->UpdateLayersDrawingSequence();
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

	void WidgetLayer::SetOwnerWidget(const Ref<Widget>& owner)
	{
		mOwnerWidget = owner;

#if IS_EDITOR
		if (Scene::IsSingletonInitialzed()) {
			if (mOwnerWidget && mOwnerWidget.Lock()->mState == Actor::State::Default)
				o2Scene.AddEditableObjectToScene(Ref(this));
			else
				o2Scene.RemoveEditableObjectFromScene(Ref(this));
		}
#endif

		for (auto child : mChildren)
			child->SetOwnerWidget(owner);

		UpdateResTransparency();
	}

	void WidgetLayer::OnLayoutChanged()
	{
		if (mUpdatingLayout)
			return;

		mUpdatingLayout = true;

		if (mOwnerWidget) {
			mOwnerWidget.Lock()->UpdateLayersLayouts();

#if IS_EDITOR
			mOwnerWidget.Lock()->OnChanged();
#endif
		}

		mUpdatingLayout = false;
	}

	void WidgetLayer::UpdateLayout()
	{
		if (mParent)
			mAbsolutePosition = layout.Calculate(mParent.Lock()->mAbsolutePosition);
		else
			mAbsolutePosition = layout.Calculate(mOwnerWidget.Lock()->layout->GetWorldRect());

		mInteractableArea = interactableLayout.Calculate(mAbsolutePosition);

		if (mDrawable)
			mDrawable->SetRect(mAbsolutePosition);

		for (auto child : mChildren)
			child->UpdateLayout();
	}

	void WidgetLayer::UpdateResTransparency()
	{
		if (mParent)
			mResTransparency = transparency * mParent.Lock()->mResTransparency;
		else if (mOwnerWidget)
			mResTransparency = transparency * mOwnerWidget.Lock()->mResTransparency;
		else
			mResTransparency = mTransparency;

		if (mDrawable)
			mDrawable->SetTransparency(mResTransparency);

		for (auto child : mChildren)
			child->UpdateResTransparency();
	}

	void WidgetLayer::OnAddToScene()
	{
#if IS_EDITOR
		o2Scene.AddEditableObjectToScene(Ref(this));
#endif

		for (auto layer : mChildren)
			layer->OnAddToScene();
	}

	void WidgetLayer::OnRemoveFromScene()
	{
#if IS_EDITOR
		o2Scene.RemoveEditableObjectFromScene(Ref(this));
#endif

		for (auto layer : mChildren)
			layer->OnRemoveFromScene();
	}

	Map<String, Ref<WidgetLayer>> WidgetLayer::GetAllChildLayers()
	{
		Map<String, Ref<WidgetLayer>> res;
		for (auto layer : mChildren)
			res.Add(layer->name, layer);

		return res;
	}

	void WidgetLayer::InstantiatePrototypeCloneVisitor::OnCopy(const WidgetLayer* source, WidgetLayer* target)
	{
		target->mPrototypeLink = Ref(const_cast<WidgetLayer*>(source));
	}

	void WidgetLayer::MakePrototypeCloneVisitor::OnCopy(const WidgetLayer* source, WidgetLayer* target)
	{
		target->mPrototypeLink = source->mPrototypeLink;
		const_cast<WidgetLayer*>(source)->mPrototypeLink = Ref(target);
	}

#if IS_EDITOR

	bool WidgetLayer::IsOnScene() const
	{
		if (mOwnerWidget)
			return mOwnerWidget.Lock()->IsOnScene();

		return false;
	}

	SceneUID WidgetLayer::GetID() const
	{
		return mUID;
	}

	void WidgetLayer::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();

		if (childs) {
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

	Vector<Ref<SceneEditableObject>> WidgetLayer::GetEditableChildren() const
	{
		return mChildren.Convert<Ref<SceneEditableObject>>([](auto& x) { return DynamicCast<SceneEditableObject>(x); });
	}

	const Ref<SceneEditableObject> o2::WidgetLayer::GetEditableLink() const
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

	void WidgetLayer::SetEditableParent(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (auto layer = dynamic_cast<WidgetLayer*>(object))
			layer->AddChild(this);
		else if (auto widget = dynamic_cast<Widget*>(object))
			widget->AddLayer(this);
		else if (auto layers = dynamic_cast<Widget::LayersEditable*>(object))
			layers->AddEditableChild(this, idx);
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
		if (mParent) {
			int lastIdx = mParent->mChildren.IndexOf(this);
			mParent->mChildren.Insert(this, idx);

			if (idx <= lastIdx)
				lastIdx++;

			mParent->mChildren.RemoveAt(lastIdx);
		}
		else {
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
		return Basis(mAbsolutePosition.LeftBottom(), Vec2F::Right() * mAbsolutePosition.Width(), Vec2F::Up() * mAbsolutePosition.Height());
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
		if (mOwnerWidget) {
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
// --- META ---

DECLARE_CLASS(o2::WidgetLayer, o2__WidgetLayer);
// --- END META ---
