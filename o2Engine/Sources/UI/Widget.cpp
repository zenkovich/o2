#include "Widget.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Scene/SceneLayer.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "UIManager.h"

namespace o2
{
	UIWidget::UIWidget(ActorCreateMode mode /*= ActorCreateMode::InScene*/):
		Actor(mnew UIWidgetLayout(), mode), layout((UIWidgetLayout*)transform)
	{
		if (mode == ActorCreateMode::InScene && mLayer)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
		InitializeProperties();
	}

	UIWidget::UIWidget(const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::InScene*/):
		Actor(mnew UIWidgetLayout(), prototype, mode), layout((UIWidgetLayout*)transform)
	{
		if (mode == ActorCreateMode::InScene && mLayer && !mOverrideDepth)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
		InitializeProperties();
	}

	UIWidget::UIWidget(ComponentsVec components, ActorCreateMode mode /*= ActorCreateMode::InScene*/):
		Actor(mnew UIWidgetLayout(), components, mode), layout((UIWidgetLayout*)transform)
	{
		if (mode == ActorCreateMode::InScene && mLayer)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		layout->SetOwner(this);
		InitializeProperties();
	}

	UIWidget::UIWidget(const UIWidget& other):
		Actor(mnew UIWidgetLayout(*other.layout), other), layout((UIWidgetLayout*)transform), 
		mTransparency(other.mTransparency), mVisible(other.mVisible), mFullyDisabled(!other.mVisible)
	{
		layout->SetOwner(this);

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew UIWidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = dynamic_cast<UIWidgetState*>(state->Clone());
			AddState(newState);
		}

		if (mLayer)
			mLayer->RegisterDrawable(this);

		if (IsFocusable() && UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Add(this);

		InitializeProperties();
		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();
	}

	UIWidget::~UIWidget()
	{
		for (auto layer : mLayers)
			delete layer;

		for (auto state : mStates)
			delete state;

		if (mLayer && mIsOnScene)
			mLayer->UnregisterDrawable(this);

		if (UIManager::IsSingletonInitialzed())
			o2UI.mFocusableWidgets.Remove(this);
	}

	UIWidget& UIWidget::operator=(const UIWidget& other)
	{
		Actor::operator=(other);

		for (auto layer : mLayers)
			delete layer;

		for (auto state : mStates)
			delete state;

		mLayers.Clear();
		mStates.Clear();
		mVisibleState = nullptr;
		mFocusedState = nullptr;

		mName = other.mName;
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

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = dynamic_cast<UIWidgetState*>(state->Clone());
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		RetargetStatesAnimations();

		return *this;
	}

	void UIWidget::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		Actor::Update(dt);

		for (auto layer : mLayers)
			layer->Update(dt);

