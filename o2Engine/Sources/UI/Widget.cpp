#include "Widget.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
	UIWidget::UIWidget()
	{
		layout.mOwner = this;
		UpdateLayout();
		InitializeProperties();
	}

	UIWidget::UIWidget(const UIWidget& other):
		mName(other.mName), layout(other.layout), mTransparency(other.mTransparency), mVisible(other.mVisible),
		mFullyDisabled(!other.mVisible)
	{
		layout.mOwner = this;
		InitializeProperties();

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew UIWidgetLayer(*layer);
			newLayer->SetOwnerWidget(this);
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		for (auto child : other.mChilds)
		{
			auto cloned = child->Clone();
			mChilds.Add(cloned);
			cloned->mParent = this;
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = state->Clone();
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		UpdateTransparency();
		UpdateVisibility();
		UpdateLayout();
		RetargetStatesAnimations();
	}

	UIWidget::~UIWidget()
	{
		if (mParent)
			mParent->RemoveChild(this, false);

		for (auto layer : mLayers)
			delete layer;

		for (auto state : mStates)
			delete state;

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			delete child;
		}
	}

	UIWidget& UIWidget::operator=(const UIWidget& other)
	{
		for (auto layer : mLayers)
			delete layer;

		for (auto state : mStates)
			delete state;

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			delete child;
		}

		mChilds.Clear();
		mLayers.Clear();
		mStates.Clear();
		mVisibleState = nullptr;
		mFocusedState = nullptr;

		mName = other.mName;
		layout.CopyFrom(other.layout);
		mTransparency = other.mTransparency;
		mIsFocusable = other.mIsFocusable;

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew UIWidgetLayer(*layer);
			newLayer->mOwnerWidget = this;
			mLayers.Add(newLayer);
			OnLayerAdded(newLayer);
		}

		for (auto child : other.mChilds)
		{
			auto cloned = child->Clone();
			mChilds.Add(cloned);
			cloned->mParent = this;
		}

		for (auto state : other.mStates)
		{
			UIWidgetState* newState = state->Clone();
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		UpdateTransparency();
		UpdateVisibility();
		UpdateLayout();
		RetargetStatesAnimations();

		return *this;
	}

	void UIWidget::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mLayers)
			layer->Update(dt);

		for (auto state : mStates)
			state->Update(dt);

		for (auto child : mChilds)
			child->Update(dt);
	}

	void UIWidget::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		OnDrawn();

		for (auto child : mChilds)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void UIWidget::DrawDebugFrame()
	{
		if (!UI_DEBUG && !o2Input.IsKeyDown(VK_F2))
			return;

		static int colr = 0;
		static int lastFrame = 0;

		if (lastFrame != o2Time.GetCurrentFrame())
			colr = 0;

		lastFrame = o2Time.GetCurrentFrame();

		o2Render.DrawRectFrame(layout.mAbsoluteRect, Color4::SomeColor(colr++));
	}

	void UIWidget::OnFocused()
	{
		onFocused();
	}

	void UIWidget::OnUnfocused()
	{
		onUnfocused();
	}

	void UIWidget::SetName(const String& name)
	{
		mName = name;
	}

	String UIWidget::GetName() const
	{
		return mName;
	}

	UIWidget* UIWidget::GetParent() const
	{
		return mParent;
	}

	void UIWidget::SetParent(UIWidget* parent)
	{
		if (parent && parent->mChilds.Contains(this))
			return;

		if (mParent)
		{
			mParent->RemoveChild(this, false);
			mParent->UpdateLayout();
		}

		mParent = parent;

		if (mParent)
		{
			mParent->mChilds.Add(this);
			mParent->UpdateLayout();
		}

		UpdateTransparency();
		UpdateVisibility();
	}

	UIWidget* UIWidget::AddChild(UIWidget* widget, bool updateNow /*= true*/)
	{
		if (mChilds.Contains(widget))
			return widget;

		if (widget->mParent)
			widget->mParent->RemoveChild(widget, false);

		mChilds.Add(widget);
		widget->mParent = this;

		if (updateNow)
		{
			UpdateLayout();
			UpdateTransparency();
			UpdateVisibility();
		}

		OnChildAdded(widget);

		return widget;
	}

	void UIWidget::AddChilds(const WidgetsVec& widgets)
	{
		for (auto widget : widgets)
		{
			if (mChilds.Contains(widget))
				continue;

			if (widget->mParent)
				widget->mParent->RemoveChild(widget, false);

			mChilds.Add(widget);
			widget->mParent = this;

			OnChildAdded(widget);
		}

		UpdateLayout();
		UpdateTransparency();
		UpdateVisibility();
	}

	UIWidget* UIWidget::AddChild(UIWidget* widget, int index)
	{
		if (mChilds.Contains(widget))
			return widget;

		if (widget->mParent)
			widget->mParent->RemoveChild(widget, false);

		mChilds.Insert(widget, index);
		widget->mParent = this;

		UpdateLayout();
		UpdateTransparency();
		UpdateVisibility();

		OnChildAdded(widget);

		return widget;
	}

	bool UIWidget::RemoveChild(const String& path)
	{
		auto child = GetChild(path);
		if (!child)
			return false;

		bool res = child->mParent->RemoveChild(child);
		child->mParent->UpdateLayout();
		child->UpdateTransparency();
		child->UpdateVisibility();

		return res;
	}

	bool UIWidget::RemoveChild(UIWidget* widget, bool release /*= true*/, bool updateLayout /*= true*/)
	{
		if (!mChilds.Contains(widget))
			return false;

		widget->mParent = nullptr;
		mChilds.Remove(widget);
		
		if (updateLayout)
			UpdateLayout();

		OnChildRemoved(widget);

		if (release)
			delete widget;
		else
		{
			widget->UpdateTransparency();
			widget->UpdateVisibility();
		}

		return true;
	}

	UIWidget* UIWidget::GetChild(const String& path)
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

		for (auto child : mChilds)
		{
			if (child->mName == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	void UIWidget::RemoveAllChilds(bool release /*= true*/, bool updateLayout /*= true*/)
	{
		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			OnChildRemoved(child);

			if (release)
				delete child;
		}

		mChilds.Clear();
		
		if (updateLayout)
			UpdateLayout();
	}

	const UIWidget::WidgetsVec& UIWidget::GetChilds() const
	{
		return mChilds;
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

	const StatesVec& UIWidget::GetStates() const
	{
		return mStates;
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
		return mDrawingScissorRect.IsInside(point) && layout.mAbsoluteRect.IsInside(point);
	}

	bool UIWidget::CheckIsLayoutDrivenByParent(bool forcibleLayout)
	{
		if (layout.mDrivenByParent && !forcibleLayout)
		{
			if (mParent)
				mParent->UpdateLayout();

			return true;
		}

		return false;
	}

	void UIWidget::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (mFullyDisabled && !forcible)
			return;

		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			UpdateChildrenLayouts(forcible);
	}

	float UIWidget::GetLayoutWidth() const
	{
		return layout.mAbsoluteRect.Width();
	}

	float UIWidget::GetLayoutHeight() const
	{
		return layout.mAbsoluteRect.Height();
	}

	void UIWidget::UpdateChildrenLayouts(bool forcible /*= false*/)
	{
		for (auto child : mChilds)
			child->UpdateLayout(forcible);
	}

	void UIWidget::UpdateBoundsWithChilds()
	{
		mBoundsWithChilds = mBounds;

		for (auto child : mChilds)
			mBoundsWithChilds.Expand(child->mBoundsWithChilds);

		if (mParent)
			mParent->UpdateBoundsWithChilds();
	}

	void UIWidget::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		for (auto child : mChilds)
			child->CheckClipping(clipArea);
	}

	void UIWidget::UpdateTransparency()
	{
		if (mParent)
			mResTransparency = mTransparency*mParent->mResTransparency;
		else
			mResTransparency = mTransparency;

		for (auto layer : mLayers)
			layer->UpdateResTransparency();

		for (auto child : mChilds)
			child->UpdateTransparency();
	}

	void UIWidget::UpdateVisibility(bool updateLayout /*= true*/)
	{
		bool lastResVisible = mResVisible;

		if (mParent)
			mResVisible = mVisible && mParent->mResVisible;
		else
			mResVisible = mVisible;

		for (auto child : mChilds)
			child->UpdateVisibility(false);

		if (mResVisible != lastResVisible)
		{
			if (mVisibleState)
				mVisibleState->SetState(mResVisible);
			else
				mFullyDisabled = !mResVisible;

			if (updateLayout)
				UpdateLayout();

			OnVisibleChanged();
		}
	}

	void UIWidget::OnChildFocused(UIWidget* child)
	{
		if (mParent)
			mParent->OnChildFocused(child);
	}

	void UIWidget::RetargetStatesAnimations()
	{
		for (auto state : mStates)
		{
			state->animation.SetTarget(this, false);
			state->animation.relTime = state->GetState() ? 1.0f : 0.0f;
		}
	}

	void UIWidget::RecalculateAbsRect()
	{
// 		if (UI_DEBUG || o2Input.IsKeyDown(VK_F2))
// 		{
// 			static int frame = 0;
// 			static int counter = 0;
// 			int curFrame = o2Time.GetCurrentFrame();
// 
// 			printf("-- %s\n", mName.Data());
// 			if (curFrame != frame)
// 			{
// 				printf("----------------%i widget layouts recalculates\n", counter);
// 				counter = 0;
// 				frame = curFrame;
// 			}
// 
// 			counter++;
// 		}

		RectF lastAbsRect = layout.mAbsoluteRect;

		Vec2F parentSize, parentPos;
		if (mParent)
		{
			parentSize = mParent->mChildsAbsRect.Size();
			parentPos = mParent->mChildsAbsRect.LeftBottom();
		}

		layout.mLocalRect.left   = parentSize.x*layout.mAnchorMin.x + layout.mOffsetMin.x;
		layout.mLocalRect.right  = parentSize.x*layout.mAnchorMax.x + layout.mOffsetMax.x;
		layout.mLocalRect.bottom = parentSize.y*layout.mAnchorMin.y + layout.mOffsetMin.y;
		layout.mLocalRect.top    = parentSize.y*layout.mAnchorMax.y + layout.mOffsetMax.y;

		layout.mCheckMinMaxFunc();

		layout.mLocalRect.left   = Math::Floor(layout.mLocalRect.left);
		layout.mLocalRect.right  = Math::Floor(layout.mLocalRect.right);
		layout.mLocalRect.bottom = Math::Floor(layout.mLocalRect.bottom);
		layout.mLocalRect.top    = Math::Floor(layout.mLocalRect.top);

		layout.mAbsoluteRect = layout.mLocalRect + parentPos;
		mLastChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (lastAbsRect != layout.mAbsoluteRect)
			onLayoutChanged();
	}

	void UIWidget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();

		UpdateBounds();
	}

	void UIWidget::UpdateBounds()
	{
		mBounds = layout.mAbsoluteRect;

		for (auto layer : mDrawingLayers)
			mBounds.Expand(layer->GetRect());

		if (mChilds.IsEmpty())
			UpdateBoundsWithChilds();
	}

	void UIWidget::UpdateLayersDrawingSequence()
	{
		mDrawingLayers.Clear();
		mTopDrawingLayers.Clear();

		for (auto layer : mLayers)
		{
			if (layer->drawable)
			{
				if (layer->mDepth < 1000.0f)
					mDrawingLayers.Add(layer);
				else
					mTopDrawingLayers.Add(layer);
			}

			auto childLayers = layer->GetAllChilds();
			for (auto childLayer : childLayers)
			{
				if (childLayer->drawable)
				{
					if (childLayer->mDepth < 1000.0f)
						mDrawingLayers.Add(childLayer);
					else
						mTopDrawingLayers.Add(childLayer);
				}
			}
		}

		mDrawingLayers.Sort([](auto a, auto b) { return a->mDepth < b->mDepth; });
		mTopDrawingLayers.Sort([](auto a, auto b) { return a->mDepth < b->mDepth; });
	}

	UIWidget::WidgetsVec UIWidget::GetChildsNonConst()
	{
		return mChilds;
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
		for (auto child : mChilds)
			res.Add(child->GetName(), child);

		return res;
	}

	void UIWidget::OnLayerAdded(UIWidgetLayer* layer)
	{}

	void UIWidget::OnStateAdded(UIWidgetState* state)
	{}

	void UIWidget::OnChildAdded(UIWidget* child)
	{}

	void UIWidget::OnChildRemoved(UIWidget* child)
	{}

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
		Vec2F oldLayoutOffsetMin = layout.mOffsetMin;
		Vec2F oldLayoutOffsetMax = layout.mOffsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;
		bool oldClipped= mIsClipped;

		layout.mOffsetMin = area.LeftBottom();
		layout.mOffsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;
		mIsClipped = false;

		UIWidget::UpdateLayout(true);
		UpdateTransparency();

		Draw();

		layout.mOffsetMin = oldLayoutOffsetMin;
		layout.mOffsetMax = oldLayoutOffsetMax;
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
		INITIALIZE_PROPERTY(UIWidget, name, SetName, GetName);
		INITIALIZE_PROPERTY(UIWidget, parent, SetParent, GetParent);
		INITIALIZE_PROPERTY(UIWidget, transparency, SetTransparency, GetTransparency);
		INITIALIZE_GETTER(UIWidget, resTransparency, GetResTransparency);
		INITIALIZE_PROPERTY(UIWidget, visible, SetVisible, IsVisible);
		INITIALIZE_GETTER(UIWidget, childs, GetChildsNonConst);
		INITIALIZE_GETTER(UIWidget, layers, GetLayersNonConst);
		INITIALIZE_GETTER(UIWidget, states, GetStatesNonConst);
		INITIALIZE_ACCESSOR(UIWidget, child, GetChild);
		INITIALIZE_ACCESSOR(UIWidget, layer, GetLayer);
		INITIALIZE_ACCESSOR(UIWidget, state, GetStateObject);

		layer.SetAllAccessFunc(this, &UIWidget::GetAllLayers);
		child.SetAllAccessFunc(this, &UIWidget::GetAllChilds);
	}
}
 
