#include "Widget.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
	UIWidget::UIWidget():
		mParent(nullptr), mTransparency(1.0f), mResTransparency(1.0f), mVisible(true), mIsSelected(false), mResVisible(true),
		mFullyDisabled(false)
	{
		layout.mOwner = this;
		InitializeProperties();
		UpdateLayout();
	}

	UIWidget::UIWidget(const UIWidget& other):
		mName(other.mName), layout(other.layout), mParent(nullptr), mTransparency(other.mTransparency),
		mVisible(other.mVisible), mIsSelected(false), mResVisible(other.mVisible), mFullyDisabled(!other.mVisible)
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
			Ptr<UIWidgetState> newState = state->Clone();
			AddState(newState);
		}

		UpdateLayersDrawingSequence();
		UpdateTransparency();
		UpdateLayout();
		UpdateVisibility();
	}

	UIWidget::~UIWidget()
	{
		if (mParent)
			mParent->RemoveChild(Ptr<UIWidget>(this), false);

		for (auto layer : mLayers)
			layer.Release();

		for (auto state : mStates)
			state.Release();

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			child.Release();
		}
	}

	UIWidget& UIWidget::operator=(const UIWidget& other)
	{
		for (auto layer : mLayers)
			layer.Release();

		for (auto state : mStates)
			state.Release();

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			child.Release();
		}
		mChilds.Clear();
		mVisibleState = nullptr;
		mSelectedState = nullptr;

		mName = other.mName;
		layout.CopyFrom(other.layout);
		mTransparency = other.mTransparency;

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
			Ptr<UIWidgetState> newState = state->Clone();
			AddState(newState);
		}

		UpdateLayout();
		UpdateTransparency();
		UpdateVisibility();

		return *this;
	}

	void UIWidget::Update(float dt)
	{
		if (mFullyDisabled)
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
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		for (auto child : mChilds)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UIWidget::DrawDebugFrame()
	{
		static int colr = 0;
		static int lastFrame = 0;

		if (lastFrame != o2Time.GetCurrentFrame())
			colr = 0;

		lastFrame = o2Time.GetCurrentFrame();

		o2Render.DrawRectFrame(layout.mAbsoluteRect, Color4::SomeColor(colr++));
	}

	void UIWidget::OnSelected()
	{}

	void UIWidget::OnDeselected()
	{}

	void UIWidget::SetName(const String& name)
	{
		mName = name;
	}

	String UIWidget::GetName() const
	{
		return mName;
	}

	Ptr<UIWidget> UIWidget::GetParent() const
	{
		return mParent;
	}

	void UIWidget::SetParent(Ptr<UIWidget> parent)
	{
		if (parent->mChilds.Contains(this))
			return;

		if (mParent)
		{
			mParent->RemoveChild(Ptr<UIWidget>(this), false);
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

	Ptr<UIWidget> UIWidget::AddChild(Ptr<UIWidget> widget)
	{
		if (mChilds.Contains(widget))
			return widget;

		if (widget->mParent)
			widget->mParent->RemoveChild(widget, false);

		mChilds.Add(widget);
		widget->mParent = this;

		UpdateLayout();
		UpdateTransparency();
		UpdateVisibility();

		OnChildAdded(widget);

		return widget;
	}

	Ptr<UIWidget> UIWidget::AddChild(Ptr<UIWidget> widget, int index)
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

	bool UIWidget::RemoveChild(Ptr<UIWidget> widget, bool release /*= true*/)
	{
		if (!mChilds.Contains(widget))
			return false;

		widget->mParent = nullptr;
		mChilds.Remove(widget);
		UpdateLayout();

		OnChildRemoved(widget);

		if (release)
			widget.Release();
		else
		{
			widget->UpdateTransparency();
			widget->UpdateVisibility();
		}

		return true;
	}

	Ptr<UIWidget> UIWidget::GetChild(const String& path)
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

	void UIWidget::RemoveAllChilds()
	{
		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			OnChildRemoved(child);
			child.Release();
		}

		mChilds.Clear();
		UpdateLayout();
	}

	const UIWidget::WidgetsVec& UIWidget::GetChilds() const
	{
		return mChilds;
	}

	Ptr<UIWidgetLayer> UIWidget::AddLayer(Ptr<UIWidgetLayer> layer)
	{
		mLayers.Add(layer);
		layer->SetOwnerWidget(this);
		UpdateLayersDrawingSequence();
		OnLayerAdded(layer);
		return layer;
	}

	Ptr<UIWidgetLayer> UIWidget::AddLayer(const String& name, Ptr<IRectDrawable> drawable,
										  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mDrawingLayers.Count();

		Ptr<UIWidgetLayer> layer = mnew UIWidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->drawable = drawable;
		layer->layout = layout;

		AddLayer(layer);

		return layer;
	}

	Ptr<UIWidgetLayer> UIWidget::GetLayer(const String& path) const
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

	bool UIWidget::RemoveLayer(Ptr<UIWidgetLayer> layer)
	{
		bool res = mLayers.Remove(layer);
		layer.Release();
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
			layer.Release();

		mDrawingLayers.Clear();

		mLayers.Clear();
	}

	const LayersVec& UIWidget::GetLayers() const
	{
		return mLayers;
	}

	Ptr<UIWidgetState> UIWidget::AddState(const String& name)
	{
		Ptr<UIWidgetState> newState = mnew UIWidgetState();
		newState->name = name;
		newState->animation.SetTarget(this);
		return AddState(newState);
	}

	Ptr<UIWidgetState> UIWidget::AddState(const String& name, const Animation& animation)
	{
		Ptr<UIWidgetState> newState = mnew UIWidgetState();
		newState->name = name;
		newState->animation = animation;
		newState->animation.SetTarget(this);
		newState->animation.relTime = 0.0f;
		return AddState(newState);
	}

	Ptr<UIWidgetState> UIWidget::AddState(Ptr<UIWidgetState> state)
	{
		mStates.Add(state);

		if (state->name == "visible")
		{
			mVisibleState = state;
			mVisibleState->SetStateForcible(mVisible);
			mVisibleState->onStateBecomesTrue += [&]() { mFullyDisabled = false; };
			mVisibleState->onStateFullyFalse += [&]() { mFullyDisabled = true; };
		}

		if (state->name == "selected")
		{
			mSelectedState = state;
			mSelectedState->SetStateForcible(mIsSelected);
		}

		OnStateAdded(state);

		return state;
	}

	bool UIWidget::RemoveState(const String& name)
	{
		int idx = mStates.FindIdx([&](const Ptr<UIWidgetState>& state) { return state->name == name; });
		if (idx < 0)
			return false;

		if (mStates[idx] == mVisibleState)
			mVisibleState = nullptr;

		if (mStates[idx] == mSelectedState)
			mSelectedState = nullptr;

		mStates[idx].Release();
		mStates.RemoveAt(idx);

		return true;
	}

	bool UIWidget::RemoveState(Ptr<UIWidgetState> state)
	{
		int idx = mStates.Find(state);
		if (idx < 0)
			return false;

		if (state == mVisibleState)
			mVisibleState = nullptr;

		mStates[idx].Release();
		mStates.RemoveAt(idx);

		return true;
	}

	void UIWidget::RemoveAllStates()
	{
		for (auto state : mStates)
			state.Release();

		mVisibleState = nullptr;
		mSelectedState = nullptr;

		mStates.Clear();
	}

	void UIWidget::SetState(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetState(state);
	}

	bool UIWidget::GetState(const String& name) const
	{
		auto state = GetStateObject(name);

		if (state)
			return state->GetState();

		return false;
	}

	Ptr<UIWidgetState> UIWidget::GetStateObject(const String& name) const
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

	void UIWidget::Select()
	{
		o2UI.SelectWidget(this);
	}

	void UIWidget::Deselect()
	{
		o2UI.SelectWidget(nullptr);
	}

	bool UIWidget::IsSelected() const
	{
		return mIsSelected;
	}

	bool UIWidget::IsSelectable() const
	{
		return false;
	}

	void UIWidget::UpdateLayout(bool forcible /*= false*/)
	{
		if (layout.mDrivenByParent && !forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;

		for (auto child : mChilds)
			child->UpdateLayout();
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

	void UIWidget::UpdateVisibility()
	{
		if (mParent)
			mResVisible = mVisible && mParent->mResVisible;
		else
			mResVisible = mVisible;

		if (mVisibleState)
			mVisibleState->SetState(mResVisible);
		else
			mFullyDisabled = !mResVisible;

		OnVisibleChanged();

		for (auto child : mChilds)
			child->UpdateVisibility();
	}

	void UIWidget::OnChildSelected(Ptr<UIWidget> child)
	{
		if (mParent)
			mParent->OnChildSelected(child);
	}

	void UIWidget::RetargetStatesAnimations()
	{
		for (auto state : mStates)
			state->animation.SetTarget(this, false);
	}

	void UIWidget::RecalculateAbsRect()
	{
		Vec2F parentSize, parentPos;
		if (mParent)
		{
			parentSize = mParent->mChildsAbsRect.Size();
			parentPos = mParent->mChildsAbsRect.LeftBottom();
		}

		layout.mLocalRect.left = parentSize.x*layout.mAnchorMin.x + layout.mOffsetMin.x;
		layout.mLocalRect.right = parentSize.x*layout.mAnchorMax.x + layout.mOffsetMax.x;
		layout.mLocalRect.bottom = parentSize.y*layout.mAnchorMin.y + layout.mOffsetMin.y;
		layout.mLocalRect.top = parentSize.y*layout.mAnchorMax.y + layout.mOffsetMax.y;

		Vec2F resSize = layout.mLocalRect.Size();
		Vec2F clampSize(Math::Clamp(resSize.x, layout.mMinSize.x, layout.mMaxSize.x),
						Math::Clamp(resSize.y, layout.mMinSize.y, layout.mMaxSize.y));
		Vec2F szDelta = clampSize - resSize;

		layout.mLocalRect.left -= szDelta.x*layout.mPivot.x;
		layout.mLocalRect.right += szDelta.x*(1.0f - layout.mPivot.x);
		layout.mLocalRect.bottom -= szDelta.y*layout.mPivot.y;
		layout.mLocalRect.top += szDelta.y*(1.0f - layout.mPivot.y);

		layout.mLocalRect.left = Math::Floor(layout.mLocalRect.left);
		layout.mLocalRect.right = Math::Floor(layout.mLocalRect.right);
		layout.mLocalRect.bottom = Math::Floor(layout.mLocalRect.bottom);
		layout.mLocalRect.top = Math::Floor(layout.mLocalRect.top);

		layout.mAbsoluteRect = layout.mLocalRect + parentPos;
	}

	void UIWidget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();
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

	float UIWidget::GetMaxDrawingDepth()
	{
		if (mDrawingLayers.Count() > 0)
			return mDrawingLayers.Last()->drawable->GetDrawingDepth();

		return 0.0f;
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

	Dictionary<String, Ptr<UIWidgetLayer>> UIWidget::GetAllLayers()
	{
		Dictionary<String, Ptr<UIWidgetLayer>> res;
		for (auto layer : mLayers)
			res.Add(layer->name, layer);

		return res;
	}

	Dictionary<String, Ptr<UIWidget>> UIWidget::GetAllChilds()
	{
		Dictionary<String, Ptr<UIWidget>> res;
		for (auto child : mChilds)
			res.Add(child->GetName(), child);

		return res;
	}

	void UIWidget::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{}

	void UIWidget::OnStateAdded(Ptr<UIWidgetState> state)
	{}

	void UIWidget::OnChildAdded(Ptr<UIWidget> child)
	{}

	void UIWidget::OnChildRemoved(Ptr<UIWidget> child)
	{}

	void UIWidget::OnDeserialized(const DataNode& node)
	{
		for (auto layer : mLayers)
			layer->mOwnerWidget = this;

		for (auto state : mStates)
			state->animation.SetTarget(this);

		UpdateLayersDrawingSequence();
	}

	void UIWidget::ForceDraw(const RectF& area, float transparency)
	{
		Vec2F oldLayoutOffsetMin = layout.mOffsetMin;
		Vec2F oldLayoutOffsetMax = layout.mOffsetMax;
		float oldTransparency = mTransparency;
		auto oldParent = mParent;

		layout.mOffsetMin = area.LeftBottom();
		layout.mOffsetMax = area.RightTop();
		mTransparency = transparency;
		mParent = nullptr;

		UpdateLayout(true);
		UpdateTransparency();

		Draw();

		layout.mOffsetMin = oldLayoutOffsetMin;
		layout.mOffsetMax = oldLayoutOffsetMax;
		mTransparency = oldTransparency;
		mParent = oldParent;

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