#include "ScrollArea.h"

#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "UI/ContextMenu.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"
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
			mHorScrollBar = other.mHorScrollBar->Clone();
			mHorScrollBar->mParent = this;
			mHorScrollBar->layout.mDrivenByParent = true;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else  mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->Clone();
			mVerScrollBar->mParent = this;
			mVerScrollBar->layout.mDrivenByParent = true;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIScrollArea::~UIScrollArea()
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				delete mHorScrollBar;
			else
				mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				delete mVerScrollBar;
			else
				mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
	}

	UIScrollArea& UIScrollArea::operator=(const UIScrollArea& other)
	{
		UIWidget::operator=(other);

		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				delete mHorScrollBar;
			else
				mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				delete mVerScrollBar;
			else
				mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
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
			mHorScrollBar = other.mHorScrollBar->Clone();
			mHorScrollBar->mParent = this;
			mHorScrollBar->layout.mDrivenByParent = true;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->Clone();
			mVerScrollBar->mParent = this;
			mVerScrollBar->layout.mDrivenByParent = true;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		RetargetStatesAnimations();
		UpdateScrollParams();
		UpdateLayout();

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

		for (auto child : mChilds)
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
		UIWidget::Update(dt);

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
			const float barsHideDelay = 1;
			float curTime = o2Time.GetApplicationTime();

			if (curTime - mLastHorScrollChangeTime > barsHideDelay &&mHorScrollBar && mHorScrollBar->IsVisible() &&
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
				mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		mHorScrollBar = scrollbar;
		mOwnHorScrollBar = owner;

		if (mHorScrollBar)
		{
			mHorScrollBar->mParent = this;
			mHorScrollBar->layout.mDrivenByParent = true;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		UpdateScrollParams();
		UpdateLayout();
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
				mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		mVerScrollBar = scrollbar;
		mOwnVerScrollBar = owner;

		if (mVerScrollBar)
		{
			mVerScrollBar->mParent = this;
			mVerScrollBar->layout.mDrivenByParent = true;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		UpdateScrollParams();
		UpdateLayout();
	}

	UIVerticalScrollBar* UIScrollArea::GetVerticalScrollbar() const
	{
		return mVerScrollBar;
	}

	void UIScrollArea::SetEnableScrollsHiding(bool hideScrolls)
	{
		mEnableScrollsHiding = hideScrolls;
	}

	bool UIScrollArea::IsScrollsHiding()
	{
		return mEnableScrollsHiding;
	}

	void UIScrollArea::SetClippingLayout(const Layout& clipLayout)
	{
		mClipAreaLayout = clipLayout;
		UpdateLayout();
	}

	Layout UIScrollArea::GetClippingLayout() const
	{
		return mClipAreaLayout;
	}

	void UIScrollArea::SetViewLayout(const Layout& viewLayout)
	{
		mViewAreaLayout = viewLayout;
		UpdateLayout();
	}

	Layout UIScrollArea::GetViewLayout() const
	{
		return mViewAreaLayout;
	}

	void UIScrollArea::OnChildAdded(UIWidget* child)
	{
		child->layout.mDrivenByParent = true;
	}

	void UIScrollArea::OnChildRemoved(UIWidget* child)
	{
		child->layout.mDrivenByParent = false;
	}

	void UIScrollArea::UpdateControls(float dt)
	{
		auto cursor = o2Input.GetCursor(0);
		bool underCursorAtFrame = layout.mAbsoluteRect.IsInside(cursor->position);
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

		if (cursor->isPressed && Math::Equals(cursor->pressedTime, 0.0f) && underClippingArea && !mPressedCursor && !underScrollbars)
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
		auto cursor = o2Input.GetCursor(0);

		if (mEnableScrollsHiding)
		{
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

	void UIScrollArea::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout.mAbsoluteRect);

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		if (withChildren)
			UpdateChildrenLayouts(true);

		CheckChildrenClipping();
		UpdateScrollParams();
		UpdateScrollBarsLayout();
	}

	void UIScrollArea::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		Vec2F widgetsMove(-delta.x, delta.y);
		for (auto child : mChilds)
			MoveWidgetAndCheckClipping(child, widgetsMove);

		UpdateScrollParams();
		UpdateScrollBarsLayout();
	}

	void UIScrollArea::MoveWidgetAndCheckClipping(UIWidget* widget, const Vec2F& delta)
	{
		widget->mBoundsWithChilds += delta;
		widget->mIsClipped = !widget->mBoundsWithChilds.IsIntersects(mAbsoluteClipArea);

		if (!widget->mIsClipped)
			widget->UpdateLayout(true, false);

		for (auto child : widget->mChilds)
			MoveWidgetAndCheckClipping(child, delta);
	}

	void UIScrollArea::UpdateScrollBarsLayout()
	{
		RectF tmpChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout(true);

		mChildsAbsRect = tmpChildsAbsRect;
	}

	void UIScrollArea::CheckChildrenClipping()
	{
		for (auto child : mChilds)
			child->CheckClipping(mAbsoluteClipArea);
	}

	void UIScrollArea::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		RectF newClipArea = clipArea.GetIntersection(mAbsoluteClipArea);

		for (auto child : mChilds)
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

		for (auto child : mChilds)
		{
			if (child->mFullyDisabled || child->GetType() == TypeOf(UIContextMenu))
				continue;

			mScrollArea.left   = Math::Min(mScrollArea.left, child->layout.mLocalRect.left);
			mScrollArea.bottom = Math::Min(mScrollArea.bottom, child->layout.mLocalRect.bottom);
			mScrollArea.right  = Math::Max(mScrollArea.right, child->layout.mLocalRect.right);
			mScrollArea.top    = Math::Max(mScrollArea.top, child->layout.mLocalRect.top);
		}
	}

	void UIScrollArea::UpdateScrollParams()
	{
		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		RectF localViewArea(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		CalculateScrollArea();

		mScrollRange = RectF(mScrollArea.left - localViewArea.left,
							 localViewArea.Height() - mScrollArea.top + localViewArea.bottom,
							 -(localViewArea.Width() - mScrollArea.right + localViewArea.left),
							 -mScrollArea.bottom + localViewArea.bottom);

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

	void UIScrollArea::OnSerialize(DataNode& node) const
	{
		if (mOwnHorScrollBar)
			*node.AddNode("mHorScrollBar") = mHorScrollBar;

		if (mOwnVerScrollBar)
			*node.AddNode("mVerScrollBar") = mVerScrollBar;
	}

	void UIScrollArea::OnDeserialized(const DataNode& node)
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar) delete mHorScrollBar;
			else                  mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar) delete mVerScrollBar;
			else                  mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		auto horScrollNode = node.GetNode("mHorScrollBar");
		mOwnHorScrollBar = horScrollNode != nullptr;
		if (mOwnHorScrollBar)
		{
			mHorScrollBar = *horScrollNode;
			mHorScrollBar->mParent = this;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		auto varScrollNode = node.GetNode("mVerScrollBar");
		mOwnVerScrollBar = varScrollNode != nullptr;
		if (mOwnVerScrollBar)
		{
			mVerScrollBar = *varScrollNode;
			mVerScrollBar->mParent = this;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		for (auto child : mChilds)
			child->layout.mDrivenByParent = true;

		UIWidget::OnDeserialized(node);
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
 
CLASS_META(o2::UIScrollArea)
{
	BASE_CLASS(o2::UIWidget);

	PUBLIC_FIELD(scroll);
	PUBLIC_FIELD(horScroll);
	PUBLIC_FIELD(verScroll);
	PUBLIC_FIELD(onScrolled);
	PROTECTED_FIELD(mHorScrollBar);
	PROTECTED_FIELD(mVerScrollBar);
	PROTECTED_FIELD(mOwnHorScrollBar);
	PROTECTED_FIELD(mOwnVerScrollBar);
	PROTECTED_FIELD(mViewAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteViewArea);
	PROTECTED_FIELD(mClipAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteClipArea);
	PROTECTED_FIELD(mScrollPos).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollSpeed);
	PROTECTED_FIELD(mScrollSpeedDamp).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollArea);
	PROTECTED_FIELD(mScrollRange);
	PROTECTED_FIELD(mEnableHorScroll);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetScroll, const Vec2F&);
	PUBLIC_FUNCTION(void, SetScrollForcible, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetScroll);
	PUBLIC_FUNCTION(RectF, GetScrollRange);
	PUBLIC_FUNCTION(void, ResetScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScroll, float);
	PUBLIC_FUNCTION(float, GetHorizontalScroll);
	PUBLIC_FUNCTION(void, SetVerticalScroll, float);
	PUBLIC_FUNCTION(float, GetVerticalScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScrollBar, UIHorizontalScrollBar*, bool);
	PUBLIC_FUNCTION(UIHorizontalScrollBar*, GetHorizontalScrollbar);
	PUBLIC_FUNCTION(void, SetVerticalScrollBar, UIVerticalScrollBar*, bool);
	PUBLIC_FUNCTION(UIVerticalScrollBar*, GetVerticalScrollbar);
	PUBLIC_FUNCTION(void, SetEnableScrollsHiding, bool);
	PUBLIC_FUNCTION(bool, IsScrollsHiding);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, SetViewLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetViewLayout);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
}
END_META;
 