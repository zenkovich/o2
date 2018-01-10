#include "stdafx.h"
#include "ScrollArea.h"

#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "UI/ContextMenu.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Debug.h"
#include "Utils/Time.h"

namespace o2
{
	UIScrollArea::UIScrollArea(): UIWidget()
	{
		InitializeProperties();
	}

	UIScrollArea::UIScrollArea(const UIScrollArea& other) :
		UIWidget(other), mOwnHorScrollBar(other.mOwnHorScrollBar), mOwnVerScrollBar(other.mOwnVerScrollBar),
		mClipAreaLayout(other.mClipAreaLayout), mScrollPos(other.mScrollPos), mScrollSpeedDamp(other.mScrollSpeedDamp),
		mViewAreaLayout(other.mViewAreaLayout), mEnableScrollsHiding(other.mEnableScrollsHiding)
	{
		InitializeProperties();

		if (mOwnHorScrollBar)
		{
			mHorScrollBar = other.mHorScrollBar->CloneAs<UIHorizontalScrollBar>();
			mHorScrollBar->SetWeakParent(this, false);
			mHorScrollBar->layout->mData->drivenByParent = true;
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->CloneAs<UIVerticalScrollBar>();
			mVerScrollBar->SetWeakParent(this, false);
			mVerScrollBar->layout->mData->drivenByParent = true;
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UIScrollArea::~UIScrollArea()
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				delete mHorScrollBar;
			else
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				delete mVerScrollBar;
			else
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}
	}

	UIScrollArea& UIScrollArea::operator=(const UIScrollArea& other)
	{
		CopyData(other);
		return *this;
	}

