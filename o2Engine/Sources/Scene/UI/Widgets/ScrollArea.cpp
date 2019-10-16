#include "stdafx.h"
#include "ScrollArea.h"

#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/HorizontalScrollBar.h"
#include "Scene/UI/Widgets/VerticalScrollBar.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Utils/Debug/Debug.h"
#include "Utils/System/Time/Time.h"

namespace o2
{
	ScrollArea::ScrollArea(): Widget()
	{}

	ScrollArea::ScrollArea(const ScrollArea& other) :
		Widget(other), mOwnHorScrollBar(other.mOwnHorScrollBar), mOwnVerScrollBar(other.mOwnVerScrollBar),
		mClipAreaLayout(other.mClipAreaLayout), mScrollPos(other.mScrollPos), mScrollSpeedDamp(other.mScrollSpeedDamp),
		mViewAreaLayout(other.mViewAreaLayout), mEnableScrollsHiding(other.mEnableScrollsHiding), scroll(this),
		horScroll(this), verScroll(this)
	{
		if (mOwnHorScrollBar)
		{
			mHorScrollBar = GetInternalWidgetByType<HorizontalScrollBar>("horScrollBar");
			mHorScrollBar->layout->mData->drivenByParent = true;
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);

			mEnableHorScroll = mHorScrollBar->IsEnabled();
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = GetInternalWidgetByType<VerticalScrollBar>("verScrollBar");
			mVerScrollBar->layout->mData->drivenByParent = true;
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);

			mEnableVerScroll = mVerScrollBar->IsEnabled();
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	ScrollArea::~ScrollArea()
	{
		if (mHorScrollBar)
		{
			if (!mOwnHorScrollBar)
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (!mOwnVerScrollBar)
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}
	}

