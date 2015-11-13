#include "ScrollArea.h"

#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"
#include "Utils/Debug.h"
#include "Utils/Time.h"

namespace o2
{
	IOBJECT_CPP(UIScrollArea);

	UIScrollArea::UIScrollArea():
		UIWidget(), mOwnHorScrollBar(false), mOwnVerScrollBar(false), mClipAreaLayout(Layout::Both()), mUnderCursor(false),
		mPressedCursor(false), mScrollSpeedDamp(7.0f), mSpeedUpdTime(0), mViewAreaLayoutOffBars(Layout::Both()),
		mLastHorScrollChangeTime(-10), mLastVerScrollChangeTime(-10), mEnableScrollsHiding(false), 
		mViewAreaLayoutHBar(Layout::Both()), mViewAreaLayoutHVBar(Layout::Both()), mViewAreaLayoutVBar(Layout::Both())
	{
		InitializeProperties();
	}

	UIScrollArea::UIScrollArea(const UIScrollArea& other):
		UIWidget(other), mOwnHorScrollBar(other.mOwnHorScrollBar), mOwnVerScrollBar(other.mOwnVerScrollBar),
		mClipAreaLayout(other.mClipAreaLayout), mScrollPos(other.mScrollPos), mUnderCursor(false), mPressedCursor(false),
		mScrollSpeedDamp(other.mScrollSpeedDamp), mSpeedUpdTime(0), mViewAreaLayoutOffBars(other.mViewAreaLayoutOffBars),
		mLastHorScrollChangeTime(-10), mLastVerScrollChangeTime(-10), mEnableScrollsHiding(other.mEnableScrollsHiding),
		mViewAreaLayoutHBar(other.mViewAreaLayoutHBar), mViewAreaLayoutHVBar(other.mViewAreaLayoutHVBar),
		mViewAreaLayoutVBar(other.mViewAreaLayoutVBar)
	{
		if (mOwnHorScrollBar)
		{
			mHorScrollBar = other.mHorScrollBar->Clone();
			mHorScrollBar->mParent = this;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else  mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->Clone();
			mVerScrollBar->mParent = this;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		UpdateScrollParams();
		UpdateLayout();

		InitializeProperties();
	}

	UIScrollArea::~UIScrollArea()
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar)
				mHorScrollBar.Release();
			else
				mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				mVerScrollBar.Release();
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
				mHorScrollBar.Release();
			else
				mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar)
				mVerScrollBar.Release();
			else
				mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		mClipAreaLayout = other.mClipAreaLayout;
		mViewAreaLayoutOffBars = other.mViewAreaLayoutOffBars;
		mViewAreaLayoutVBar = other.mViewAreaLayoutVBar;
		mViewAreaLayoutHBar = other.mViewAreaLayoutHBar;
		mViewAreaLayoutHVBar = other.mViewAreaLayoutHVBar;
		mScrollPos = other.mScrollPos;
		mOwnHorScrollBar = other.mOwnHorScrollBar;
		mOwnVerScrollBar = other.mOwnVerScrollBar;
		mScrollSpeedDamp = other.mScrollSpeedDamp;
		mEnableScrollsHiding = other.mEnableScrollsHiding;

		if (mOwnHorScrollBar)
		{
			mHorScrollBar = other.mHorScrollBar->Clone();
			mHorScrollBar->mParent = this;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		if (mOwnVerScrollBar)
		{
			mVerScrollBar = other.mVerScrollBar->Clone();
			mVerScrollBar->mParent = this;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;

		UpdateScrollParams();
		UpdateLayout();

		return *this;
	}

	void UIScrollArea::Draw()
	{
		if (!mVisible)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		o2Render.SetupScissorRect(mAbsoluteClipArea);
		o2Render.EnableScissorTest();

		for (auto child : mChilds)
			child->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();
	}

	void UIScrollArea::Update(float dt)
	{
		UIWidget::Update(dt);

		if (mOwnHorScrollBar)
			mHorScrollBar->Update(dt);

		if (mOwnVerScrollBar)
			mVerScrollBar->Update(dt);

		UpdateControls(dt);

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

			if (curTime - mLastHorScrollChangeTime > barsHideDelay && mHorScrollBar->IsVisible())
				mHorScrollBar->Hide();

			if (curTime - mLastVerScrollChangeTime > barsHideDelay && mVerScrollBar->IsVisible())
				mVerScrollBar->Hide();
		}
	}

	void UIScrollArea::SetScroll(const Vec2F& scroll)
	{
		Vec2F newScrollPos(Math::Clamp(scroll.x, mScrollRange.left, mScrollRange.right),
						   Math::Clamp(scroll.y, mScrollRange.bottom, mScrollRange.top));

		if (mHorScrollBar)
			mHorScrollBar->SetValueForcible(newScrollPos.x);
		else
			mScrollPos.x = newScrollPos.x;

		if (mVerScrollBar)
			mVerScrollBar->SetValueForcible(newScrollPos.y);
		else
			mScrollPos.y = newScrollPos.y;

		if (!mVerScrollBar || !mHorScrollBar)
		{
			UpdateLayout();
		}

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

	void UIScrollArea::ResetSroll()
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

	void UIScrollArea::SetHorizontalScrollBar(Ptr<UIHorizontalScrollBar> scrollbar, bool owner /*= true*/)
	{
		if (mHorScrollBar)
		{
			if (mOwnHorScrollBar) mHorScrollBar.Release();
			else                  mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		mHorScrollBar = scrollbar;
		mOwnHorScrollBar = owner;

		if (mHorScrollBar)
		{
			mHorScrollBar->mParent = this;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		UpdateScrollParams();
		UpdateLayout();
	}

	Ptr<UIHorizontalScrollBar> UIScrollArea::GetHorizontalScrollbar() const
	{
		return mHorScrollBar;
	}

	void UIScrollArea::SetVerticalScrollBar(Ptr<UIVerticalScrollBar> scrollbar, bool owner /*= true*/)
	{
		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar) mVerScrollBar.Release();
			else                  mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		mVerScrollBar = scrollbar;
		mOwnVerScrollBar = owner;

		if (mVerScrollBar)
		{
			mVerScrollBar->mParent = this;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		UpdateScrollParams();
		UpdateLayout();
	}

	Ptr<UIVerticalScrollBar> UIScrollArea::GetVerticalScrollbar() const
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
		mViewAreaLayoutOffBars = viewLayout;
		mViewAreaLayoutHBar = viewLayout;
		mViewAreaLayoutVBar = viewLayout;
		mViewAreaLayoutHVBar = viewLayout;

		UpdateLayout();
	}

	void UIScrollArea::SetViewLayout(const Layout& viewLayoutOffBars, const Layout& viewLayoutHBars, 
									 const Layout& viewLayoutVBars, const Layout& viewLayoutHVBars)
	{
		mViewAreaLayoutOffBars = viewLayoutOffBars;
		mViewAreaLayoutHBar = viewLayoutHBars;
		mViewAreaLayoutVBar = viewLayoutVBars;
		mViewAreaLayoutHVBar = viewLayoutHVBars;

		UpdateLayout();
	}

	Layout UIScrollArea::GetViewLayout() const
	{
		return mViewAreaLayoutOffBars;
	}

	Layout UIScrollArea::GetViewLayoutOffBars() const
	{
		return mViewAreaLayoutOffBars;
	}

	Layout UIScrollArea::GetViewLayoutHBar() const
	{
		return mViewAreaLayoutHBar;
	}

	Layout UIScrollArea::GetViewLayoutVBar() const
	{
		return mViewAreaLayoutVBar;
	}

	Layout UIScrollArea::GetViewLayoutHVBar() const
	{
		return mViewAreaLayoutHVBar;
	}

	void UIScrollArea::OnChildAdded(Ptr<UIWidget> child)
	{
		child->layout.mDrivenByParent = true;
	}

	void UIScrollArea::OnChildRemoved(Ptr<UIWidget> child)
	{
		child->layout.mDrivenByParent = false;
	}

	void UIScrollArea::UpdateControls(float dt)
	{
		auto cursor = o2Input.GetCursor(0);
		bool underCursorAtFrame = layout.mAbsoluteRect.IsInside(cursor->mPosition);
		bool underClippingArea = mAbsoluteClipArea.IsInside(cursor->mPosition);
		bool underScrollbars = 
			((mHorScrollBar && mOwnHorScrollBar) ? mHorScrollBar->IsUnderPoint(cursor->mPosition):false) ||
			((mVerScrollBar && mOwnVerScrollBar) ? mVerScrollBar->IsUnderPoint(cursor->mPosition):false);

		bool lastPressedCursor = mPressedCursor;

		if (mEnableScrollsHiding)
		{
			if (mHorScrollBar->IsUnderPoint(cursor->mPosition))
			{
				mLastHorScrollChangeTime = o2Time.GetApplicationTime();

				if (!mHorScrollBar->IsVisible())
					mHorScrollBar->Show();
			}

			if (mVerScrollBar->IsUnderPoint(cursor->mPosition))
			{
				mLastVerScrollChangeTime = o2Time.GetApplicationTime();

				if (!mVerScrollBar->IsVisible())
					mVerScrollBar->Show();
			}
		}

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

		if (cursor->mPressed && Math::Equals(cursor->mPressedTime, 0.0f) && underClippingArea && !mPressedCursor && !underScrollbars)
		{
			mPressedCursor = true;
			auto selectState = state["pressed"];
			if (selectState)
				*selectState = true;

			mPressedScroll = mScrollPos;
			mPressedCursorPos = cursor->mPosition;
		}

		if (!cursor->mPressed && mPressedCursor && mPressedCursor)
		{
			mPressedCursor = false;
			auto selectState = state["pressed"];
			if (selectState)
				*selectState = false;
		}

		if (!Math::Equals(o2Input.GetMouseWheelDelta(), 0.0f) && underClippingArea && !underScrollbars)
		{
			mScrollSpeed = Vec2F();

			if (mVerScrollBar && !Math::Equals(mScrollRange.top, mScrollRange.bottom))
				mVerScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
			else if (mHorScrollBar && !Math::Equals(mScrollRange.left, mScrollRange.right))
				mHorScrollBar->OnScrolled(o2Input.GetMouseWheelDelta());
		}

		if (lastPressedCursor && mPressedCursor)
		{
			if (!Math::Equals(mScrollRange.left, mScrollRange.right) || !Math::Equals(mScrollRange.top, mScrollRange.bottom))
			{

				Vec2F delta = cursor->mPosition - mPressedCursorPos;
				delta.x = -delta.x;

				if (delta != Vec2F())
				{
					o2Events.BreakCursorEvent();

					Vec2F newScroll = mPressedScroll + delta;
					Vec2F scrollDelta = newScroll - mScrollPos;

					if (scrollDelta.Length() > 7.0f)
					{
						mScrollSpeed = scrollDelta/dt;
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

	void UIScrollArea::UpdateLayout(bool forcible /*= false*/)
	{
		if (layout.mDrivenByParent && !forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mAbsoluteViewArea = mActualViewLayout.Calculate(layout.mAbsoluteRect);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout.mAbsoluteRect);
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));

		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChilds)
			child->UpdateLayout(true);

		UpdateScrollParams();

		RectF _mChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout();

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout();

		mChildsAbsRect = _mChildsAbsRect;
	}

	void UIScrollArea::UpdateScrollParams()
	{
		mAbsoluteViewArea = mActualViewLayout.Calculate(layout.mAbsoluteRect);
		RectF localViewArea(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		mScrollArea = RectF(0.0f, 0.0f, localViewArea.Width(), localViewArea.Height());

		for (auto child : mChilds)
		{
			mScrollArea.left   = Math::Min(mScrollArea.left, child->layout.mLocalRect.left);
			mScrollArea.bottom = Math::Min(mScrollArea.bottom, child->layout.mLocalRect.bottom);
			mScrollArea.right  = Math::Max(mScrollArea.right, child->layout.mLocalRect.right);
			mScrollArea.top    = Math::Max(mScrollArea.top, child->layout.mLocalRect.top);
		}

		mScrollRange = RectF(mScrollArea.left - localViewArea.left,
							 localViewArea.Height() - mScrollArea.top + localViewArea.bottom,
							 -(localViewArea.Width() - mScrollArea.right + localViewArea.left),
							 -mScrollArea.bottom + localViewArea.bottom);

		if (mHorScrollBar)
		{
			if (Math::Equals(mScrollRange.left, mScrollRange.right))
				mHorScrollBar->Hide();
			else
			{
				mHorScrollBar->Show();
				mHorScrollBar->SetValueRange(mScrollRange.left, mScrollRange.right);
				mHorScrollBar->SetScrollHandleSize(localViewArea.Width());
			}
		}

		if (mVerScrollBar)
		{
			if (Math::Equals(mScrollRange.bottom, mScrollRange.top))
				mVerScrollBar->Hide();
			else
			{
				mVerScrollBar->Show();
				mVerScrollBar->SetValueRange(mScrollRange.bottom, mScrollRange.top);
				mVerScrollBar->SetScrollHandleSize(localViewArea.Height());
			}
		}

		bool enabledHorBar = !Math::Equals(mScrollRange.left, mScrollRange.right);
		bool enabledVerBar = !Math::Equals(mScrollRange.top, mScrollRange.bottom);

		Layout lastActialLayout = mActualViewLayout;

		mActualViewLayout = mViewAreaLayoutOffBars;
		if (enabledHorBar && !enabledVerBar)
			mActualViewLayout = mViewAreaLayoutHBar;
		else if (!enabledHorBar && enabledVerBar)
			mActualViewLayout = mViewAreaLayoutVBar;
		else if (enabledHorBar && enabledVerBar)
			mActualViewLayout = mViewAreaLayoutHVBar;

		if (mActualViewLayout != lastActialLayout)
			UpdateScrollParams();
	}

	void UIScrollArea::OnHorScrollChanged(float value)
	{
		if (!mHorScrollBar->IsVisible())
			mHorScrollBar->Show();

		mLastHorScrollChangeTime = o2Time.GetApplicationTime();

		Vec2F delta(Math::Clamp(value, mScrollRange.left, mScrollRange.right) - mScrollPos.x, 0.0f);
		mScrollPos += delta;
		UpdateLayout();

		onScrolled(mScrollPos);
		OnScrolled();
	}

	void UIScrollArea::OnVerScrollChanged(float value)
	{
		if (!mVerScrollBar->IsVisible())
			mVerScrollBar->Show();

		mLastVerScrollChangeTime = o2Time.GetApplicationTime();

		Vec2F delta(0.0f, Math::Clamp(value, mScrollRange.bottom, mScrollRange.top) - mScrollPos.y);
		mScrollPos += delta;
		UpdateLayout();

		onScrolled(mScrollPos);
		OnScrolled();
	}

	void UIScrollArea::OnSerialize(DataNode& node)
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
			if (mOwnHorScrollBar) mHorScrollBar.Release();
			else                  mHorScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}

		if (mVerScrollBar)
		{
			if (mOwnVerScrollBar) mVerScrollBar.Release();
			else                  mVerScrollBar->onSmoothChange -= Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}

		auto horScrollNode = node.GetNode("mHorScrollBar");
		mOwnHorScrollBar = horScrollNode.IsValid();
		if (mOwnHorScrollBar)
		{
			mHorScrollBar = *horScrollNode;
			mHorScrollBar->mParent = this;
			mHorScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnHorScrollChanged);
		}
		else mHorScrollBar = nullptr;

		auto varScrollNode = node.GetNode("mHorScrollBar");
		mOwnVerScrollBar = varScrollNode.IsValid();
		if (mOwnVerScrollBar)
		{
			mVerScrollBar = *varScrollNode;
			mVerScrollBar->mParent = this;
			mVerScrollBar->onSmoothChange += Function<void(float)>(this, &UIScrollArea::OnVerScrollChanged);
		}
		else mVerScrollBar = nullptr;
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