		for (auto state : mStates)
			state->Update(dt);
	}

	void UIWidget::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

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

		o2Render.DrawRectFrame(layout->GetWorldRect(), Color4::SomeColor(colr++));
	}

	void UIWidget::OnTransformUpdated()
	{
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

	void UIWidget::UpdateLayout(bool withChildren /*= true*/)
	{
		layout->Update();

		if (withChildren)
			UpdateChildrenLayouts();
	}

	float UIWidget::GetMinWidthWithChildren() const
	{
		return layout->mData->minSize.x;
	}

	float UIWidget::GetMinHeightWithChildren() const
	{
		return layout->mData->minSize.y;
	}

	void UIWidget::UpdateChildrenLayouts()
	{
		for (auto child : mChildWidgets)
			child->UpdateLayout();
	}

	void UIWidget::UpdateBoundsWithChilds()
	{
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
	}

	void UIWidget::UpdateVisibility(bool updateLayout /*= true*/)
	{
		bool lastResVisible = mResVisible;

		if (mParentWidget)
			mResVisible = mVisible && mParentWidget->mResVisible;
		else
			mResVisible = mVisible;

		for (auto child : mChildWidgets)
			child->UpdateVisibility(false);

		if (mResVisible != lastResVisible)
		{
			if (mVisibleState)
				mVisibleState->SetState(mResVisible);
			else
				mFullyDisabled = !mResVisible;

			if (updateLayout)
				UpdateLayout();

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
		mBounds = layout->mData->worldRectangle;

		for (auto layer : mDrawingLayers)
			mBounds.Expand(layer->GetRect());

		if (mChildren.IsEmpty())
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
	{
	}

	void UIWidget::OnChildRemoved(Actor* child)
	{
		layout->SetDirty();

		UIWidget* widget = dynamic_cast<UIWidget*>(child);
		if (widget)
		{
			mChildWidgets.Remove(widget);
			UpdateDrawingChildren();

			OnChildRemoved(widget);
		}
	}

	void UIWidget::OnChildRemoved(UIWidget* child)
	{
	}

	void UIWidget::OnLayerChanged(SceneLayer* oldLayer)
	{
		SceneDrawable::SetLayer(mLayer);
	}

	void UIWidget::OnDeserialized(const DataNode& node)
	{
		for (auto layer : mLayers)
			layer->mOwnerWidget = this;

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
		bool oldClipped= mIsClipped;

		layout->mData->offsetMin = area.LeftBottom();
		layout->mData->offsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mIsClipped = false;

		UIWidget::UpdateLayout(true);
		UpdateTransparency();

		Draw();

		layout->mData->offsetMin = oldLayoutOffsetMin;
		layout->mData->offsetMax = oldLayoutOffsetMax;
		mTransparency = oldTransparency;
		mParent = oldParent;
		mIsClipped = oldClipped;

		UpdateLayout(true);
		UpdateTransparency();
	}

	void UIWidget::OnVisibleChanged()
	{}

	void UIWidget::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIWidget, visible, SetVisible, IsVisible);
		INITIALIZE_PROPERTY(UIWidget, transparency, SetTransparency, GetTransparency);
		INITIALIZE_GETTER(UIWidget, resTransparency, GetResTransparency);
		INITIALIZE_PROPERTY(UIWidget, parentWidget, SetParentWidget, GetParentWidget);
		INITIALIZE_GETTER(UIWidget, childrenWidgets, GetChildrenNonConst);
		INITIALIZE_GETTER(UIWidget, layers, GetLayersNonConst);
		INITIALIZE_GETTER(UIWidget, states, GetStatesNonConst);
		INITIALIZE_ACCESSOR(UIWidget, childWidget, GetChildWidget);
		INITIALIZE_ACCESSOR(UIWidget, layer, GetLayer);
		INITIALIZE_ACCESSOR(UIWidget, state, GetStateObject);

		layer.SetAllAccessFunc(this, &UIWidget::GetAllLayers);
		childWidget.SetAllAccessFunc(this, &UIWidget::GetAllChilds);
	}
}