	ScrollArea& ScrollArea::operator=(const ScrollArea& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void ScrollArea::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped) {
			for (auto child : mDrawingChildren)
				child->Draw();

			return;
		}

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mDrawingChildren)
			child->Draw();

		o2Render.DisableScissorTest();

		CursorAreaEventsListener::OnDrawn();

		for (auto child : mInternalWidgets)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void ScrollArea::Update(float dt)
	{
		mHasScrolled = false;

		mLayoutUpdated = layout->IsDirty();

		Widget::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		CheckScrollBarsVisibility();

		if (!mPressedCursor && mScrollSpeed != Vec2F())
		{
			SetScroll(mScrollPos + mScrollSpeed*dt);

			if (Math::Abs(mScrollSpeed.x) > FLT_EPSILON)
				mScrollSpeed.x -= Math::Sign(mScrollSpeed.x)*
				Math::Min(Math::Abs(mScrollSpeed.x), Math::Max(Math::Abs(mScrollSpeed.x)*dt*mScrollSpeedDamp, 0.05f));

			if (Math::Abs(mScrollSpeed.y) > FLT_EPSILON)
				mScrollSpeed.y -= Math::Sign(mScrollSpeed.y)*
				Math::Min(Math::Abs(mScrollSpeed.y), Math::Max(Math::Abs(mScrollSpeed.y)*dt*mScrollSpeedDamp, 0.05f));
		}

		if (mEnableScrollsHiding)
		{
			const float barsHideDelay = 2;
			float curTime = o2Time.GetApplicationTime();

			if (curTime - mLastHorScrollChangeTime > barsHideDelay && mHorScrollBar && mHorScrollBar->IsEnabled() &&
				mEnableHorScroll)
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = false;

				mHorScrollBar->Hide();
			}

			if (curTime - mLastVerScrollChangeTime > barsHideDelay && mVerScrollBar && mVerScrollBar->IsEnabled() &&
				mEnableVerScroll)
			{
				auto enableVerBarState = state["enableVerBar"];
				if (enableVerBarState)
					*enableVerBarState = false;

				mVerScrollBar->Hide();
			}
		}
	}

	void ScrollArea::SetScroll(const Vec2F& scroll)
	{
		Vec2F newScrollPos(Math::Clamp(scroll.x, mScrollRange.left, mScrollRange.right),
						   Math::Clamp(scroll.y, mScrollRange.bottom, mScrollRange.top));

		Vec2F scrollDelta;

		if (mHorScrollBar)
			mHorScrollBar->SetValue(newScrollPos.x);
		else
			scrollDelta.x = newScrollPos.x - mScrollPos.x;

		if (mVerScrollBar)
			mVerScrollBar->SetValue(newScrollPos.y);
		else
			scrollDelta.y = newScrollPos.y - mScrollPos.y;

		if (!mVerScrollBar || !mHorScrollBar)
			MoveScrollPosition(scrollDelta);

		onScrolled(newScrollPos);
		OnScrolled();
	}

	void ScrollArea::SetScrollForcible(const Vec2F& scroll)
	{
		Vec2F newScrollPos(Math::Clamp(scroll.x, mScrollRange.left, mScrollRange.right),
						   Math::Clamp(scroll.y, mScrollRange.bottom, mScrollRange.top));

		Vec2F scrollDelta;

		if (mHorScrollBar)
			mHorScrollBar->SetValueForcible(newScrollPos.x);
		else
			scrollDelta.x = newScrollPos.x - mScrollPos.x;

		if (mVerScrollBar)
			mVerScrollBar->SetValueForcible(newScrollPos.y);
		else
			scrollDelta.y = newScrollPos.y - mScrollPos.y;

		if (!mVerScrollBar || !mHorScrollBar)
			MoveScrollPosition(scrollDelta);

		onScrolled(newScrollPos);
		OnScrolled();
	}

	Vec2F ScrollArea::GetScroll() const
	{
		return mScrollPos;
	}

	RectF ScrollArea::GetScrollRange() const
	{
		return mScrollRange;
	}

	void ScrollArea::ResetScroll()
	{
		SetScroll(mScrollArea.LeftBottom());
	}

	void ScrollArea::SetHorizontalScroll(float scroll)
	{
		SetScroll(Vec2F(scroll, mScrollPos.y));
	}

	float ScrollArea::GetHorizontalScroll() const
	{
		return mScrollPos.x;
	}

	void ScrollArea::SetVerticalScroll(float scroll)
	{
		SetScroll(Vec2F(mScrollPos.x, scroll));
	}

	float ScrollArea::GetVerticalScroll() const
	{
		return mScrollPos.y;
	}

	void ScrollArea::SetHorizontalScrollBar(HorizontalScrollBar* scrollbar, bool owner /*= true*/)
	{
		if (mHorScrollBar)
		{
			if (!mOwnHorScrollBar)
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		mHorScrollBar = scrollbar;
		mOwnHorScrollBar = owner;

		if (mHorScrollBar)
		{
			mHorScrollBar->name = "horScrollBar";
			mHorScrollBar->SetInternalParent(this, false);
			mHorScrollBar->layout->mData->drivenByParent = true;
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
		}

		UpdateScrollParams();
		SetLayoutDirty();
	}

	HorizontalScrollBar* ScrollArea::GetHorizontalScrollbar() const
	{
		return mHorScrollBar;
	}

	void ScrollArea::SetVerticalScrollBar(VerticalScrollBar* scrollbar, bool owner /*= true*/)
	{
		if (mVerScrollBar)
		{
			if (!mOwnVerScrollBar)
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}

		mVerScrollBar = scrollbar;
		mOwnVerScrollBar = owner;

		if (mVerScrollBar)
		{
			mVerScrollBar->name = "verScrollBar";
			mVerScrollBar->SetInternalParent(this, false);
			mVerScrollBar->layout->mData->drivenByParent = true;
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
		}

		UpdateScrollParams();
		SetLayoutDirty();
	}

	VerticalScrollBar* ScrollArea::GetVerticalScrollbar() const
	{
		return mVerScrollBar;
	}

	void ScrollArea::SetEnableScrollsHiding(bool hideScrolls)
	{
		mEnableScrollsHiding = hideScrolls;

		if (!mEnableScrollsHiding)
		{
			if (mHorScrollBar && !mHorScrollBar->IsEnabled())
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = true;

				mHorScrollBar->Show(true);
			}

			if (mVerScrollBar && !mVerScrollBar->IsEnabled())
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = true;

				mVerScrollBar->Show(true);
			}
		}
	}

	bool ScrollArea::IsScrollsHiding()
	{
		return mEnableScrollsHiding;
	}

	void ScrollArea::SetScrollBarsShowingByCursor(bool showByCursor)
	{
		mShowScrollBarsByCursor = showByCursor;
	}

	bool ScrollArea::IsScrollBarsShowingByCursor()
	{
		return mShowScrollBarsByCursor;
	}

	void ScrollArea::SetClippingLayout(const Layout& clipLayout)
	{
		mClipAreaLayout = clipLayout;
		SetLayoutDirty();
	}

	Layout ScrollArea::GetClippingLayout() const
	{
		return mClipAreaLayout;
	}

	void ScrollArea::SetViewLayout(const Layout& viewLayout)
	{
		mViewAreaLayout = viewLayout;
		SetLayoutDirty();
	}

	Layout ScrollArea::GetViewLayout() const
	{
		return mViewAreaLayout;
	}

	void ScrollArea::OnChildAdded(Widget* child)
	{
		child->layout->mData->drivenByParent = true;
	}

	void ScrollArea::OnChildRemoved(Widget* child)
	{
		child->layout->mData->drivenByParent = false;
	}

	void ScrollArea::CheckScrollBarsVisibility()
	{
		if (mEnableScrollsHiding && mShowScrollBarsByCursor)
		{
			auto cursor = o2Input.GetCursor(0);

			if (cursor)
			{
				if (mHorScrollBar && mHorScrollBar->IsUnderPoint(cursor->position) &&
					mEnableHorScroll)
				{
					mLastHorScrollChangeTime = o2Time.GetApplicationTime();

					if (!mHorScrollBar->IsEnabled())
					{
						auto enableHorBarState = state["enableHorBar"];
						if (enableHorBarState)
							*enableHorBarState = true;

						mHorScrollBar->Show();
					}
				}

				if (mVerScrollBar && mVerScrollBar->IsUnderPoint(cursor->position) &&
					mEnableVerScroll)
				{
					mLastVerScrollChangeTime = o2Time.GetApplicationTime();

					if (!mVerScrollBar->IsEnabled())
					{
						auto enableVerBarState = state["enableVerBar"];
						if (enableVerBarState)
							*enableVerBarState = true;

						mVerScrollBar->Show();
					}
				}
			}
		}
	}

	void ScrollArea::UpdateSelfTransform()
	{
		layout->Update();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout->mData->worldRectangle);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout->mData->worldRectangle);
		mChildrenWorldRect = mAbsoluteViewArea;
	}

	void ScrollArea::UpdateChildren(float dt)
	{
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChildren)
			child->Update(dt);

		for (auto child : mChildren)
			child->UpdateChildren(dt);

		mChildrenWorldRect = mAbsoluteViewArea;

		for (auto child : mInternalWidgets)
			child->Update(dt);

		for (auto child : mInternalWidgets)
			child->UpdateChildren(dt);

		if (mLayoutUpdated && !!mResEnabledInHierarchy)
		{
			CheckChildrenClipping();
			UpdateScrollParams();
		}
	}

	void ScrollArea::UpdateChildrenTransforms()
	{
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		Widget::UpdateChildrenTransforms();

		mChildrenWorldRect = mAbsoluteViewArea;

		CheckChildrenClipping();
		UpdateScrollParams();
	}

	bool ScrollArea::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	bool ScrollArea::IsScrollable() const
	{
		return true;
	}

	bool ScrollArea::IsInputTransparent() const
	{
		return !mHasScrolled;
	}

	void ScrollArea::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		Vec2F widgetsMove(-delta.x, delta.y);
		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(widgetsMove, mAbsoluteClipArea);

		mChildrenWorldRect = mAbsoluteViewArea;

		UpdateScrollParams();
	}

	void ScrollArea::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateSelfTransform();

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		mChildrenWorldRect = mAbsoluteViewArea;

		for (auto child : mInternalWidgets)
			child->MoveAndCheckClipping(delta, clipArea);
	}

	void ScrollArea::CheckChildrenClipping()
	{
		for (auto child : mChildWidgets)
			child->CheckClipping(mAbsoluteClipArea);
	}

	void ScrollArea::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		RectF newClipArea = clipArea.GetIntersection(mAbsoluteClipArea);

		for (auto child : mChildWidgets)
			child->CheckClipping(newClipArea);
	}

	void ScrollArea::CalculateScrollArea()
	{
		Vec2F offset;
		InitializeScrollAreaRectCalculation(offset);

		for (auto child : mChildWidgets)
		{
			if (!child->mResEnabledInHierarchy || child->GetType() == TypeOf(ContextMenu))
				continue;

			RecalculateScrollAreaRect(child->layout->mData->rectangle, offset);
		}
	}

	void ScrollArea::RecalculateScrollAreaRect(const RectF &childRect, const Vec2F &offset)
	{
		mScrollArea.left = Math::Min(mScrollArea.left, childRect.left - offset.x);
		mScrollArea.bottom = Math::Min(mScrollArea.bottom, childRect.bottom - offset.y);
		mScrollArea.right = Math::Max(mScrollArea.right, childRect.right - offset.x);
		mScrollArea.top = Math::Max(mScrollArea.top, childRect.top - offset.y);
	}

	void ScrollArea::InitializeScrollAreaRectCalculation(Vec2F& offset)
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		offset = mChildrenWorldRect.LeftBottom() - layout->mData->worldRectangle.LeftBottom() - mChildrenWorldRect.Size()*layout->pivot + 
			Vec2F(Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
	}

	void ScrollArea::UpdateScrollParams()
	{
		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout->mData->worldRectangle);
		RectF localViewArea(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		CalculateScrollArea();

		mScrollRange.left = localViewArea.left + mScrollArea.left;
		mScrollRange.right = mScrollArea.right - localViewArea.right;

		mScrollRange.bottom = localViewArea.top - mScrollArea.top;
		mScrollRange.top = localViewArea.bottom - mScrollArea.bottom;

// 		o2Debug.Log(mName + " area: " + (String)mScrollArea + ", range: " + (String)mScrollRange + 
// 					", scroll: " + (String)mScrollPos);

// 		mScrollPos.x = Math::Clamp(mScrollPos.x, mScrollRange.left, mScrollRange.right);
// 		mScrollPos.y = Math::Clamp(mScrollPos.y, mScrollRange.bottom, mScrollRange.top);

		if (mHorScrollBar)
		{
			if (Math::Equals(mScrollRange.left, mScrollRange.right, 1.2f))
			{
				if (mHorScrollBar->IsEnabled())
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = false;

					mHorScrollBar->Hide();
				}

				mHorScrollBar->SetValueRange(0, 0);

				mEnableHorScroll = false;
			}
			else
			{
				if (!mEnableHorScroll)
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = true;

					mHorScrollBar->Show();
				}

				mEnableHorScroll = true;

				mHorScrollBar->SetValueRange(mScrollRange.left, mScrollRange.right);
				mHorScrollBar->SetScrollHandleSize(localViewArea.Width());
			}
		}

		if (mVerScrollBar)
		{
			if (Math::Equals(mScrollRange.bottom, mScrollRange.top, 1.2f))
			{
				if (mVerScrollBar->IsEnabled())
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = false;

					mVerScrollBar->Hide();
				}

				mVerScrollBar->SetValueRange(0, 0);

				mEnableVerScroll = false;
			}
			else
			{
				if (!mEnableVerScroll)
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = true;

					mVerScrollBar->Show();
				}

				mEnableVerScroll = true;

				mVerScrollBar->SetValueRange(mScrollRange.bottom, mScrollRange.top);
				mVerScrollBar->SetScrollHandleSize(localViewArea.Height());
			}
		}
	}

	void ScrollArea::OnHorScrollChanged(float value)
	{
		if (!mHorScrollBar->IsEnabled() && mEnableHorScroll)
		{
			auto enableHorBarState = state["enableHorBar"];
			if (enableHorBarState)
				*enableHorBarState = true;

			mHorScrollBar->Show();
		}

		mLastHorScrollChangeTime = o2Time.GetApplicationTime();

		Vec2F delta(Math::Clamp(value, mScrollRange.left, mScrollRange.right) - mScrollPos.x, 0.0f);
		MoveScrollPosition(delta);

		onScrolled(mScrollPos);
		OnScrolled();
	}

	void ScrollArea::OnVerScrollChanged(float value)
	{
		if (!mVerScrollBar->IsEnabled() && mEnableVerScroll)
		{
			auto enableVerBarState = state["enableVerBar"];
			if (enableVerBarState)
				*enableVerBarState = true;

			mVerScrollBar->Show();
		}

		mLastVerScrollChangeTime = o2Time.GetApplicationTime();

		Vec2F delta(0.0f, Math::Clamp(value, mScrollRange.bottom, mScrollRange.top) - mScrollPos.y);
		MoveScrollPosition(delta);

		onScrolled(mScrollPos);
		OnScrolled();
	}

	void ScrollArea::CopyData(const Actor& otherActor)
	{
		const ScrollArea& other = dynamic_cast<const ScrollArea&>(otherActor);

		if (mHorScrollBar)
		{
			if (!mOwnHorScrollBar)
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (!mOwnVerScrollBar)
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}

		Widget::CopyData(other);

		mClipAreaLayout      = other.mClipAreaLayout;
		mViewAreaLayout      = other.mViewAreaLayout;
		mScrollPos           = other.mScrollPos;
		mOwnHorScrollBar     = other.mOwnHorScrollBar;
		mOwnVerScrollBar     = other.mOwnVerScrollBar;
		mScrollSpeedDamp     = other.mScrollSpeedDamp;
		mEnableScrollsHiding = other.mEnableScrollsHiding;

		if (mOwnHorScrollBar)
		{
			mHorScrollBar = GetInternalWidgetByType<HorizontalScrollBar>("horScrollBar");
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
			mHorScrollBar->layout->mData->drivenByParent = true;

			mEnableHorScroll = mHorScrollBar->IsEnabled();
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = GetInternalWidgetByType<VerticalScrollBar>("verScrollBar");
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
			mVerScrollBar->layout->mData->drivenByParent = true;

			mEnableVerScroll = mVerScrollBar->IsEnabled();
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		UpdateScrollParams();
		SetLayoutDirty();
	}

	void ScrollArea::OnDeserialized(const DataNode& node)
	{
		Widget::OnDeserialized(node);

		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar) delete mHorScrollBar;
			else                  mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);

			mEnableHorScroll = mHorScrollBar->IsEnabled();
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar) delete mVerScrollBar;
			else                  mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);

			mEnableVerScroll = mVerScrollBar->IsEnabled();
		}

		mHorScrollBar = GetInternalWidgetByType<HorizontalScrollBar>("horScrollBar");
		mOwnHorScrollBar = mHorScrollBar != nullptr;
		if (mOwnHorScrollBar)
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);

		mVerScrollBar = GetInternalWidgetByType<VerticalScrollBar>("verScrollBar");
		mOwnVerScrollBar = mVerScrollBar != nullptr;
		if (mOwnVerScrollBar)
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);

		for (auto child : mChildWidgets)
			child->layout->mData->drivenByParent = true;

		RetargetStatesAnimations();
	}

	void ScrollArea::OnScrolled()
	{}

	void ScrollArea::OnScrolled(float scroll)
	{
		mScrollSpeed = Vec2F();

		if (mVerScrollBar && mEnableVerScroll)
		{
			mVerScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
			mHasScrolled = !Math::Equals(mVerScrollBar->GetMinValue(), mVerScrollBar->GetMaxValue());
		}
		else if (mHorScrollBar && mEnableHorScroll)
		{
			mHorScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
			mHasScrolled = !Math::Equals(mHorScrollBar->GetMinValue(), mHorScrollBar->GetMaxValue());
		}
	}

}

DECLARE_CLASS(o2::ScrollArea);
