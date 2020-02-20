#include "o2/stdafx.h"
#include "Widget.h"

#include "o2/Application/Input.h"
#include "o2/Render/Render.h"
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
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if (mIsOnScene && mLayer)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew WidgetLayout(), prototype, mode), layout(dynamic_cast<WidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;

		if (mIsOnScene && mLayer && !mOverrideDepth)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew WidgetLayout(), components, mode), layout(dynamic_cast<WidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if (mIsOnScene && mLayer)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	Widget::Widget(const Widget& other) :
		Actor(mnew WidgetLayout(*other.layout), other), layout(dynamic_cast<WidgetLayout*>(transform)),
		mTransparency(other.mTransparency), transparency(this), resTransparency(this), 
		childrenWidgets(this), layers(this), states(this), childWidget(this), layer(this), state(this)
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;
		layout->SetOwner(this);

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
				childWidget->mParentWidget = this;
				mChildWidgets.Add(childWidget);

				if (childWidget->mOverrideDepth)
					childWidget->IncludeInScene();
				else
					childWidget->ExcludeFromScene();
			}
		}

		for (auto child : other.mInternalWidgets)
		{
			auto newChild = child->CloneAs<Widget>();
			newChild->ExcludeFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			WidgetState* newState = dynamic_cast<WidgetState*>(state->Clone());
			AddState(newState, false);
		}

		if (mLayer && mDefaultCreationMode == ActorCreateMode::InScene)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		UpdateDrawingChildren();
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

		if (mLayer && mIsOnScene)
			mLayer->UnregisterDrawable(this);

		if (UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Remove(this);
	}

	Widget& Widget::operator=(const Widget& other)
	{
		Actor::operator=(other);
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
				for (auto layer : mLayers)
					layer->Update(dt);

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
		if (GetLayoutData().drivenByParent && mParentWidget) {
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
				for (auto child : mDrawingChildren)
					child->Draw();
			}

			return;
		}

		for (auto layer : mDrawingLayers)
			layer->Draw();

		OnDrawn();

		for (auto child : mDrawingChildren)
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

	void Widget::OnTransformUpdated()
	{
		mIsClipped = false;
		Actor::OnTransformUpdated();
		UpdateLayersLayouts();
		onLayoutUpdated();
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

	WidgetLayer* Widget::AddLayer(WidgetLayer* layer)
	{
		if (layer->mParent)
			layer->mParent->RemoveChild(layer, false);

		mLayers.Add(layer);
		layer->SetOwnerWidget(this);
		UpdateLayersDrawingSequence();
		OnLayerAdded(layer);

#if IS_EDITOR
		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene()) {
			o2Scene.OnObjectChanged(&layersEditable);
			o2Scene.onChildrenHierarchyChanged(&layersEditable);
		}
#endif

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
		WString pathPart = path.SubStr(0, delPos);

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
		layer->mOwnerWidget = nullptr;

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
		newState->animation.SetTarget(this);

		return AddState(newState);
	}

	WidgetState* Widget::AddState(const String& name, const Animation& animation)
	{
		WidgetState* newState = mnew WidgetState();
		newState->name = name;
		newState->animation = animation;
		newState->animation.SetTarget(this);
		newState->animation.relTime = 0.0f;

		return AddState(newState);
	}

	WidgetState* Widget::AddState(WidgetState* state, bool showAnimErrors /*= true*/)
	{
		mStates.Add(state);

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

		state->mOwner = this;
		state->animation.SetTarget(this, showAnimErrors);

		OnStateAdded(state);

		return state;
	}

	bool Widget::RemoveState(const String& name)
	{
		int idx = mStates.FindIdx([&](WidgetState* state) { return state->name == name; });
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
		int idx = mStates.Find(state);
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
		return mStates.FindMatch([&](auto state) { return state->name == name; });
	}

	const Vector<WidgetState*>& Widget::GetStates() const
	{
		return mStates;
	}

	void Widget::SetDepthOverridden(bool overrideDepth)
	{
		if (mOverrideDepth == overrideDepth)
			return;

		mOverrideDepth = overrideDepth;

		if (mLayer)
		{
			if (mOverrideDepth)
			{
				mLayer->RegisterDrawable(this);

				if (mParentWidget)
					mParentWidget->mDrawingChildren.Remove(this);
			}
			else
			{
				mLayer->UnregisterDrawable(this);

				if (mParentWidget)
					mParentWidget->mDrawingChildren.Add(this);
			}
		}
	}

	bool Widget::IsDepthOverriden() const
	{
		return mOverrideDepth;
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

	void Widget::SetEnableForcible(bool visible)
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
			SetEnableForcible(true);
		else
			SetEnabled(true);
	}

	void Widget::Hide(bool forcible /*= false*/)
	{
		if (forcible)
			SetEnableForcible(false);
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
		{
			mParentWidget->UpdateChildWidgetsList();
			mParentWidget->UpdateDrawingChildren();
		}
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
			mResTransparency = mTransparency*mParentWidget->mResTransparency;
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
	{}

	void Widget::OnChildFocused(Widget* child)
	{
		if (mParentWidget)
			mParentWidget->OnChildFocused(child);
	}

	void Widget::RetargetStatesAnimations()
	{
		for (auto state : mStates)
		{
			state->animation.SetTarget(this, false);
			state->animation.relTime = state->GetState() ? 1.0f : 0.0f;
		}
	}

	void Widget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();

		UpdateBounds();
	}

	void Widget::UpdateDrawingChildren()
	{
		mDrawingChildren.Clear();

		for (auto child : mChildWidgets)
		{
			if (!child->mOverrideDepth)
				mDrawingChildren.Add(child);
		}
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

	Map<String, WidgetState*> Widget::GetAllStates()
	{
		Map<String, WidgetState*> res;
		for (auto state : mStates)
			res.Add(state->name, state);

		return res;
	}

	void Widget::OnSerialize(DataNode& node) const
	{
		Actor::OnSerialize(node);
	}

	void Widget::OnLayerAdded(WidgetLayer* layer)
	{}

	void Widget::OnStateAdded(WidgetState* state)
	{}

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

		if (mParentWidget)
		{
			if (mOverrideDepth)
				IncludeInScene();
			else
				ExcludeFromScene(true);
		}
		else if (mParent && mParent->IsOnScene()) IncludeInScene();
	}

	void Widget::OnChildAdded(Actor* child)
	{
		layout->SetDirty(false);

		Widget* widget = dynamic_cast<Widget*>(child);
		if (widget)
		{
			UpdateChildWidgetsList();
			UpdateDrawingChildren();

			OnChildAdded(widget);
		}
	}

	void Widget::OnChildAdded(Widget* child)
	{}

	void Widget::OnChildRemoved(Actor* child)
	{
		layout->SetDirty();

		Widget* widget = dynamic_cast<Widget*>(child);
		if (widget)
		{
			mChildWidgets.Remove(widget);
			mInternalWidgets.Remove(widget);
			UpdateDrawingChildren();

			OnChildRemoved(widget);
		}
	}

	void Widget::OnChildRemoved(Widget* child)
	{}

	void Widget::OnLayerChanged(SceneLayer* oldLayer)
	{
		SceneDrawable::SetLayer(mLayer);
	}

	void Widget::OnExcludeFromScene()
	{
		for (auto child : mInternalWidgets)
			child->ExcludeFromScene();

		for (auto layer : mLayers)
			layer->OnExcludeFromScene();

		SceneDrawable::OnExcludeFromScene();

#if IS_EDITOR
		o2Scene.mEditableObjects.Remove(&layersEditable);
		o2Scene.mEditableObjects.Remove(&internalChildrenEditable);
#endif
	}

	void Widget::OnIncludeToScene()
	{
		SceneDrawable::OnIncludeToScene();

		for (auto child : mInternalWidgets)
			child->IncludeInScene();

		for (auto layer : mLayers)
			layer->OnIncludeInScene();

#if IS_EDITOR
		o2Scene.mEditableObjects.Add(&layersEditable);
		o2Scene.mEditableObjects.Add(&internalChildrenEditable);
#endif
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

	void Widget::OnDeserialized(const DataNode& node)
	{
		Actor::OnDeserialized(node);

		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		for (auto layer : mLayers)
			layer->SetOwnerWidget(this);

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			Widget* childWidget = dynamic_cast<Widget*>(child);
			if (childWidget)
			{
				childWidget->mParentWidget = this;
				mChildWidgets.Add(childWidget);
			}
		}

		for (auto child : mInternalWidgets)
		{
			child->mParent = this;
			child->mParentWidget = this;

			child->ExcludeFromScene();
		}

		RetargetStatesAnimations();
		SetEnableForcible(mEnabled);
		UpdateLayersDrawingSequence();
	}

	void Widget::ForceDraw(const RectF& area, float transparency)
	{
		Vec2F oldLayoutOffsetMin = GetLayoutData().offsetMin;
		Vec2F oldLayoutOffsetMax = GetLayoutData().offsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;
		auto oldParentWidget = mParentWidget;
		bool oldClipped = mIsClipped;
		bool oldResEnabledInHierarchy = mResEnabledInHierarchy;

		GetLayoutData().offsetMin = area.LeftBottom();
		GetLayoutData().offsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mParentWidget = nullptr;
		mIsClipped = false;
		mResEnabledInHierarchy = true;

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
		mResEnabledInHierarchy = oldResEnabledInHierarchy;

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
				mLayer->mEnabledActors.Add(this);
			}
			else
			{
				onHide();
				mLayer->mEnabledActors.Remove(this);
			}

			layout->SetDirty(true);

