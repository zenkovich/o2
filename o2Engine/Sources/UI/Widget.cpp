#include "stdafx.h"
#include "Widget.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Scene/SceneLayer.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "UIManager.h"
#include "Button.h"

namespace o2
{
	UIWidget::UIWidget(ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(mnew UIWidgetLayout(), mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if (mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene)
			&& mLayer)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(mnew UIWidgetLayout(), prototype, mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;

		if (mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene)
			&& mLayer && !mOverrideDepth)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(ComponentsVec components, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(mnew UIWidgetLayout(), components, mode), layout(dynamic_cast<UIWidgetLayout*>(transform))
	{
		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		if ((mode == ActorCreateMode::InScene || mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene)
			&& mLayer)
		{
			mLayer->RegisterDrawable(this);
		}

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
	}

	UIWidget::UIWidget(const UIWidget& other):
		Actor(mnew UIWidgetLayout(*other.layout), other), layout(dynamic_cast<UIWidgetLayout*>(transform)),
		mTransparency(other.mTransparency), mVisible(other.mVisible), mFullyDisabled(!other.mVisible)
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
			newChild->mParent = this;
			newChild->mParentWidget = this;
			mInternalWidgets.Add(newChild);
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
		{
			mParent->OnChildRemoved(this);
		}

		for (auto layer : mLayers)
			delete layer;

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
		CopyData(other);
		return *this;
	}

	void UIWidget::Update(float dt)
	{
		if (!mFullyDisabled)
		{
			if (layout->mData->updateFrame == 0)
			{
				for (auto child : mChildren)
					child->transform->SetDirty(true);

				for (auto child : mInternalWidgets)
					child->transform->SetDirty(true);

				UpdateTransform(false);
			}

			if (!mIsClipped)
			{
				for (auto layer : mLayers)
					layer->Update(dt);

				for (auto state : mStates)
					state->Update(dt);
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

	void UIWidget::UpdateChildrenTransforms()
	{
		Actor::UpdateChildrenTransforms();

		for (auto child : mInternalWidgets)
			child->UpdateTransform(false);

		for (auto child : mInternalWidgets)
			child->UpdateChildrenTransforms();
	}

	void UIWidget::SetLayoutDirty()
	{
		layout->SetDirty();
	}

	void UIWidget::Draw()
	{
		DrawDebugFrame();

		if (mFullyDisabled || mIsClipped)
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

	const UIWidget::WidgetsVec& UIWidget::GetChildWidgets() const
	{
		return mChildWidgets;
	}

	UIWidgetLayer* UIWidget::AddLayer(UIWidgetLayer* layer)
	{
		mLayers.Add(layer);
		layer->SetOwnerWidget(this);
		UpdateLayersDrawingSequence();
		OnLayerAdded(layer);

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
		layer->drawable = drawable;
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

	bool UIWidget::RemoveLayer(UIWidgetLayer* layer)
	{
		bool res = mLayers.Remove(layer);
		delete layer;
		UpdateLayersDrawingSequence();

		return res;
	}

	bool UIWidget::RemoveLayer(const String& path)
	{
		auto layer = GetLayer(path);

		if (!layer)
			return false;

		if (layer->GetParent())
			return layer->GetParent()->RemoveChild(layer);

		bool res = mLayers.Remove(layer);
		UpdateLayersDrawingSequence();

		return res;
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
			mVisibleState->SetStateForcible(mVisible);

			mVisibleState->onStateBecomesTrue += [&]() {
				mFullyDisabled = false;
			};

			mVisibleState->onStateFullyFalse += [&]() {
				mFullyDisabled = true;
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

	void UIWidget::SetVisible(bool visible)
	{
		mVisible = visible;
		UpdateVisibility();
	}

	void UIWidget::SetVisibleForcible(bool visible)
	{
		if (mVisibleState)
			mVisibleState->SetStateForcible(visible);

		mVisible = visible;
		mFullyDisabled = !visible;

		UpdateVisibility();
	}

	void UIWidget::Show(bool forcible /*= false*/)
	{
		if (forcible)
			SetVisibleForcible(true);
		else
			SetVisible(true);
	}

	void UIWidget::Hide(bool forcible /*= false*/)
	{
		if (forcible)
			SetVisibleForcible(false);
		else
			SetVisible(false);
	}

	bool UIWidget::IsVisible() const
	{
		return mVisible;
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

	void UIWidget::SetPositionIndexInParent(int index)
	{
		Actor::SetPositionIndexInParent(index);

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
		if ((mFullyDisabled || mIsClipped) && layout->mData->dirtyFrame != o2Time.GetCurrentFrame())
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
	{
		bool lastResVisible = mResVisible;

		if (mParentWidget)
			mResVisible = mVisible && mParentWidget->mResVisible;
		else
			mResVisible = mVisible;

		mIsClipped = false;

		for (auto child : mChildWidgets)
			child->UpdateVisibility(false);

		for (auto child : mInternalWidgets)
			child->UpdateVisibility(false);

		if (mResVisible != lastResVisible)
		{
			if (mVisibleState)
				mVisibleState->SetState(mResVisible);
			else
				mFullyDisabled = !mResVisible;

			if (updateLayout)
				SetLayoutDirty();

			if (mResVisible)
				onShow();
			else
				onHide();

			OnVisibleChanged();
		}
	}

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
		if ((mFullyDisabled || mIsClipped) && layout->mData->dirtyFrame != o2Time.GetCurrentFrame())
			return;

		mBounds = layout->mData->worldRectangle;

		for (auto layer : mDrawingLayers)
			mBounds.Expand(layer->GetRect());

		bool anyEnabled = false;
		for (auto child : mChildWidgets)
		{
			if (!child->mFullyDisabled)
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
			if (layer->drawable)
			{
				if (layer->mDepth < topLayersDepth)
					mDrawingLayers.Add(layer);
				else
					mTopDrawingLayers.Add(layer);
			}

			auto childLayers = layer->GetAllChilds();
			for (auto childLayer : childLayers)
			{
				if (childLayer->drawable)
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
				ExcludeFromScene();
		}
		else if (mParent && mParent->IsOnScene()) IncludeInScene();
	}

	void UIWidget::OnChildAdded(Actor* child)
	{
		layout->SetDirty();

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

		SceneDrawable::OnExcludeFromScene();
	}

	void UIWidget::OnIncludeToScene()
	{
		SceneDrawable::OnIncludeToScene();
	}

	void UIWidget::OnDeserialized(const DataNode& node)
	{
		Actor::OnDeserialized(node);

		SceneDrawable::mLayer = Actor::mLayer;
		SceneDrawable::mIsOnScene = Actor::mIsOnScene;

		for (auto layer : mLayers)
			layer->mOwnerWidget = this;

		mChildWidgets.Clear();
		for (auto child : mChildren)
		{
			UIWidget* childWidget = dynamic_cast<UIWidget*>(child);
			if (childWidget)
				mChildWidgets.Add(childWidget);
		}

		for (auto child : mInternalWidgets)
		{
			child->mParent = this;
			child->mParentWidget = this;
		}

		RetargetStatesAnimations();
		SetVisibleForcible(mVisible);
		UpdateLayersDrawingSequence();
	}

	void UIWidget::ForceDraw(const RectF& area, float transparency)
	{
		Vec2F oldLayoutOffsetMin = layout->mData->offsetMin;
		Vec2F oldLayoutOffsetMax = layout->mData->offsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;
		auto oldParentWidget = mParentWidget;
		bool oldClipped= mIsClipped;
		bool oldDisabled = mFullyDisabled;

		layout->mData->offsetMin = area.LeftBottom();
		layout->mData->offsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mParentWidget = nullptr;
		mIsClipped = false;
		mFullyDisabled = false;

		UIWidget::UpdateTransform(true);
		UpdateTransparency();

		Draw();

		layout->mData->offsetMin = oldLayoutOffsetMin;
		layout->mData->offsetMax = oldLayoutOffsetMax;
		mTransparency = oldTransparency;
		mParent = oldParent;
		mParentWidget = oldParentWidget;
		mIsClipped = false;
		mFullyDisabled = oldDisabled;

		UIWidget::UpdateTransform(true);
		layout->mData->dirtyFrame = o2Time.GetCurrentFrame();
		UpdateBounds();
		UpdateBoundsWithChilds();
		UpdateTransparency();
	}

	void UIWidget::OnVisibleChanged()
	{}

	void UIWidget::CopyData(const Actor& otherActor)
	{
		const UIWidget& other = dynamic_cast<const UIWidget&>(otherActor);

		Actor::CopyData(other);

		for (auto layer : mLayers)
			delete layer;

		for (auto state : mStates)
			delete state;

		for (auto child : mInternalWidgets)
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
			newLayer->mOwnerWidget = this;
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
			newChild->mParent = this;
			newChild->mParentWidget = this;
			mInternalWidgets.Add(newChild);
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = dynamic_cast<UIWidgetState*>(state->Clone());
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();
	}

	void UIWidget::SetInternalParent(UIWidget* parent, bool worldPositionStays /*= true*/)
	{
		SetParent(parent, worldPositionStays);

		parent->mChildren.Remove(this);
		parent->mChildWidgets.Remove(this);
		parent->mInternalWidgets.Add(this);
	}

	void UIWidget::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		RectF last = mBoundsWithChilds;

		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateTransform(false);

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		for (auto child : mInternalWidgets)
			child->MoveAndCheckClipping(delta, clipArea);
	}

}

DECLARE_CLASS(o2::UIWidget);