	void UIScrollArea::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mDrawingChildren)
			child->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		DrawDebugFrame();
	}

	void UIScrollArea::Update(float dt)
	{
		mLayoutUpdated = layout->IsDirty();

		UIWidget::Update(dt);

		if (mFullyDisabled || mIsClipped)
			return;

		if (mOwnHorScrollBar)
			mHorScrollBar->Update(dt);

		if (mOwnVerScrollBar)
			mVerScrollBar->Update(dt);

		UpdateControls(dt);
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

			if (curTime - mLastHorScrollChangeTime > barsHideDelay && mHorScrollBar && mHorScrollBar->IsVisible() &&
				mEnableHorScroll)
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = false;

				mHorScrollBar->Hide();
			}

			if (curTime - mLastVerScrollChangeTime > barsHideDelay && mVerScrollBar && mVerScrollBar->IsVisible() &&
				mEnableVerScroll)
			{
				auto enableVerBarState = state["enableVerBar"];
				if (enableVerBarState)
					*enableVerBarState = false;

				mVerScrollBar->Hide();
			}
		}
	}

	void UIScrollArea::SetScroll(const Vec2F& scroll)
	{
		Vec2F newScrollPos(Math::Clamp(scroll.x, mScrollRange.left, mScrollRange.right),
						   Math::Clamp(scroll.y, mScrollRange.bottom, mScrollRange.top));

		Vec2F scrollDelta;

		if (mHorScrollBar)
			mHorScrollBar->SetValue(newScrollPos.x);
		else
			scrollDelta.x = newScrollPos.x;

		if (mVerScrollBar)
			mVerScrollBar->SetValue(newScrollPos.y);
		else
			scrollDelta.y = newScrollPos.y;

		if (!mVerScrollBar || !mHorScrollBar)
			MoveScrollPosition(scrollDelta);

		onScrolled(newScrollPos);
		OnScrolled();
	}

	void UIScrollArea::SetScrollForcible(const Vec2F& scroll)
	{
		Vec2F newScrollPos(Math::Clamp(scroll.x, mScrollRange.left, mScrollRange.right),
						   Math::Clamp(scroll.y, mScrollRange.bottom, mScrollRange.top));

		Vec2F scrollDelta;

		if (mHorScrollBar)
			mHorScrollBar->SetValueForcible(newScrollPos.x);
		else
			scrollDelta.x = newScrollPos.x;

		if (mVerScrollBar)
			mVerScrollBar->SetValueForcible(newScrollPos.y);
		else
			scrollDelta.y = newScrollPos.y;

		if (!mVerScrollBar || !mHorScrollBar)
			MoveScrollPosition(scrollDelta);

		onScrolled(newScrollPos);
		OnScrolled();
	}

	Vec2F UIScrollArea::GetScroll() const
	{
		return mScrollPos;
	}

	RectF UIScrollArea::GetScrollRange() const
	{
		return mScrollRange;
	}

	void UIScrollArea::ResetScroll()
	{
		SetScroll(mScrollArea.LeftBottom());
	}

	void UIScrollArea::SetHorizontalScroll(float scroll)
	{
		SetScroll(Vec2F(scroll, mScrollPos.y));
	}

	float UIScrollArea::GetHorizontalScroll() const
	{
		return mScrollPos.x;
	}

	void UIScrollArea::SetVerticalScroll(float scroll)
	{
		SetScroll(Vec2F(mScrollPos.x, scroll));
	}

	float UIScrollArea::GetVerticalScroll() const
	{
		return mScrollPos.y;
	}

	void UIScrollArea::SetHorizontalScrollBar(UIHorizontalScrollBar* scrollbar, bool owner /*= true*/)
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				delete mHorScrollBar;
			else
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		mHorScrollBar = scrollbar;
		mOwnHorScrollBar = owner;

		if (mHorScrollBar)
		{
			mHorScrollBar->SetWeakParent(this, false);
			mHorScrollBar->layout->mData->drivenByParent = true;
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
		}

		UpdateScrollParams();
		SetLayoutDirty();
	}

	UIHorizontalScrollBar* UIScrollArea::GetHorizontalScrollbar() const
	{
		return mHorScrollBar;
	}

	void UIScrollArea::SetVerticalScrollBar(UIVerticalScrollBar* scrollbar, bool owner /*= true*/)
	{
		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				delete mVerScrollBar;
			else
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}

		mVerScrollBar = scrollbar;
		mOwnVerScrollBar = owner;

		if (mVerScrollBar)
		{
			mVerScrollBar->SetWeakParent(this, false);
			mVerScrollBar->layout->mData->drivenByParent = true;
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
		}

		UpdateScrollParams();
		SetLayoutDirty();
	}

	UIVerticalScrollBar* UIScrollArea::GetVerticalScrollbar() const
	{
		return mVerScrollBar;
	}

	void UIScrollArea::SetEnableScrollsHiding(bool hideScrolls)
	{
		mEnableScrollsHiding = hideScrolls;

		if (!mEnableScrollsHiding)
		{
			if (mHorScrollBar && !mHorScrollBar->IsVisible())
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = true;

				mHorScrollBar->Show(true);
			}

			if (mVerScrollBar && !mVerScrollBar->IsVisible())
			{
				auto enableHorBarState = state["enableHorBar"];
				if (enableHorBarState)
					*enableHorBarState = true;

				mVerScrollBar->Show(true);
			}
		}
	}

	bool UIScrollArea::IsScrollsHiding()
	{
		return mEnableScrollsHiding;
	}

	void UIScrollArea::SetScrollBarsShowingByCursor(bool showByCursor)
	{
		mShowScrollBarsByCursor = showByCursor;
	}

	bool UIScrollArea::IsScrollBarsShowingByCursor()
	{
		return mShowScrollBarsByCursor;
	}

	void UIScrollArea::SetClippingLayout(const Layout& clipLayout)
	{
		mClipAreaLayout = clipLayout;
		SetLayoutDirty();
	}

	Layout UIScrollArea::GetClippingLayout() const
	{
		return mClipAreaLayout;
	}

	void UIScrollArea::SetViewLayout(const Layout& viewLayout)
	{
		mViewAreaLayout = viewLayout;
		SetLayoutDirty();
	}

	Layout UIScrollArea::GetViewLayout() const
	{
		return mViewAreaLayout;
	}

	void UIScrollArea::OnChildAdded(UIWidget* child)
	{
		child->layout->mData->drivenByParent = true;
	}

	void UIScrollArea::OnChildRemoved(UIWidget* child)
	{
		child->layout->mData->drivenByParent = false;
	}

	void UIScrollArea::UpdateControls(float dt)
	{
		auto cursor = o2Input.GetCursor(0);
		bool underCursorAtFrame = layout->IsPointInside(cursor->position);
		bool underClippingArea = mAbsoluteClipArea.IsInside(cursor->position);
		bool underScrollbars =
			((mHorScrollBar && mOwnHorScrollBar) ? mHorScrollBar->IsUnderPoint(cursor->position) : false) ||
			((mVerScrollBar && mOwnVerScrollBar) ? mVerScrollBar->IsUnderPoint(cursor->position) : false);

		bool lastPressedCursor = mPressedCursor;

		if (!mUnderCursor && underCursorAtFrame)
		{
			mUnderCursor = true;
			auto selectState = state["select"];
			if (selectState)
				*selectState = true;
		}

		if (mUnderCursor && !underCursorAtFrame)
		{
			mUnderCursor = false;
			auto selectState = state["select"];
			if (selectState)
				*selectState = false;
		}

		if (cursor->isPressed && Math::Equals(cursor->pressedTime, 0.0f) && underClippingArea && !mPressedCursor && 
			!underScrollbars)
		{
			mPressedCursor = true;
			auto selectState = state["pressed"];
			if (selectState)
				*selectState = true;

			mPressedScroll = mScrollPos;
			mPressedCursorPos = cursor->position;
		}

		if (!cursor->isPressed && mPressedCursor && mPressedCursor)
		{
			mPressedCursor = false;
			auto selectState = state["pressed"];
			if (selectState)
				*selectState = false;
		}

		if (!Math::Equals(o2Input.GetMouseWheelDelta(), 0.0f) && underClippingArea && !underScrollbars)
		{
			CursorAreaEventsListener* listenerunderCursor = nullptr;
			for (auto x : o2Events.GetAllCursorListenersUnderCursor(0))
			{
				auto scrollArea = dynamic_cast<UIScrollArea*>(x);
				if (scrollArea)
				{
					if (scrollArea != this)
						listenerunderCursor = x;

					break;
				}
			}

			if (listenerunderCursor == nullptr || !listenerunderCursor->IsScrollable())
			{
				mScrollSpeed = Vec2F();

				if (mVerScrollBar && mEnableVerScroll)
					mVerScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
				else if (mHorScrollBar && mEnableHorScroll)
					mHorScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
			}
		}

		if (lastPressedCursor && mPressedCursor && false)
		{
			if (!Math::Equals(mScrollRange.left, mScrollRange.right) || !Math::Equals(mScrollRange.top, mScrollRange.bottom))
			{
				Vec2F delta = cursor->position - mPressedCursorPos;
				delta.x = -delta.x;

				if (delta != Vec2F())
				{
					o2Events.BreakCursorEvent();

					Vec2F newScroll = mPressedScroll + delta;
					Vec2F scrollDelta = newScroll - mScrollPos;

					if (scrollDelta.Length() > 7.0f)
					{
						mScrollSpeed = scrollDelta / dt;
						mSpeedUpdTime = 0.0f;
					}
					else
					{
						mSpeedUpdTime += dt;
						if (mSpeedUpdTime > 0.2f)
						{
							mScrollSpeed = Vec2F();
							mSpeedUpdTime = 0.0f;
						}
					}

					SetScroll(mPressedScroll + delta);
				}
			}
		}
	}

	void UIScrollArea::CheckScrollBarsVisibility()
	{
		if (mEnableScrollsHiding && mShowScrollBarsByCursor)
		{
			auto cursor = o2Input.GetCursor(0);

			if (mHorScrollBar && mHorScrollBar->IsUnderPoint(cursor->position) && mEnableHorScroll)
			{
				mLastHorScrollChangeTime = o2Time.GetApplicationTime();

				if (!mHorScrollBar->IsVisible())
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = true;

					mHorScrollBar->Show();
				}
			}

			if (mVerScrollBar && mVerScrollBar->IsUnderPoint(cursor->position) && mEnableVerScroll)
			{
				mLastVerScrollChangeTime = o2Time.GetApplicationTime();

				if (!mVerScrollBar->IsVisible())
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = true;

					mVerScrollBar->Show();
				}
			}
		}
	}

	void UIScrollArea::UpdateTransform(bool withChildren /*= true*/)
	{
		layout->Update();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout->mData->worldRectangle);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout->mData->worldRectangle);

		if (withChildren)
			UpdateChildrenTransforms();
	}

	void UIScrollArea::UpdateChildren(float dt)
	{
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		UIWidget::UpdateChildren(dt);

		mChildrenWorldRect = mAbsoluteViewArea;

		if (mLayoutUpdated)
		{
			CheckChildrenClipping();
			UpdateScrollParams();
		}
	}

	void UIScrollArea::UpdateChildrenTransforms()
	{
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		UIWidget::UpdateChildrenTransforms();

		mChildrenWorldRect = mAbsoluteViewArea;

		CheckChildrenClipping();
		UpdateScrollParams();
	}

	void UIScrollArea::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		Vec2F widgetsMove(-delta.x, delta.y);
		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(widgetsMove, mAbsoluteClipArea);

		mChildrenWorldRect = mAbsoluteViewArea;

		UpdateScrollParams();
		UpdateScrollBarsLayout();
	}

	void UIScrollArea::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateTransform(false);

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		mChildrenWorldRect = mAbsoluteViewArea;
	}

	void UIScrollArea::UpdateScrollBarsLayout()
	{
		RectF tmpChildsAbsRect = mChildrenWorldRect;
		mChildrenWorldRect = mAbsoluteViewArea;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateTransform();

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateTransform();

		mChildrenWorldRect = tmpChildsAbsRect;
	}

	void UIScrollArea::CheckChildrenClipping()
	{
		for (auto child : mChildWidgets)
			child->CheckClipping(mAbsoluteClipArea);
	}

	void UIScrollArea::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		RectF newClipArea = clipArea.GetIntersection(mAbsoluteClipArea);

		for (auto child : mChildWidgets)
			child->CheckClipping(newClipArea);
	}

	void UIScrollArea::UpdateTransparency()
	{
		UIWidget::UpdateTransparency();

		if (mHorScrollBar)
			mHorScrollBar->UpdateTransparency();

		if (mVerScrollBar)
			mVerScrollBar->UpdateTransparency();
	}

	void UIScrollArea::CalculateScrollArea()
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		Vec2F offset = mChildrenWorldRect.LeftBottom() - layout->mData->worldRectangle.LeftBottom() - 
			mChildrenWorldRect.Size()*layout->pivot;

		for (auto child : mChildWidgets)
		{
			if (child->mFullyDisabled || child->GetType() == TypeOf(UIContextMenu))
				continue;

			mScrollArea.left   = Math::Min(mScrollArea.left, child->layout->mData->rectangle.left - offset.x);
			mScrollArea.bottom = Math::Min(mScrollArea.bottom, child->layout->mData->rectangle.bottom - offset.y);
			mScrollArea.right  = Math::Max(mScrollArea.right, child->layout->mData->rectangle.right - offset.x);
			mScrollArea.top    = Math::Max(mScrollArea.top, child->layout->mData->rectangle.top - offset.y);
		}
	}

	void UIScrollArea::UpdateScrollParams()
	{
		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout->mData->worldRectangle);
		RectF localViewArea(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		CalculateScrollArea();

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mScrollRange = RectF(mScrollArea.left - localViewArea.left - roundedScrollPos.x,
							 localViewArea.Height() - mScrollArea.top + localViewArea.bottom + roundedScrollPos.y,
							 -(localViewArea.Width() - mScrollArea.right + localViewArea.left + roundedScrollPos.x),
							 -mScrollArea.bottom + localViewArea.bottom + roundedScrollPos.y);
// 
// 		o2Debug.Log(mName + " area: " + (String)mScrollArea + ", range: " + (String)mScrollRange + 
// 					", scroll: " + (String)mScrollPos);

		if (mHorScrollBar)
		{
			if (Math::Equals(mScrollRange.left, mScrollRange.right, 1.2f))
			{
				if (mHorScrollBar->IsVisible())
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = false;

					mHorScrollBar->Hide();
					mHorScrollBar->SetValueRange(0, 0);
				}

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
				if (mVerScrollBar->IsVisible())
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = false;

					mVerScrollBar->Hide();
					mVerScrollBar->SetValueRange(0, 0);
				}

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

	void UIScrollArea::OnHorScrollChanged(float value)
	{
		if (!mHorScrollBar->IsVisible() && mEnableHorScroll)
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

	void UIScrollArea::OnVerScrollChanged(float value)
	{
		if (!mVerScrollBar->IsVisible() && mEnableVerScroll)
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

	void UIScrollArea::CopyData(const Actor& otherActor)
	{
		const UIScrollArea& other = dynamic_cast<const UIScrollArea&>(otherActor);

		UIWidget::CopyData(other);

		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				delete mHorScrollBar;
			else
				mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				delete mVerScrollBar;
			else
				mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}

		mClipAreaLayout      = other.mClipAreaLayout;
		mViewAreaLayout      = other.mViewAreaLayout;
		mScrollPos           = other.mScrollPos;
		mOwnHorScrollBar     = other.mOwnHorScrollBar;
		mOwnVerScrollBar     = other.mOwnVerScrollBar;
		mScrollSpeedDamp     = other.mScrollSpeedDamp;
		mEnableScrollsHiding = other.mEnableScrollsHiding;

		if (mOwnHorScrollBar)
		{
			mHorScrollBar = other.mHorScrollBar->CloneAs<UIHorizontalScrollBar>();
			mHorScrollBar->SetWeakParent(this, false);
			mHorScrollBar->layout->mData->drivenByParent = true;
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->CloneAs<UIVerticalScrollBar>();
			mVerScrollBar->SetWeakParent(this, false);
			mVerScrollBar->layout->mData->drivenByParent = true;
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		UpdateScrollParams();
		SetLayoutDirty();
	}

	void UIScrollArea::OnSerialize(DataNode& node) const
	{
		UIWidget::OnSerialize(node);

		if (mOwnHorScrollBar)
			*node.AddNode("mHorScrollBar") = mHorScrollBar;

		if (mOwnVerScrollBar)
			*node.AddNode("mVerScrollBar") = mVerScrollBar;
	}

	void UIScrollArea::OnDeserialized(const DataNode& node)
	{
		UIWidget::OnDeserialized(node);

		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar) delete mHorScrollBar;
			else                  mHorScrollBar->onSmoothChange -= THIS_FUNC(OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar) delete mVerScrollBar;
			else                  mVerScrollBar->onSmoothChange -= THIS_FUNC(OnVerScrollChanged);
		}

		auto horScrollNode = node.GetNode("mHorScrollBar");
		mOwnHorScrollBar = horScrollNode != nullptr;
		if (mOwnHorScrollBar)
		{
			mHorScrollBar = *horScrollNode;
			mHorScrollBar->SetWeakParent(this, false);
			mHorScrollBar->onSmoothChange += THIS_FUNC(OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		auto varScrollNode = node.GetNode("mVerScrollBar");
		mOwnVerScrollBar = varScrollNode != nullptr;
		if (mOwnVerScrollBar)
		{
			mVerScrollBar = *varScrollNode;
			mVerScrollBar->SetWeakParent(this, false);
			mVerScrollBar->onSmoothChange += THIS_FUNC(OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		for (auto child : mChildWidgets)
			child->layout->mData->drivenByParent = true;

		RetargetStatesAnimations();
	}

	void UIScrollArea::OnTransformUpdated()
	{
		UIWidget::OnTransformUpdated();
		UpdateScrollBarsLayout();
	}

	void UIScrollArea::OnScrolled()
	{}

	void UIScrollArea::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIScrollArea, scroll, SetScroll, GetScroll);
		INITIALIZE_PROPERTY(UIScrollArea, horScroll, SetHorizontalScroll, GetHorizontalScroll);
		INITIALIZE_PROPERTY(UIScrollArea, verScroll, SetVerticalScroll, GetVerticalScroll);
	}
}

DECLARE_CLASS(o2::UIScrollArea);