#if IS_EDITOR
			if (IsHieararchyOnScene())
				o2Scene.onEnableChanged(this);
#endif

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

	void Widget::CopyData(const Actor& otherActor)
	{
		const Widget& other = dynamic_cast<const Widget&>(otherActor);

		Actor::CopyData(other);

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

		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

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
				mChildWidgets.Add(childWidget);
		}

		for (auto child : other.mInternalWidgets)
		{
			auto newChild = child->CloneAs<Widget>();
			newChild->ExcludeFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			WidgetState* newState = dynamic_cast<WidgetState*>(state->Clone());
			AddState(newState, false);
		}

		UpdateLayersDrawingSequence();
	}

	void Widget::SetInternalParent(Widget* parent, bool worldPositionStays /*= false*/)
	{
		SetParent(parent, worldPositionStays);

		parent->mChildren.Remove(this);
		parent->mChildWidgets.Remove(this);
		parent->mDrawingChildren.Remove(this);
		parent->mInternalWidgets.Add(this);
	}

	void Widget::AddInternalWidget(Widget* widget, bool worldPositionStays /*= false*/)
	{
		widget->SetInternalParent(this, worldPositionStays);
	}

	Widget* Widget::GetInternalWidget(const String& path) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

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

	SceneEditableObject* Widget::GetEditableParent() const
	{
		if (mParentWidget && std::find(mParentWidget->mInternalWidgets.begin(),
									   mParentWidget->mInternalWidgets.end(), this) != mParentWidget->mInternalWidgets.end())
		{
			return &mParentWidget->internalChildrenEditable;
		}

		return Actor::GetEditableParent();
	}

	Vector<SceneEditableObject*> Widget::GetEditablesChildren() const
	{
		Vector<SceneEditableObject*> res = Actor::GetEditablesChildren();

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
	{}

	Widget::LayersEditable::LayersEditable(Widget* widget) :
		mWidget(widget)
	{}

	SceneUID Widget::LayersEditable::GetID() const
	{
		return mUID;
	}

	void Widget::LayersEditable::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();
	}

	String Widget::LayersEditable::GetName() const
	{
		return "layers";
	}

	void Widget::LayersEditable::SetName(const String& name)
	{}

	Vector<SceneEditableObject*> Widget::LayersEditable::GetEditablesChildren() const
	{
		return mWidget->mLayers.Select<SceneEditableObject*>([](WidgetLayer* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* Widget::LayersEditable::GetEditableParent() const
{
		return dynamic_cast<SceneEditableObject*>(mWidget);
	}

	void Widget::LayersEditable::SetEditableParent(SceneEditableObject* object)
	{}

	void Widget::LayersEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (WidgetLayer* layer = dynamic_cast<WidgetLayer*>(object))
			mWidget->AddLayer(layer);
	}

	void Widget::LayersEditable::SetIndexInSiblings(int idx)
	{}

	bool Widget::LayersEditable::IsSupportsDeleting() const
	{
		return false;
	}

	Basis Widget::LayersEditable::GetTransform() const
	{
		return mWidget->GetTransform();
	}

	Widget::InternalChildrenEditableEditable::InternalChildrenEditableEditable()
	{}

	Widget::InternalChildrenEditableEditable::InternalChildrenEditableEditable(Widget* widget) :
		mWidget(widget)
	{}

	SceneUID Widget::InternalChildrenEditableEditable::GetID() const
	{
		return mUID;
	}

	void Widget::InternalChildrenEditableEditable::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();
	}

	String Widget::InternalChildrenEditableEditable::GetName() const
	{
		return "internal children";
	}

	void Widget::InternalChildrenEditableEditable::SetName(const String& name)
	{}

	Vector<SceneEditableObject*> Widget::InternalChildrenEditableEditable::GetEditablesChildren() const
	{
		return mWidget->mInternalWidgets.Select<SceneEditableObject*>([](Widget* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* Widget::InternalChildrenEditableEditable::GetEditableParent() const
{
		return dynamic_cast<SceneEditableObject*>(mWidget);
	}

	void Widget::InternalChildrenEditableEditable::SetEditableParent(SceneEditableObject* object)
	{}

	void Widget::InternalChildrenEditableEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (Widget* widget = dynamic_cast<Widget*>(object))
			widget->SetInternalParent(mWidget);
	}

	void Widget::InternalChildrenEditableEditable::SetIndexInSiblings(int idx)
	{}

	bool Widget::InternalChildrenEditableEditable::IsSupportsDeleting() const
	{
		return false;
	}

	Basis Widget::InternalChildrenEditableEditable::GetTransform() const
	{
		return mWidget->GetTransform();
	}

#endif // IS_EDITOR
}

DECLARE_CLASS(o2::Widget);

DECLARE_CLASS(o2::Widget::LayersEditable);

DECLARE_CLASS(o2::Widget::InternalChildrenEditableEditable);
