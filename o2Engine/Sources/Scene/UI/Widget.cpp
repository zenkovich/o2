#include "stdafx.h"
#include "Widget.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace o2
{
	UIWidget::UIWidget(ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew UIWidgetLayout(), mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if ((mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene))
			&& mLayer)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew UIWidgetLayout(), prototype, mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;

		if ((mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene))
			&& mLayer && !mOverrideDepth)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(ComponentsVec components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew UIWidgetLayout(), components, mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if ((mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene))
			&& mLayer)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(const UIWidget& other) :
		Actor(mnew UIWidgetLayout(*other.layout), other), layout(dynamic_cast<UIWidgetLayout*>(transform)),
		mTransparency(other.mTransparency), transparency(this), resTransparency(this), parentWidget(this),
		childrenWidgets(this), layers(this), states(this), childWidget(this), layer(this), state(this)
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;
		layout->SetOwner(this);

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew UIWidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			UIWidget* childWidget = dynamic_cast<UIWidget*>(child);
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
			auto newChild = child->CloneAs<UIWidget>();
			newChild->ExcludeFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = dynamic_cast<UIWidgetState*>(state->Clone());
			AddState(newState);
		}

		if (mLayer && mDefaultCreationMode == ActorCreateMode::InScene)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		UpdateDrawingChildren();
		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();
	}

	UIWidget::~UIWidget()
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

	UIWidget& UIWidget::operator=(const UIWidget& other)
	{
		Actor::operator=(other);
		return *this;
	}

	void UIWidget::Update(float dt)
	{
		if (mResEnabledInHierarchy)
		{
			if (layout->mData->updateFrame == 0)
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

	void UIWidget::UpdateChildren(float dt)
	{
		for (auto child : mChildren)
			child->Update(dt);

		for (auto child : mInternalWidgets)
			child->Update(dt);

		for (auto child : mChildren)
			child->UpdateChildren(dt);

		for (auto child : mInternalWidgets)
			child->UpdateChildren(dt);
	}

	void UIWidget::UpdateTransform()
	{
		if (layout->mData->drivenByParent && mParentWidget) {
			mParentWidget->UpdateTransform();
		}

		UpdateSelfTransform();
		UpdateChildrenTransforms();
	}

	void UIWidget::UpdateChildrenTransforms()
	{
		Actor::UpdateChildrenTransforms();

		for (auto child : mInternalWidgets)
			child->UpdateSelfTransform();

		for (auto child : mInternalWidgets)
			child->UpdateChildrenTransforms();
	}

	void UIWidget::SetLayoutDirty()
	{
		layout->SetDirty(false);
	}

	void UIWidget::Draw()
	{
		DrawDebugFrame();

		if (!mResEnabledInHierarchy || mIsClipped)
		{
			for (auto child : mDrawingChildren)
				child->Draw();

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

	void UIWidget::DrawDebugFrame()
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

	void UIWidget::OnTransformUpdated()
	{
		mIsClipped = false;
		Actor::OnTransformUpdated();
		UpdateLayersLayouts();
		onLayoutUpdated();
	}

	void UIWidget::OnFocused()
	{
		onFocused();
	}

	void UIWidget::OnUnfocused()
	{
		onUnfocused();
	}

	UIWidget* UIWidget::GetParentWidget() const
	{
		return mParentWidget;
	}

	RectF UIWidget::GetChildrenRect() const
	{
		return mChildrenWorldRect;
	}

	const UIWidget::WidgetsVec& UIWidget::GetChildWidgets() const
	{
		return mChildWidgets;
	}

	UIWidgetLayer* UIWidget::AddLayer(UIWidgetLayer* layer)
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

	UIWidgetLayer* UIWidget::AddLayer(const String& name, IRectDrawable* drawable,
									  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mDrawingLayers.Count();

		UIWidgetLayer* layer = mnew UIWidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->SetDrawable(drawable);
		layer->layout = layout;

		AddLayer(layer);

		return layer;
	}

	UIWidgetLayer* UIWidget::GetLayer(const String& path) const
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

	UIWidgetLayer* UIWidget::FindLayer(const String& name) const
	{
		for (auto childLayer : mLayers)
		{
			if (childLayer->name == name)
				return childLayer;

			UIWidgetLayer* layer = childLayer->FindChild(name);
			if (layer)
				return layer;
		}

		return nullptr;
	}

	void UIWidget::RemoveLayer(UIWidgetLayer* layer, bool release /*= true*/)
	{
		layer->mOwnerWidget = nullptr;

		mLayers.Remove(layer);

		if (release)
			delete layer;

		UpdateLayersDrawingSequence();
		OnChildrenChanged();
	}

	void UIWidget::RemoveLayer(const String& path)
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

	void UIWidget::RemoveAllLayers()
	{
		for (auto layer : mLayers)
			delete layer;

		mDrawingLayers.Clear();

		mLayers.Clear();
	}

	const LayersVec& UIWidget::GetLayers() const
	{
		return mLayers;
	}

	UIWidgetState* UIWidget::AddState(const String& name)
	{
		UIWidgetState* newState = mnew UIWidgetState();
		newState->name = name;
		newState->animation.SetTarget(this);

		return AddState(newState);
	}

	UIWidgetState* UIWidget::AddState(const String& name, const Animation& animation)
	{
		UIWidgetState* newState = mnew UIWidgetState();
		newState->name = name;
		newState->animation = animation;
		newState->animation.SetTarget(this);
		newState->animation.relTime = 0.0f;

		return AddState(newState);
	}

	UIWidgetState* UIWidget::AddState(UIWidgetState* state)
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

		OnStateAdded(state);

		return state;
	}

	bool UIWidget::RemoveState(const String& name)
	{
		int idx = mStates.FindIdx([&](UIWidgetState* state) { return state->name == name; });
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

	bool UIWidget::RemoveState(UIWidgetState* state)
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

	void UIWidget::RemoveAllStates()
	{
		for (auto state : mStates)
			delete state;

		mVisibleState = nullptr;
		mFocusedState = nullptr;

		mStates.Clear();
	}

	void UIWidget::SetState(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetState(state);
	}

	void UIWidget::SetStateForcible(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetStateForcible(state);
	}

	bool UIWidget::GetState(const String& name) const
	{
		auto state = GetStateObject(name);

		if (state)
			return state->GetState();

		return false;
	}

	UIWidgetState* UIWidget::GetStateObject(const String& name) const
	{
		return mStates.FindMatch([&](auto state) { return state->name == name; });
	}

	const UIWidget::StatesVec& UIWidget::GetStates() const
	{
		return mStates;
	}

	void UIWidget::SetDepthOverridden(bool overrideDepth)
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

	bool UIWidget::IsDepthOverriden() const
	{
		return mOverrideDepth;
	}

	void UIWidget::SetTransparency(float transparency)
	{
		mTransparency = transparency;
		UpdateTransparency();
	}

	float UIWidget::GetTransparency() const
	{
		return mTransparency;
	}

	float UIWidget::GetResTransparency() const
	{
		return mResTransparency;
	}

	void UIWidget::SetEnableForcible(bool visible)
	{
		if (mVisibleState)
			mVisibleState->SetStateForcible(visible);

		mEnabled = visible;
		UIWidget::UpdateResEnabled();
	}

	void UIWidget::Show(bool forcible /*= false*/)
	{
		if (forcible)
			SetEnableForcible(true);
		else
			SetEnabled(true);
	}

	void UIWidget::Hide(bool forcible /*= false*/)
	{
		if (forcible)
			SetEnableForcible(false);
		else
			SetEnabled(false);
	}

	void UIWidget::Focus()
	{
		o2UI.FocusWidget(this);
	}

	void UIWidget::Unfocus()
	{
		o2UI.FocusWidget(nullptr);
	}

	bool UIWidget::IsFocused() const
	{
		return mIsFocused;
	}

	bool UIWidget::IsFocusable() const
	{
		return mIsFocusable;
	}

	void UIWidget::SetFocusable(bool selectable)
	{
		mIsFocusable = selectable;
	}

	bool UIWidget::IsUnderPoint(const Vec2F& point)
	{
		return mDrawingScissorRect.IsInside(point) && layout->IsPointInside(point);
	}

	void UIWidget::SetIndexInSiblings(int index)
	{
		Actor::SetIndexInSiblings(index);

		if (mParentWidget)
		{
			mParentWidget->mChildWidgets.Clear();
			for (auto child : mParentWidget->mChildren)
			{
				UIWidget* widget = dynamic_cast<UIWidget*>(child);
				if (widget)
					mParentWidget->mChildWidgets.Add(widget);
			}

			mParentWidget->UpdateDrawingChildren();
		}
	}

	float UIWidget::GetMinWidthWithChildren() const
	{
		return layout->mData->minSize.x;
	}

	float UIWidget::GetMinHeightWithChildren() const
	{
		return layout->mData->minSize.y;
	}

	float UIWidget::GetWidthWeightWithChildren() const
	{
		return layout->mData->weight.x;
	}

	float UIWidget::GetHeightWeightWithChildren() const
	{
		return layout->mData->weight.y;
	}

	void UIWidget::UpdateBoundsWithChilds()
	{
		if ((!mResEnabledInHierarchy || mIsClipped) && layout->mData->dirtyFrame != o2Time.GetCurrentFrame())
			return;

		mBoundsWithChilds = mBounds;

		for (auto child : mChildWidgets)
			mBoundsWithChilds.Expand(child->mBoundsWithChilds);

		if (mParentWidget)
			mParentWidget->UpdateBoundsWithChilds();
	}

	void UIWidget::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		for (auto child : mChildWidgets)
			child->CheckClipping(clipArea);
	}

	void UIWidget::UpdateTransparency()
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

	void UIWidget::UpdateVisibility(bool updateLayout /*= true*/)
	{}

	void UIWidget::OnChildFocused(UIWidget* child)
	{
		if (mParentWidget)
			mParentWidget->OnChildFocused(child);
	}

	void UIWidget::RetargetStatesAnimations()
	{
		for (auto state : mStates)
		{
			state->animation.SetTarget(this, false);
			state->animation.relTime = state->GetState() ? 1.0f : 0.0f;
		}
	}

	void UIWidget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();

		UpdateBounds();
	}

	void UIWidget::UpdateDrawingChildren()
	{
		mDrawingChildren.Clear();

		for (auto child : mChildWidgets)
		{
			if (!child->mOverrideDepth)
				mDrawingChildren.Add(child);
		}
	}

	void UIWidget::UpdateBounds()
	{
		if ((!mResEnabledInHierarchy || mIsClipped) && layout->mData->dirtyFrame != o2Time.GetCurrentFrame())
			return;

		mBounds = layout->mData->worldRectangle;

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

	void UIWidget::UpdateLayersDrawingSequence()
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

	void UIWidget::SetParentWidget(UIWidget* widget)
	{
		SetParent(widget);
	}

	UIWidget* UIWidget::GetChildWidget(const String& path) const
	{
		Actor* actor = GetChild(path);
		return dynamic_cast<UIWidget*>(actor);
	}

	UIWidget* UIWidget::AddChildWidget(UIWidget* widget)
	{
		return dynamic_cast<UIWidget*>(AddChild(widget));
	}

	UIWidget* UIWidget::AddChildWidget(UIWidget* widget, int position)
	{
		return dynamic_cast<UIWidget*>(AddChild(widget, position));
	}

	UIWidget::WidgetsVec UIWidget::GetChildrenNonConst()
	{
		return mChildWidgets;
	}

	LayersVec UIWidget::GetLayersNonConst()
	{
		return mLayers;
	}

	StatesVec UIWidget::GetStatesNonConst()
	{
		return mStates;
	}

	Dictionary<String, UIWidgetLayer*> UIWidget::GetAllLayers()
	{
		Dictionary<String, UIWidgetLayer*> res;
		for (auto layer : mLayers)
			res.Add(layer->name, layer);

		return res;
	}

	Dictionary<String, UIWidget*> UIWidget::GetAllChilds()
	{
		Dictionary<String, UIWidget*> res;
		for (auto child : mChildWidgets)
			res.Add(child->GetName(), child);

		return res;
	}

	Dictionary<String, UIWidgetState*> UIWidget::GetAllStates()
	{
		Dictionary<String, UIWidgetState*> res;
		for (auto state : mStates)
			res.Add(state->name, state);

		return res;
	}

	void UIWidget::OnSerialize(DataNode& node) const
	{
		Actor::OnSerialize(node);
	}

	void UIWidget::OnLayerAdded(UIWidgetLayer* layer)
	{}

	void UIWidget::OnStateAdded(UIWidgetState* state)
	{}

	void UIWidget::OnParentChanged(Actor* oldParent)
	{
		layout->SetDirty();

		mParentWidget = dynamic_cast<UIWidget*>(mParent);

		if (mParentWidget)
		{
			if (mOverrideDepth)
				IncludeInScene();
			else
				ExcludeFromScene(true);
		}
		else if (mParent && mParent->IsOnScene()) IncludeInScene();
	}

	void UIWidget::OnChildAdded(Actor* child)
	{
		layout->SetDirty(false);

		UIWidget* widget = dynamic_cast<UIWidget*>(child);
		if (widget)
		{
			mChildWidgets.Add(widget);
			UpdateDrawingChildren();

			OnChildAdded(widget);
		}
	}

	void UIWidget::OnChildAdded(UIWidget* child)
	{}

	void UIWidget::OnChildRemoved(Actor* child)
	{
		layout->SetDirty();

		UIWidget* widget = dynamic_cast<UIWidget*>(child);
		if (widget)
		{
			mChildWidgets.Remove(widget);
			mInternalWidgets.Remove(widget);
			UpdateDrawingChildren();

			OnChildRemoved(widget);
		}
	}

	void UIWidget::OnChildRemoved(UIWidget* child)
	{}

	void UIWidget::OnLayerChanged(SceneLayer* oldLayer)
	{
		SceneDrawable::SetLayer(mLayer);
	}

	void UIWidget::OnExcludeFromScene()
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

	void UIWidget::OnIncludeToScene()
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

	void UIWidget::OnDeserialized(const DataNode& node)
	{
		Actor::OnDeserialized(node);

		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		for (auto layer : mLayers)
			layer->SetOwnerWidget(this);

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			UIWidget* childWidget = dynamic_cast<UIWidget*>(child);
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
		}

		RetargetStatesAnimations();
		SetEnableForcible(mEnabled);
		UpdateLayersDrawingSequence();
	}

	void UIWidget::ForceDraw(const RectF& area, float transparency)
	{
		Vec2F oldLayoutOffsetMin = layout->mData->offsetMin;
		Vec2F oldLayoutOffsetMax = layout->mData->offsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;
		auto oldParentWidget = mParentWidget;
		bool oldClipped = mIsClipped;
		bool oldResEnabledInHierarchy = mResEnabledInHierarchy;

		layout->mData->offsetMin = area.LeftBottom();
		layout->mData->offsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mParentWidget = nullptr;
		mIsClipped = false;
		mResEnabledInHierarchy = true;

		UpdateSelfTransform();
		UpdateChildrenTransforms();
		UpdateTransparency();

		Draw();

		layout->mData->offsetMin = oldLayoutOffsetMin;
		layout->mData->offsetMax = oldLayoutOffsetMax;
		mTransparency = oldTransparency;
		mParent = oldParent;
		mParentWidget = oldParentWidget;
		mIsClipped = false;
		mResEnabledInHierarchy = oldResEnabledInHierarchy;

		UpdateSelfTransform();
		UpdateChildrenTransforms();

		layout->mData->dirtyFrame = o2Time.GetCurrentFrame();

		UpdateBounds();
		UpdateBoundsWithChilds();
		UpdateTransparency();
	}

	void UIWidget::UpdateResEnabled()
	{
		if (mVisibleState)
			mVisibleState->SetState(mEnabled);
		else
			Actor::UpdateResEnabled();
	}

	void UIWidget::UpdateResEnabledInHierarchy()
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

			o2Scene.onEnableChanged(this);

			OnResEnableInHierarchyChanged();
			OnChanged();
		}

		for (auto comp : mComponents)
			comp->UpdateEnabled();

		for (auto child : mChildren)
			child->UpdateResEnabledInHierarchy();

		for (auto child : mInternalWidgets)
			child->UpdateResEnabledInHierarchy();
	}

	void UIWidget::CopyData(const Actor& otherActor)
	{
		const UIWidget& other = dynamic_cast<const UIWidget&>(otherActor);

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
			auto newLayer = mnew UIWidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			UIWidget* childWidget = dynamic_cast<UIWidget*>(child);
			if (childWidget)
				mChildWidgets.Add(childWidget);
		}

		for (auto child : other.mInternalWidgets)
		{
			auto newChild = child->CloneAs<UIWidget>();
			newChild->ExcludeFromScene();
			newChild->SetInternalParent(this, false);
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = dynamic_cast<UIWidgetState*>(state->Clone());
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();
	}

	void UIWidget::SetInternalParent(UIWidget* parent, bool worldPositionStays /*= false*/)
	{
		SetParent(parent, worldPositionStays);

		parent->mChildren.Remove(this);
		parent->mChildWidgets.Remove(this);
		parent->mDrawingChildren.Remove(this);
		parent->mInternalWidgets.Add(this);
	}

	void UIWidget::AddInternalWidget(UIWidget* widget, bool worldPositionStays /*= false*/)
	{
		widget->SetInternalParent(this, worldPositionStays);
	}

	UIWidget* UIWidget::GetInternalWidget(const String& path) const
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
					return mParent->GetChildByType<UIWidget>(path.SubStr(delPos + 1));
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
					return child->GetChildByType<UIWidget>(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	UIWidget* UIWidget::FindInternalWidget(const String& name) const
	{
		for (auto widget : mInternalWidgets)
		{
			if (widget->GetName() == name)
				return widget;

			if (UIWidget* res = widget->FindChildByTypeAndName<UIWidget>(name))
				return res;
		}

		return nullptr;
	}

	void UIWidget::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		RectF last = mBoundsWithChilds;

		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateSelfTransform();

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		for (auto child : mInternalWidgets)
			child->MoveAndCheckClipping(delta, clipArea);
	}

