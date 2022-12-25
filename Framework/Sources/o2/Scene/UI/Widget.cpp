#include "o2/stdafx.h"
#include "Widget.h"

#include "o2/Application/Input.h"
#include "o2/Render/Render.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{
	Widget::Widget(ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew WidgetLayout(), mode), layout(dynamic_cast<WidgetLayout*>(transform))
	{
		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew WidgetLayout(), prototype, mode), layout(dynamic_cast<WidgetLayout*>(transform))
	{
		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew WidgetLayout(), components, mode), layout(dynamic_cast<WidgetLayout*>(transform))
	{
		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(const Widget& other) :
		Actor(mnew WidgetLayout(*other.layout), other), layout(dynamic_cast<WidgetLayout*>(transform)),
		mTransparency(other.mTransparency), transparency(this), resTransparency(this),
		childrenWidgets(this), layers(this), states(this), childWidget(this), layer(this), state(this)
	{
		layout->SetOwner(this);

		WidgetLayer::ICopyVisitor* layerCopyVisitor = nullptr;

		if (dynamic_cast<InstantiatePrototypeCloneVisitor*>(other.mCopyVisitor) || other.mIsAsset)
			layerCopyVisitor = mnew WidgetLayer::InstantiatePrototypeCloneVisitor();

		if (dynamic_cast<MakePrototypeCloneVisitor*>(other.mCopyVisitor))
			layerCopyVisitor = mnew WidgetLayer::MakePrototypeCloneVisitor();

		if constexpr (IS_EDITOR)
		{
			if (dynamic_cast<InstantiatePrototypeCloneVisitor*>(other.mCopyVisitor) || other.mIsAsset)
			{
				layersEditable.prototypeLink = &other.layersEditable;
				internalChildrenEditable.prototypeLink = &other.internalChildrenEditable;
			}
		}

		for (auto layer : other.mLayers)
		{
			layer->mCopyVisitor = layerCopyVisitor;

			auto newLayer = mnew WidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		if (layerCopyVisitor)
			delete layerCopyVisitor;

		for (auto child : mChildren)
		{
			Widget* childWidget = dynamic_cast<Widget*>(child);
			if (childWidget)
			{
				childWidget->mParentWidget = this;
				mChildWidgets.Add(childWidget);
				OnChildAdded(childWidget);
			}

			child->OnParentChanged(nullptr);
		}

		for (auto child : other.mInternalWidgets)
		{
			auto newChild = child->CloneAs<Widget>();
			newChild->RemoveFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			WidgetState* newState = dynamic_cast<WidgetState*>(state->Clone());
			AddState(newState, false);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();
	}

	Widget::~Widget()
	{
		if (mParent)
			mParent->OnChildRemoved(this);

		for (auto layer : mLayers)
		{
			layer->mOwnerWidget = nullptr;
			delete layer;
		}

		for (auto state : mStates)
			delete state;

		for (auto child : mInternalWidgets)
		{
			child->mParent = nullptr;
			child->mParentWidget = nullptr;

			delete child;
		}

		if (UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Remove(this);

		if (IsOnScene())
			ISceneDrawable::OnRemoveFromScene();
	}

	Widget& Widget::operator=(const Widget& other)
	{
		auto layers = mLayers;
		for (auto layer : layers)
			delete layer;

		auto states = mStates;
		for (auto state : states)
			delete state;

		auto internalChildren = mInternalWidgets;
		for (auto child : internalChildren)
			delete child;

		mInternalWidgets.Clear();
		mLayers.Clear();
		mStates.Clear();
		mVisibleState = nullptr;
		mFocusedState = nullptr;

		layout->CopyFrom(*other.layout);
		mTransparency = other.mTransparency;
		mIsFocusable = other.mIsFocusable;

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew WidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			Widget* childWidget = dynamic_cast<Widget*>(child);
			if (childWidget)
			{
				mChildWidgets.Add(childWidget);
				OnChildAdded(childWidget);
			}
		}

		for (auto child : other.mInternalWidgets)
		{
			auto newChild = child->CloneAs<Widget>();
			newChild->RemoveFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			WidgetState* newState = dynamic_cast<WidgetState*>(state->Clone());
			AddState(newState, false);
		}

		UpdateLayersDrawingSequence();

		return *this;
	}

	void Widget::Update(float dt)
	{
		if (mResEnabledInHierarchy)
		{
			if (GetLayoutData().updateFrame == 0)
			{
				for (auto child : mChildren)
					child->transform->SetDirty(true);

				for (auto child : mInternalWidgets)
					child->transform->SetDirty(true);

				UpdateSelfTransform();
			}

			if (!mIsClipped)
			{
				for (auto state : mStates)
				{
					if (state)
						state->Update(dt);
				}
			}

			for (auto comp : mComponents)
				comp->Update(dt);
		}
	}

	void Widget::UpdateChildren(float dt)
	{
		for (auto child : mChildren)
			child->Update(dt);

		RectF childrenWorldRect = GetLayoutData().childrenWorldRect;
		GetLayoutData().childrenWorldRect = GetLayoutData().worldRectangle;

		for (auto child : mInternalWidgets)
			child->Update(dt);

		GetLayoutData().childrenWorldRect = childrenWorldRect;

		for (auto child : mChildren)
			child->UpdateChildren(dt);

		GetLayoutData().childrenWorldRect = GetLayoutData().worldRectangle;

		for (auto child : mInternalWidgets)
			child->UpdateChildren(dt);

		GetLayoutData().childrenWorldRect = childrenWorldRect;
	}

	void Widget::UpdateTransform()
	{
		if (GetLayoutData().drivenByParent && mParentWidget)
		{
			mParentWidget->UpdateTransform();
		}

		UpdateSelfTransform();
		UpdateChildrenTransforms();
	}

	void Widget::UpdateChildrenTransforms()
	{
		Actor::UpdateChildrenTransforms();

		RectF childrenWorldRect = GetLayoutData().childrenWorldRect;
		GetLayoutData().childrenWorldRect = GetLayoutData().worldRectangle;

		for (auto child : mInternalWidgets)
			child->UpdateSelfTransform();

		for (auto child : mInternalWidgets)
			child->UpdateChildrenTransforms();

		GetLayoutData().childrenWorldRect = childrenWorldRect;
	}

	void Widget::SetLayoutDirty()
	{
		layout->SetDirty(false);
	}

	void Widget::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
		{
			if (mIsClipped)
			{
				for (auto child : mChildrenInheritedDepth)
					child->Draw();
			}

			return;
		}

		for (auto layer : mDrawingLayers)
			layer->Draw();

		OnDrawn();

		for (auto child : mChildrenInheritedDepth)
			child->Draw();

		for (auto child : mInternalWidgets)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void Widget::DrawDebugFrame()
	{
		if (!IsUIDebugEnabled() && !o2Input.IsKeyDown(VK_F2))
			return;

		static int colr = 0;
		static int lastFrame = 0;

		if (lastFrame != o2Time.GetCurrentFrame())
			colr = 0;

		lastFrame = o2Time.GetCurrentFrame();

		o2Render.DrawRectFrame(mBoundsWithChilds, Color4::SomeColor(colr++));
	}

	void Widget::SerializeRaw(DataValue& node) const
	{
		Actor::SerializeRaw(node);

		node["InternalWidgets"] = mInternalWidgets;
		node["Layers"] = mLayers;
		node["States"] = mStates;
	}

	void Widget::DeserializeRaw(const DataValue& node)
	{
		Actor::DeserializeRaw(node);

		if (auto internalWidgetsNode = node.FindMember("InternalWidgets"))
			mInternalWidgets = *internalWidgetsNode;

		if (auto layersNode = node.FindMember("Layers"))
			mLayers = *layersNode;

		if (auto statesNode = node.FindMember("States"))
			mStates = *statesNode;
	}

	void Widget::SerializeWithProto(DataValue& node) const
	{
		Actor::SerializeWithProto(node);

		if (!mInternalWidgets.IsEmpty())
		{
			auto& internalWidgetsNode = node.AddMember("InternalWidgets");
			internalWidgetsNode.SetArray();
			for (auto widget : mInternalWidgets)
			{
				auto& widgetNode = internalWidgetsNode.AddElement();
				widgetNode.AddMember("Type") = widget->GetType().GetName();
				widget->Serialize(widgetNode.AddMember("Data"));
			}
		}

		if (!mLayers.IsEmpty())
		{
			auto& layersNode = node.AddMember("Layers");
			layersNode.SetArray();
			for (auto layer : mLayers)
				layer->Serialize(layersNode.AddElement());
		}

		if (!mStates.IsEmpty())
		{
			const Widget* proto = dynamic_cast<const Widget*>(mPrototypeLink.Get());
			auto& statesNode = node.AddMember("States");
			for (auto state : mStates)
			{
				if (auto protoState = proto->GetStateObject(state->name))
				{
					auto& stateNode = statesNode.AddElement();
					stateNode["mName"] = state->name;
					stateNode.SetDelta(*state, *protoState);
				}
				else
					statesNode.AddElement().Set(*state);
			}
		}
	}

	void Widget::DeserializeWithProto(const DataValue& node)
	{
		for (auto layer : mLayers)
		{
			layer->mOwnerWidget = nullptr;
			delete layer;
		}

		for (auto state : mStates)
			delete state;

		for (auto child : mInternalWidgets)
		{
			child->mParent = nullptr;
			child->mParentWidget = nullptr;

			delete child;
		}

		mLayers.Clear();
		mStates.Clear();
		mInternalWidgets.Clear();

		Actor::DeserializeWithProto(node);

		auto internalWidgetsNode = node.FindMember("InternalWidgets");
		if (internalWidgetsNode && internalWidgetsNode->IsArray())
		{
			for (auto& widgetNode : *internalWidgetsNode)
			{
				const DataValue* typeNode = widgetNode.FindMember("Type");
				const DataValue* dataValue = widgetNode.FindMember("Data");
				if (typeNode && dataValue)
				{
					const ObjectType* type = dynamic_cast<const ObjectType*>(o2Reflection.GetType(*typeNode));
					if (type)
					{
						Widget* widget = dynamic_cast<Widget*>(type->DynamicCastToIObject(type->CreateSample()));
						AddInternalWidget(widget);
						widget->mCopyVisitor = mCopyVisitor;
						widget->Deserialize(*dataValue);
						widget->mCopyVisitor = nullptr;
					}
				}
			}
		}

		auto layersNode = node.FindMember("Layers");
		if (layersNode && layersNode->IsArray())
		{
			for (auto& layerNode : *layersNode)
			{
				WidgetLayer* layer = mnew WidgetLayer();
				AddLayer(layer);
				layer->Deserialize(layerNode);
			}
		}

		auto statesNode = node.FindMember("States");
		if (statesNode && statesNode->IsArray())
		{
			const Widget* proto = dynamic_cast<const Widget*>(mPrototypeLink.Get());

			for (auto& stateNode : *statesNode)
			{
				WidgetState* state = mnew WidgetState();
				stateNode["mName"].Get(state->name);
				if (auto protoState = proto->GetStateObject(state->name))
					stateNode.GetDelta(*state, *protoState);
				else
					stateNode.Get(*state);

				AddState(state, false);
			}
		}
	}

	void Widget::OnDeserialized(const DataValue& node)
	{
		for (auto layer : mLayers)
			layer->SetOwnerWidget(this);

		for (auto layer : mLayers)
			OnLayerAdded(layer);

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			Widget* childWidget = dynamic_cast<Widget*>(child);
			if (childWidget)
			{
				childWidget->mParentWidget = this;
				mChildWidgets.Add(childWidget);
				OnChildAdded(childWidget);
			}
		}

		for (auto child : mInternalWidgets)
		{
			child->mParent = this;
			child->mParentWidget = this;

			child->RemoveFromScene();
		}

		RetargetStatesAnimations();
		SetEnabledForcible(mEnabled);
		UpdateLayersDrawingSequence();
	}

	void Widget::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		OnDeserialized(node);
	}

	void Widget::OnTransformUpdated()
	{
		mIsClipped = false;
		Actor::OnTransformUpdated();
		UpdateLayersLayouts();
		onLayoutUpdated();
	}

	SceneLayer* Widget::GetSceneDrawableSceneLayer() const
	{
		return mLayer;
	}

	bool Widget::IsSceneDrawableEnabled() const
	{
		return mResEnabledInHierarchy;
	}

	ISceneDrawable* Widget::GetParentDrawable()
	{
		if (mParentWidget)
			return (ISceneDrawable*)mParentWidget;

		if (mParent)
		{
			auto itParent = mParent;
			while (itParent)
			{
				if (auto drawable = dynamic_cast<ISceneDrawable*>(itParent))
					return drawable;

				auto comp = itParent->GetComponent<DrawableComponent>();
				if (comp)
					return (ISceneDrawable*)comp;

				itParent = itParent->mParent;
			}
		}

		return nullptr;
	}

	int Widget::GetIndexInParentDrawable() const
	{
		if (mParent)
			return mParent->mChildren.IndexOf((Actor*)this);

		return 0;
	}

	void Widget::OnFocused()
	{
		onFocused();
	}

	void Widget::OnUnfocused()
	{
		onUnfocused();
	}

	Widget* Widget::GetParentWidget() const
	{
		return mParentWidget;
	}

	const RectF& Widget::GetChildrenWorldRect() const
	{
		return GetLayoutData().childrenWorldRect;
	}

	const Vector<Widget*>& Widget::GetChildWidgets() const
	{
		return mChildWidgets;
	}

	Actor* Widget::FindActorById(SceneUID id)
	{
		if (auto res = Actor::FindActorById(id))
			return res;

		for (auto widget : mInternalWidgets)
		{
			if (auto res = widget->FindActorById(id))
				return res;
		}

		return nullptr;
	}

	String Widget::GetCreateMenuCategory()
	{
		return "UI";
	}

	WidgetLayer* Widget::AddLayer(WidgetLayer* layer)
	{
		if (layer->mParent)
			layer->mParent->RemoveChild(layer, false);
		else if (layer->mOwnerWidget)
			layer->mOwnerWidget->RemoveLayer(layer, false);

		mLayers.Add(layer);
		layer->SetOwnerWidget(this);
		UpdateLayersDrawingSequence();
		OnLayerAdded(layer);

		if constexpr (IS_EDITOR)
		{
			if (Scene::IsSingletonInitialzed() && IsOnScene())
			{
				o2Scene.OnObjectChanged(&layersEditable);
				o2Scene.onChildrenHierarchyChanged(&layersEditable);
			}
		}

		return layer;
	}

	WidgetLayer* Widget::AddLayer(const String& name, IRectDrawable* drawable,
								  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mDrawingLayers.Count();

		WidgetLayer* layer = mnew WidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->SetDrawable(drawable);
		layer->layout = layout;

		AddLayer(layer);

		return layer;
	}

	WidgetLayer* Widget::GetLayer(const String& path) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		for (auto layer : mLayers)
		{
			if (layer->name == pathPart)
			{
				if (delPos == -1)
					return layer;
				else
					return layer->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	WidgetLayer* Widget::FindLayer(const String& name) const
	{
		for (auto childLayer : mLayers)
		{
			if (childLayer->name == name)
				return childLayer;

			WidgetLayer* layer = childLayer->FindChild(name);
			if (layer)
				return layer;
		}

		return nullptr;
	}

	void Widget::RemoveLayer(WidgetLayer* layer, bool release /*= true*/)
	{
		layer->SetOwnerWidget(nullptr);

		mLayers.Remove(layer);

		if (release)
			delete layer;

		UpdateLayersDrawingSequence();
		OnChildrenChanged();
	}

	void Widget::RemoveLayer(const String& path)
	{
		auto layer = GetLayer(path);

		if (!layer)
			return;

		if (layer->GetParent())
		{
			layer->GetParent()->RemoveChild(layer);
			return;
		}

		mLayers.Remove(layer);
		UpdateLayersDrawingSequence();
	}

	void Widget::RemoveAllLayers()
	{
		for (auto layer : mLayers)
			delete layer;

		mDrawingLayers.Clear();

		mLayers.Clear();
	}

	const Vector<WidgetLayer*>& Widget::GetLayers() const
	{
		return mLayers;
	}

	WidgetState* Widget::AddState(const String& name)
	{
		WidgetState* newState = mnew WidgetState();
		newState->name = name;

		return AddState(newState);
	}

	WidgetState* Widget::AddState(const String& name, const AnimationClip& animation)
	{
		WidgetState* newState = mnew WidgetState();
		newState->name = name;
		newState->animationClip = animation;

		return AddState(newState);
	}

	WidgetState* Widget::AddState(const String& name, const AnimationAssetRef& animation)
	{
		WidgetState* newState = mnew WidgetState();
		newState->name = name;
		newState->animationAsset = animation;

		return AddState(newState);
	}

	WidgetState* Widget::AddState(WidgetState* state, bool showAnimErrors /*= true*/)
	{
		mStates.Add(state);
		state->SetOwner(this, showAnimErrors);

		if (state->name == "visible")
		{
			mVisibleState = state;
			mVisibleState->SetStateForcible(mEnabled);

			mVisibleState->onStateBecomesTrue += [&]()
			{
				mResEnabled = true;
				UpdateResEnabledInHierarchy();
			};

			mVisibleState->onStateFullyFalse += [&]()
			{
				mResEnabled = false;
				UpdateResEnabledInHierarchy();
			};
		}

		if (state->name == "focused")
			mFocusedState = state;

		OnStateAdded(state);

		return state;
	}

	bool Widget::RemoveState(const String& name)
	{
		int idx = mStates.IndexOf([&](WidgetState* state) { return state->name == name; });
		if (idx < 0)
			return false;

		if (mStates[idx] == mVisibleState)
			mVisibleState = nullptr;

		if (mStates[idx] == mFocusedState)
			mFocusedState = nullptr;

		delete mStates[idx];
		mStates.RemoveAt(idx);

		return true;
	}

	bool Widget::RemoveState(WidgetState* state)
	{
		int idx = mStates.IndexOf(state);
		if (idx < 0)
			return false;

		if (state == mVisibleState)
			mVisibleState = nullptr;

		delete mStates[idx];
		mStates.RemoveAt(idx);

		return true;
	}

	void Widget::RemoveAllStates()
	{
		for (auto state : mStates)
			delete state;

		mVisibleState = nullptr;
		mFocusedState = nullptr;

		mStates.Clear();
	}

	void Widget::SetState(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetState(state);
	}

	void Widget::SetStateForcible(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetStateForcible(state);
	}

	bool Widget::GetState(const String& name) const
	{
		auto state = GetStateObject(name);

		if (state)
			return state->GetState();

		return false;
	}

	WidgetState* Widget::GetStateObject(const String& name) const
	{
		return mStates.FindOrDefault([&](auto state) { return state->name == name; });
	}

	const Vector<WidgetState*>& Widget::GetStates() const
	{
		return mStates;
	}

	void Widget::SetTransparency(float transparency)
	{
		mTransparency = transparency;
		UpdateTransparency();
	}

	float Widget::GetTransparency() const
	{
		return mTransparency;
	}

	float Widget::GetResTransparency() const
	{
		return mResTransparency;
	}

	void Widget::SetEnabledForcible(bool visible)
	{
		if (mVisibleState)
			mVisibleState->SetStateForcible(visible);

		mEnabled = visible;
		Widget::UpdateResEnabled();
		Widget::UpdateTransparency();
	}

	void Widget::Show(bool forcible /*= false*/)
	{
		if (forcible)
			SetEnabledForcible(true);
		else
			SetEnabled(true);
	}

	void Widget::Hide(bool forcible /*= false*/)
	{
		if (forcible)
			SetEnabledForcible(false);
		else
			SetEnabled(false);
	}

	void Widget::Focus()
	{
		o2UI.FocusWidget(this);
	}

	void Widget::Unfocus()
	{
		o2UI.FocusWidget(nullptr);
	}

	bool Widget::IsFocused() const
	{
		return mIsFocused;
	}

	bool Widget::IsFocusable() const
	{
		return mIsFocusable;
	}

	void Widget::SetFocusable(bool selectable)
	{
		mIsFocusable = selectable;
	}

	bool Widget::IsUnderPoint(const Vec2F& point)
	{
		return mDrawingScissorRect.IsInside(point) && layout->IsPointInside(point);
	}

	void Widget::SetIndexInSiblings(int index)
	{
		Actor::SetIndexInSiblings(index);

		if (mParentWidget)
			mParentWidget->UpdateChildWidgetsList();
	}

	float Widget::GetMinWidthWithChildren() const
	{
		return GetLayoutData().minSize.x;
	}

	float Widget::GetMinHeightWithChildren() const
	{
		return GetLayoutData().minSize.y;
	}

	float Widget::GetWidthWeightWithChildren() const
	{
		return GetLayoutData().weight.x;
	}

	float Widget::GetHeightWeightWithChildren() const
	{
		return GetLayoutData().weight.y;
	}

	void Widget::UpdateBoundsWithChilds()
	{
		if ((!mResEnabledInHierarchy || mIsClipped) && GetLayoutData().dirtyFrame != o2Time.GetCurrentFrame())
			return;

		mBoundsWithChilds = mBounds;

		for (auto child : mChildWidgets)
			mBoundsWithChilds.Expand(child->mBoundsWithChilds);

		if (mParentWidget)
			mParentWidget->UpdateBoundsWithChilds();
	}

	void Widget::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		for (auto child : mChildWidgets)
			child->CheckClipping(clipArea);
	}

	void Widget::UpdateTransparency()
	{
		if (mParentWidget)
			mResTransparency = mTransparency * mParentWidget->mResTransparency;
		else
			mResTransparency = mTransparency;

		for (auto layer : mLayers)
			layer->UpdateResTransparency();

		for (auto child : mChildWidgets)
			child->UpdateTransparency();

		for (auto child : mInternalWidgets)
			child->UpdateTransparency();
	}

	void Widget::UpdateVisibility(bool updateLayout /*= true*/)
	{
	}

	void Widget::OnChildFocused(Widget* child)
	{
		if (mParentWidget)
			mParentWidget->OnChildFocused(child);
	}

	void Widget::RetargetStatesAnimations()
	{
		for (auto state : mStates)
		{
			state->player.SetTarget(this, false);
			state->player.relTime = state->GetState() ? 1.0f : 0.0f;
		}
	}

	void Widget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();

		UpdateBounds();
	}

	void Widget::UpdateBounds()
	{
		if ((!mResEnabledInHierarchy || mIsClipped) && GetLayoutData().dirtyFrame != o2Time.GetCurrentFrame())
			return;

		mBounds = GetLayoutData().worldRectangle;

		for (auto layer : mDrawingLayers)
			mBounds.Expand(layer->GetRect());

		bool anyEnabled = false;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
			{
				anyEnabled = true;
				break;
			}
		}

		if (!anyEnabled)
			UpdateBoundsWithChilds();
	}

	void Widget::UpdateLayersDrawingSequence()
	{
		const float topLayersDepth = 1000.0f;

		mDrawingLayers.Clear();
		mTopDrawingLayers.Clear();

		for (auto layer : mLayers)
		{
			if (layer->GetDrawable())
			{
				if (layer->mDepth < topLayersDepth)
					mDrawingLayers.Add(layer);
				else
					mTopDrawingLayers.Add(layer);
			}

			auto childLayers = layer->GetAllChilds();
			for (auto childLayer : childLayers)
			{
				if (childLayer->GetDrawable())
				{
					if (childLayer->mDepth < topLayersDepth)
						mDrawingLayers.Add(childLayer);
					else
						mTopDrawingLayers.Add(childLayer);
				}
			}
		}

		mDrawingLayers.Sort([](auto a, auto b) { return a->mDepth < b->mDepth; });
		mTopDrawingLayers.Sort([](auto a, auto b) { return a->mDepth < b->mDepth; });
	}

	void Widget::SetParentWidget(Widget* widget)
	{
		SetParent(widget);
	}

	Widget* Widget::GetChildWidget(const String& path) const
	{
		Actor* actor = GetChild(path);
		return dynamic_cast<Widget*>(actor);
	}

	Actor* Widget::AddChild(Actor* actor)
	{
		return Actor::AddChild(actor);
	}

	Actor* Widget::AddChild(Actor* actor, int index)
	{
		Actor::AddChild(actor, index);
		UpdateChildWidgetsList();
		return actor;
	}

	Widget* Widget::AddChildWidget(Widget* widget)
	{
		return dynamic_cast<Widget*>(AddChild(widget));
	}

	Widget* Widget::AddChildWidget(Widget* widget, int position)
	{
		return dynamic_cast<Widget*>(AddChild(widget, position));
	}

	Vector<Widget*> Widget::GetChildrenNonConst()
	{
		return mChildWidgets;
	}

	Vector<WidgetLayer*> Widget::GetLayersNonConst()
	{
		return mLayers;
	}

	Vector<WidgetState*> Widget::GetStatesNonConst()
	{
		return mStates;
	}

	Map<String, WidgetLayer*> Widget::GetAllLayers()
	{
		Map<String, WidgetLayer*> res;
		for (auto layer : mLayers)
			res.Add(layer->name, layer);

		return res;
	}

	Map<String, Widget*> Widget::GetAllChilds()
	{
		Map<String, Widget*> res;
		for (auto child : mChildWidgets)
			res.Add(child->GetName(), child);

		return res;
	}

	Map<o2::String, Widget*> Widget::GetAllInternalWidgets()
	{
		Map<String, Widget*> res;
		for (auto child : mInternalWidgets)
			res.Add(child->GetName(), child);

		return res;
	}

	Map<String, WidgetState*> Widget::GetAllStates()
	{
		Map<String, WidgetState*> res;
		for (auto state : mStates)
			res.Add(state->name, state);

		return res;
	}

	void Widget::OnLayerAdded(WidgetLayer* layer)
	{
	}

	void Widget::OnStateAdded(WidgetState* state)
	{
	}

	void Widget::OnStatesListChanged()
	{
		auto statesCopy = mStates;
		mStates.Clear();

		for (auto state : statesCopy)
			AddState(state);
	}

	void Widget::OnParentChanged(Actor* oldParent)
	{
		layout->SetDirty();

		mParentWidget = dynamic_cast<Widget*>(mParent);

		if (mParent && mParent->IsOnScene())
			AddToScene();

		OnDrawbleParentChanged();
	}

	void Widget::OnChildAdded(Actor* child)
	{
		layout->SetDirty(false);

		Widget* widget = dynamic_cast<Widget*>(child);
		if (widget)
		{
			UpdateChildWidgetsList();

			OnChildAdded(widget);
		}
	}

	void Widget::OnChildAdded(Widget* child)
	{
	}

	void Widget::OnChildRemoved(Actor* child)
	{
		layout->SetDirty();

		Widget* widget = dynamic_cast<Widget*>(child);
		if (widget)
		{
			mChildWidgets.Remove(widget);
			mInternalWidgets.Remove(widget);

			OnChildRemoved(widget);
		}
	}

	void Widget::OnChildRemoved(Widget* child)
	{
	}

	void Widget::OnRemoveFromScene()
	{
		Actor::OnRemoveFromScene();
		ISceneDrawable::OnRemoveFromScene();

		if constexpr (IS_EDITOR)
		{
			o2Scene.mEditableObjects.Remove(&layersEditable);
			o2Scene.mEditableObjects.Remove(&internalChildrenEditable);
		}

		for (auto layer : mLayers)
			layer->OnRemoveFromScene();

		for (auto child : mInternalWidgets)
			child->OnRemoveFromScene();

	}

	void Widget::OnAddToScene()
	{
		Actor::OnAddToScene();
		ISceneDrawable::OnAddToScene();

		for (auto child : mInternalWidgets)
			child->OnAddToScene();

		for (auto layer : mLayers)
			layer->OnAddToScene();

		if constexpr (IS_EDITOR)
		{
			o2Scene.mEditableObjects.Add(&layersEditable);
			o2Scene.mEditableObjects.Add(&internalChildrenEditable);
		}
	}

	void Widget::UpdateChildWidgetsList()
	{
		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			if (auto widget = dynamic_cast<Widget*>(child))
				mChildWidgets.Add(widget);
		}
	}

	WidgetLayoutData& Widget::GetLayoutData()
	{
		return *layout->mData;
	}

	const WidgetLayoutData& Widget::GetLayoutData() const
	{
		return *layout->mData;
	}

	void Widget::SetChildrenWorldRect(const RectF& childrenWorldRect)
	{
		layout->mData->childrenWorldRect = childrenWorldRect;
	}

	void Widget::ForceDraw(const RectF& area, float transparency)
	{
		Vec2F oldLayoutOffsetMin = GetLayoutData().offsetMin;
		Vec2F oldLayoutOffsetMax = GetLayoutData().offsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;
		auto oldParentWidget = mParentWidget;
		bool oldResEnabledInHierarchy = mEnabled;

		GetLayoutData().offsetMin = area.LeftBottom();
		GetLayoutData().offsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mParentWidget = nullptr;
		mIsClipped = false;

		SetEnabledForcible(true);

		UpdateSelfTransform();
		UpdateChildrenTransforms();
		UpdateTransparency();

		Draw();

		GetLayoutData().offsetMin = oldLayoutOffsetMin;
		GetLayoutData().offsetMax = oldLayoutOffsetMax;
		mTransparency = oldTransparency;
		mParent = oldParent;
		mParentWidget = oldParentWidget;
		mIsClipped = false;

		SetEnabledForcible(oldResEnabledInHierarchy);

		UpdateSelfTransform();
		UpdateChildrenTransforms();

		GetLayoutData().dirtyFrame = o2Time.GetCurrentFrame();

		UpdateBounds();
		UpdateBoundsWithChilds();
		UpdateTransparency();
	}

	void Widget::UpdateResEnabled()
	{
		if (mVisibleState)
			mVisibleState->SetState(mEnabled);
		else
			Actor::UpdateResEnabled();
	}

	void Widget::UpdateResEnabledInHierarchy()
	{
		bool lastResEnabledInHierarchy = mResEnabledInHierarchy;

		if (mParent)
			mResEnabledInHierarchy = mResEnabled && mParent->mResEnabledInHierarchy;
		else
			mResEnabledInHierarchy = mResEnabled;

		mIsClipped = false;

		if (lastResEnabledInHierarchy != mResEnabledInHierarchy)
		{
			if (mResEnabledInHierarchy)
			{
				onShow();

				if (mLayer && mState == Actor::State::InScene)
				{
					mLayer->OnActorEnabled(this);
					ISceneDrawable::OnEnabled();
				}
			}
			else
			{
				onHide();

				if (mLayer && mState == Actor::State::InScene)
				{
					mLayer->OnActorDisabled(this);
					ISceneDrawable::OnDisabled();
				}
			}

			layout->SetDirty(false);

			if constexpr (IS_EDITOR)
			{
				if (IsOnScene())
					o2Scene.onEnableChanged(this);
			}

			OnEnableInHierarchyChanged();
			OnChanged();
		}

		for (auto comp : mComponents)
			comp->UpdateEnabled();

		for (auto child : mChildren)
			child->UpdateResEnabledInHierarchy();

		for (auto child : mInternalWidgets)
			child->UpdateResEnabledInHierarchy();
	}

	void Widget::SetInternalParent(Widget* parent, bool worldPositionStays /*= false*/)
	{
		SetParent(parent, worldPositionStays);

		if (parent) 
		{
			parent->mChildren.Remove(this);
			parent->mChildWidgets.Remove(this);
			parent->mInternalWidgets.Add(this);
		}
	}

	void Widget::AddInternalWidget(Widget* widget, bool worldPositionStays /*= false*/)
	{
		widget->SetInternalParent(this, worldPositionStays);
	}

	Widget* Widget::GetInternalWidget(const String& path) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParentWidget;
				else
					return mParent->GetChildByType<Widget>(path.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mInternalWidgets)
		{
			if (child->mName == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChildByType<Widget>(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	Widget* Widget::FindInternalWidget(const String& name) const
	{
		for (auto widget : mInternalWidgets)
		{
			if (widget->GetName() == name)
				return widget;

			if (Widget* res = widget->FindChildByTypeAndName<Widget>(name))
				return res;
		}

		return nullptr;
	}

	void Widget::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateSelfTransform();

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		RectF childrenWorldRect = GetLayoutData().childrenWorldRect;
		GetLayoutData().childrenWorldRect = GetLayoutData().worldRectangle;

		for (auto child : mInternalWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		GetLayoutData().childrenWorldRect = childrenWorldRect;
	}

#if IS_EDITOR

	bool Widget::isEditorLayersVisible = true;
	bool Widget::isEditorInternalChildrenVisible = true;

	void Widget::SetEditableParent(SceneEditableObject* object)
	{
		if (auto inter = dynamic_cast<InternalChildrenEditableEditable*>(object))
			SetInternalParent(inter->widget);
		else
			Actor::SetEditableParent(object);
	}

	SceneEditableObject* Widget::GetEditableParent() const
	{
		if (mParentWidget && std::find(mParentWidget->mInternalWidgets.begin(),
									   mParentWidget->mInternalWidgets.end(), this) != mParentWidget->mInternalWidgets.end())
		{
			return &mParentWidget->internalChildrenEditable;
		}

		return Actor::GetEditableParent();
	}

	Vector<SceneEditableObject*> Widget::GetEditableChildren() const
	{
		Vector<SceneEditableObject*> res = Actor::GetEditableChildren();

		if (isEditorInternalChildrenVisible)
			res.Insert(const_cast<SceneEditableObject*>(dynamic_cast<const SceneEditableObject*>(&internalChildrenEditable)), 0);

		if (isEditorLayersVisible)
			res.Insert(const_cast<SceneEditableObject*>(dynamic_cast<const SceneEditableObject*>(&layersEditable)), 0);

		return res;
	}

	void Widget::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (auto actor = dynamic_cast<Actor*>(object))
		{
			if (idx < 0)
				AddChild(actor);
			else
				AddChild(actor, idx);
		}
		else if (auto layer = dynamic_cast<WidgetLayer*>(object))
			AddLayer(layer);
	}

	bool Widget::IsSupportsTransforming() const
	{
		return true;
	}

	Basis Widget::GetTransform() const
	{
		return layout->GetWorldBasis();
	}

	void Widget::SetTransform(const Basis& transform)
	{
		layout->SetWorldBasis(transform);
	}

	bool Widget::IsSupportsLayout() const
	{
		return true;
	}

	Layout Widget::GetLayout() const
	{
		return Layout(layout->GetAnchorMin(), layout->GetAnchorMax(), layout->GetOffsetMin(), layout->GetOffsetMax());
	}

	void Widget::SetLayout(const Layout& layout)
	{
		this->layout->SetAnchorMin(layout.anchorMin);
		this->layout->SetAnchorMax(layout.anchorMax);
		this->layout->SetOffsetMin(layout.offsetMin);
		this->layout->SetOffsetMax(layout.offsetMax);
	}

	SceneEditableObject* Widget::GetEditableOwner()
	{
		return this;
	}

	Widget::LayersEditable::LayersEditable()
	{
	}

	Widget::LayersEditable::LayersEditable(Widget* widget) :
		widget(widget)
	{
	}

	SceneUID Widget::LayersEditable::GetID() const
	{
		return UID;
	}

	void Widget::LayersEditable::GenerateNewID(bool childs /*= true*/)
	{
		UID = Math::Random();
	}

	const String& Widget::LayersEditable::GetName() const
	{
		static String name = "layers";
		return name;
	}

	void Widget::LayersEditable::SetName(const String& name)
	{
	}

	Vector<SceneEditableObject*> Widget::LayersEditable::GetEditableChildren() const
	{
		return widget->mLayers.Convert<SceneEditableObject*>([](WidgetLayer* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* Widget::LayersEditable::GetEditableParent() const
	{
		return dynamic_cast<SceneEditableObject*>(widget);
	}

	void Widget::LayersEditable::SetEditableParent(SceneEditableObject* object)
	{
	}

	void Widget::LayersEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (WidgetLayer* layer = dynamic_cast<WidgetLayer*>(object))
			widget->AddLayer(layer);
	}

	void Widget::LayersEditable::SetIndexInSiblings(int idx)
	{
	}

	bool Widget::LayersEditable::IsSupportsDeleting() const
	{
		return false;
	}

	Basis Widget::LayersEditable::GetTransform() const
	{
		return widget->GetTransform();
	}

	const SceneEditableObject* Widget::LayersEditable::GetEditableLink() const
	{
		return prototypeLink;
	}

	Widget::InternalChildrenEditableEditable::InternalChildrenEditableEditable()
	{
	}

	Widget::InternalChildrenEditableEditable::InternalChildrenEditableEditable(Widget* widget) :
		widget(widget)
	{
	}

	SceneUID Widget::InternalChildrenEditableEditable::GetID() const
	{
		return UID;
	}

	void Widget::InternalChildrenEditableEditable::GenerateNewID(bool childs /*= true*/)
	{
		UID = Math::Random();
	}

	const String& Widget::InternalChildrenEditableEditable::GetName() const
	{
		static String name = "internal children";
		return name;
	}

	void Widget::InternalChildrenEditableEditable::SetName(const String& name)
	{
	}

	Vector<SceneEditableObject*> Widget::InternalChildrenEditableEditable::GetEditableChildren() const
	{
		return widget->mInternalWidgets.Convert<SceneEditableObject*>([](Widget* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* Widget::InternalChildrenEditableEditable::GetEditableParent() const
	{
		return dynamic_cast<SceneEditableObject*>(widget);
	}

	void Widget::InternalChildrenEditableEditable::SetEditableParent(SceneEditableObject* object)
	{
	}

	void Widget::InternalChildrenEditableEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (Widget* widget = dynamic_cast<Widget*>(object))
			widget->SetInternalParent(widget);
	}

	void Widget::InternalChildrenEditableEditable::SetIndexInSiblings(int idx)
	{
	}

	bool Widget::InternalChildrenEditableEditable::IsSupportsDeleting() const
	{
		return false;
	}

	Basis Widget::InternalChildrenEditableEditable::GetTransform() const
	{
		return widget->GetTransform();
	}

	const SceneEditableObject* Widget::InternalChildrenEditableEditable::GetEditableLink() const
	{
		return prototypeLink;
	}

#endif // IS_EDITOR

}

DECLARE_CLASS(o2::Widget);

DECLARE_CLASS(o2::Widget::LayersEditable);

DECLARE_CLASS(o2::Widget::InternalChildrenEditableEditable);