CLASS_META(o2::UIWidget)
{
	BASE_CLASS(o2::Actor);
	BASE_CLASS(o2::SceneDrawable);

	PUBLIC_FIELD(layout);
	PUBLIC_FIELD(visible);
	PUBLIC_FIELD(transparency);
	PUBLIC_FIELD(resTransparency);
	PUBLIC_FIELD(parentWidget);
	PUBLIC_FIELD(childrenWidgets);
	PUBLIC_FIELD(layers);
	PUBLIC_FIELD(states);
	PUBLIC_FIELD(childWidget);
	PUBLIC_FIELD(layer);
	PUBLIC_FIELD(state);
	PUBLIC_FIELD(onLayoutUpdated);
	PUBLIC_FIELD(onFocused);
	PUBLIC_FIELD(onUnfocused);
	PUBLIC_FIELD(onShow);
	PUBLIC_FIELD(onHide);
	PROTECTED_FIELD();
	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLayers).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mStates).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParentWidget);
	PROTECTED_FIELD(mChildWidgets);
	PROTECTED_FIELD(mDrawingChildren);
	PROTECTED_FIELD(mChildrenWorldRect);
	PROTECTED_FIELD(mOverrideDepth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTransparency).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResTransparency);
	PROTECTED_FIELD(mDrawingLayers);
	PROTECTED_FIELD(mFocusedState);
	PROTECTED_FIELD(mIsFocused);
	PROTECTED_FIELD(mIsFocusable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVisibleState);
	PROTECTED_FIELD(mVisible).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResVisible);
	PROTECTED_FIELD(mFullyDisabled);
	PROTECTED_FIELD(mIsClipped);
	PROTECTED_FIELD(mBounds);
	PROTECTED_FIELD(mBoundsWithChilds);

	typedef Dictionary<String, UIWidgetLayer*> _tmp1;
	typedef Dictionary<String, UIWidget*> _tmp2;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateLayout, bool);
	PUBLIC_FUNCTION(void, UpdateChildrenLayouts);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ForceDraw, const RectF&, float);
	PUBLIC_FUNCTION(UIWidget*, GetParentWidget);
	PUBLIC_FUNCTION(UIWidget*, GetChildWidget, const String&);
	PUBLIC_FUNCTION(UIWidget*, AddChildWidget, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, AddChildWidget, UIWidget*, int);
	PUBLIC_FUNCTION(const WidgetsVec&, GetChildWidgets);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddLayer, UIWidgetLayer*);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(bool, RemoveLayer, UIWidgetLayer*);
	PUBLIC_FUNCTION(bool, RemoveLayer, const String&);
	PUBLIC_FUNCTION(void, RemoveAllLayers);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetLayer, const String&);
	PUBLIC_FUNCTION(const LayersVec&, GetLayers);
	PUBLIC_FUNCTION(UIWidgetState*, AddState, const String&);
	PUBLIC_FUNCTION(UIWidgetState*, AddState, const String&, const Animation&);
	PUBLIC_FUNCTION(UIWidgetState*, AddState, UIWidgetState*);
	PUBLIC_FUNCTION(bool, RemoveState, const String&);
	PUBLIC_FUNCTION(bool, RemoveState, UIWidgetState*);
	PUBLIC_FUNCTION(void, RemoveAllStates);
	PUBLIC_FUNCTION(void, SetState, const String&, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, const String&, bool);
	PUBLIC_FUNCTION(bool, GetState, const String&);
	PUBLIC_FUNCTION(UIWidgetState*, GetStateObject, const String&);
	PUBLIC_FUNCTION(const StatesVec&, GetStates);
	PUBLIC_FUNCTION(void, SetDepthOverridden, bool);
	PUBLIC_FUNCTION(bool, IsDepthOverriden);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(void, SetVisible, bool);
	PUBLIC_FUNCTION(void, SetVisibleForcible, bool);
	PUBLIC_FUNCTION(void, Show, bool);
	PUBLIC_FUNCTION(void, Hide, bool);
	PUBLIC_FUNCTION(bool, IsVisible);
	PUBLIC_FUNCTION(void, Focus);
	PUBLIC_FUNCTION(void, Unfocus);
	PUBLIC_FUNCTION(bool, IsFocused);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetFocusable, bool);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, DrawDebugFrame);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, OnChildAdded, Actor*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Actor*);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(float, GetMinWidthWithChildren);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateBounds);
	PROTECTED_FUNCTION(void, UpdateBoundsWithChilds);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateVisibility, bool);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, UpdateDrawingChildren);
	PROTECTED_FUNCTION(void, UpdateLayersDrawingSequence);
	PROTECTED_FUNCTION(void, OnChildFocused, UIWidget*);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, OnStateAdded, UIWidgetState*);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, RetargetStatesAnimations);
	PROTECTED_FUNCTION(void, SetParentWidget, UIWidget*);
	PROTECTED_FUNCTION(WidgetsVec, GetChildrenNonConst);
	PROTECTED_FUNCTION(LayersVec, GetLayersNonConst);
	PROTECTED_FUNCTION(StatesVec, GetStatesNonConst);
	PROTECTED_FUNCTION(_tmp1, GetAllLayers);
	PROTECTED_FUNCTION(_tmp2, GetAllChilds);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