#if IS_EDITOR

	bool UIWidget::isEditorLayersVisible = true;
	bool UIWidget::isEditorInternalChildrenVisible = true;

	SceneEditableObject* UIWidget::GetEditableParent() const
	{
		if (mParentWidget && std::find(mParentWidget->mInternalWidgets.begin(),
									   mParentWidget->mInternalWidgets.end(), this) != mParentWidget->mInternalWidgets.end())
		{
			return &mParentWidget->internalChildrenEditable;
		}

		return Actor::GetEditableParent();
	}

	Vector<SceneEditableObject*> UIWidget::GetEditablesChildren() const
	{
		Vector<SceneEditableObject*> res = Actor::GetEditablesChildren();

		if (isEditorInternalChildrenVisible)
			res.Insert(const_cast<SceneEditableObject*>(dynamic_cast<const SceneEditableObject*>(&internalChildrenEditable)), 0);

		if (isEditorLayersVisible)
			res.Insert(const_cast<SceneEditableObject*>(dynamic_cast<const SceneEditableObject*>(&layersEditable)), 0);

		return res;
	}

	void UIWidget::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (auto actor = dynamic_cast<Actor*>(object))
		{
			if (idx < 0)
				AddChild(actor);
			else
				AddChild(actor, idx);
		}
		else if (auto layer = dynamic_cast<UIWidgetLayer*>(object))
			AddLayer(layer);
	}

	bool UIWidget::IsSupportsTransforming() const
	{
		return true;
	}

	Basis UIWidget::GetTransform() const
	{
		return layout->GetWorldBasis();
	}

	void UIWidget::SetTransform(const Basis& transform)
	{
		layout->SetWorldBasis(transform);
	}

	bool UIWidget::IsSupportsLayout() const
	{
		return true;
	}

	Layout UIWidget::GetLayout() const
	{
		return Layout(layout->GetAnchorMin(), layout->GetAnchorMax(), layout->GetOffsetMin(), layout->GetOffsetMax());
	}

	void UIWidget::SetLayout(const Layout& layout)
	{
		this->layout->SetAnchorMin(layout.anchorMin);
		this->layout->SetAnchorMax(layout.anchorMax);
		this->layout->SetOffsetMin(layout.offsetMin);
		this->layout->SetOffsetMax(layout.offsetMax);
	}

	SceneEditableObject* UIWidget::GetEditableOwner()
	{
		return this;
	}

	UIWidget::LayersEditable::LayersEditable()
	{}

	UIWidget::LayersEditable::LayersEditable(UIWidget* widget) :
		mWidget(widget)
	{}

	SceneUID UIWidget::LayersEditable::GetID() const
	{
		return mUID;
	}

	void UIWidget::LayersEditable::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();
	}

	String UIWidget::LayersEditable::GetName() const
	{
		return "layers";
	}

	void UIWidget::LayersEditable::SetName(const String& name)
	{}

	Vector<SceneEditableObject*> UIWidget::LayersEditable::GetEditablesChildren() const
	{
		return mWidget->mLayers.Select<SceneEditableObject*>([](UIWidgetLayer* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* UIWidget::LayersEditable::GetEditableParent() const
{
		return dynamic_cast<SceneEditableObject*>(mWidget);
	}

	void UIWidget::LayersEditable::SetEditableParent(SceneEditableObject* object)
	{}

	void UIWidget::LayersEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (UIWidgetLayer* layer = dynamic_cast<UIWidgetLayer*>(object))
			mWidget->AddLayer(layer);
	}

	void UIWidget::LayersEditable::SetIndexInSiblings(int idx)
	{}

	Basis UIWidget::LayersEditable::GetTransform() const
	{
		return mWidget->GetTransform();
	}

	UIWidget::InternalChildrenEditableEditable::InternalChildrenEditableEditable()
	{}

	UIWidget::InternalChildrenEditableEditable::InternalChildrenEditableEditable(UIWidget* widget) :
		mWidget(widget)
	{}

	SceneUID UIWidget::InternalChildrenEditableEditable::GetID() const
	{
		return mUID;
	}

	void UIWidget::InternalChildrenEditableEditable::GenerateNewID(bool childs /*= true*/)
	{
		mUID = Math::Random();
	}

	String UIWidget::InternalChildrenEditableEditable::GetName() const
	{
		return "internal children";
	}

	void UIWidget::InternalChildrenEditableEditable::SetName(const String& name)
	{}

	Vector<SceneEditableObject*> UIWidget::InternalChildrenEditableEditable::GetEditablesChildren() const
	{
		return mWidget->mInternalWidgets.Select<SceneEditableObject*>([](UIWidget* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	o2::SceneEditableObject* UIWidget::InternalChildrenEditableEditable::GetEditableParent() const
{
		return dynamic_cast<SceneEditableObject*>(mWidget);
	}

	void UIWidget::InternalChildrenEditableEditable::SetEditableParent(SceneEditableObject* object)
	{}

	void UIWidget::InternalChildrenEditableEditable::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (UIWidget* widget = dynamic_cast<UIWidget*>(object))
			widget->SetInternalParent(mWidget);
	}

	void UIWidget::InternalChildrenEditableEditable::SetIndexInSiblings(int idx)
	{}

	Basis UIWidget::InternalChildrenEditableEditable::GetTransform() const
	{
		return mWidget->GetTransform();
	}

#endif // IS_EDITOR
}

DECLARE_CLASS(o2::UIWidget);

DECLARE_CLASS(o2::UIWidget::LayersEditable);

DECLARE_CLASS(o2::UIWidget::InternalChildrenEditableEditable);