CLASS_META(o2::UIWidget)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::IDrawable);

	PUBLIC_FIELD(name);
	PUBLIC_FIELD(parent);
	PUBLIC_FIELD(childs);
	PUBLIC_FIELD(layers);
	PUBLIC_FIELD(states);
	PUBLIC_FIELD(transparency);
	PUBLIC_FIELD(resTransparency);
	PUBLIC_FIELD(visible);
	PUBLIC_FIELD(child);
	PUBLIC_FIELD(layer);
	PUBLIC_FIELD(state);
	PUBLIC_FIELD(layout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onLayoutChanged);
	PUBLIC_FIELD(onFocused);
	PUBLIC_FIELD(onUnfocused);
	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLayers).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mStates).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParent);
	PROTECTED_FIELD(mChilds).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mChildsAbsRect);
	PROTECTED_FIELD(mLastChildsAbsRect);
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
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ForceDraw, const RectF&, float);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(UIWidget*, GetParent);
	PUBLIC_FUNCTION(void, SetParent, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, AddChild, UIWidget*, bool);
	PUBLIC_FUNCTION(void, AddChilds, const WidgetsVec&);
	PUBLIC_FUNCTION(UIWidget*, AddChild, UIWidget*, int);
	PUBLIC_FUNCTION(bool, RemoveChild, const String&);
	PUBLIC_FUNCTION(bool, RemoveChild, UIWidget*, bool, bool);
	PUBLIC_FUNCTION(UIWidget*, GetChild, const String&);
	PUBLIC_FUNCTION(void, RemoveAllChilds, bool, bool);
	PUBLIC_FUNCTION(const WidgetsVec&, GetChilds);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddLayer, UIWidgetLayer*);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetLayer, const String&);
	PUBLIC_FUNCTION(bool, RemoveLayer, UIWidgetLayer*);
	PUBLIC_FUNCTION(bool, RemoveLayer, const String&);
	PUBLIC_FUNCTION(void, RemoveAllLayers);
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
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, DrawDebugFrame);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(bool, CheckIsLayoutDrivenByParent, bool);
	PROTECTED_FUNCTION(float, GetLayoutWidth);
	PROTECTED_FUNCTION(float, GetLayoutHeight);
	PROTECTED_FUNCTION(void, UpdateChildrenLayouts, bool);
	PROTECTED_FUNCTION(void, UpdateBounds);
	PROTECTED_FUNCTION(void, UpdateBoundsWithChilds);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateVisibility, bool);
	PROTECTED_FUNCTION(void, OnChildFocused, UIWidget*);
	PROTECTED_FUNCTION(void, RetargetStatesAnimations);
	PROTECTED_FUNCTION(void, RecalculateAbsRect);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, UpdateLayersDrawingSequence);
	PROTECTED_FUNCTION(WidgetsVec, GetChildsNonConst);
	PROTECTED_FUNCTION(LayersVec, GetLayersNonConst);
	PROTECTED_FUNCTION(StatesVec, GetStatesNonConst);
	PROTECTED_FUNCTION(_tmp1, GetAllLayers);
	PROTECTED_FUNCTION(_tmp2, GetAllChilds);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, OnStateAdded, UIWidgetState*);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 