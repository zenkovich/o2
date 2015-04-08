#include "ui_scrollarea.h"

#include "render_system/render_system.h"
#include "ui_scroll_bar.h"
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIScrollArea);

UIScrollArea::UIScrollArea(const Layout& layout, UIScrollBar* horBarSample /*= NULL*/,
						   UIScrollBar* verBarSample /*= NULL*/, const String& id /*= ""*/):
						   UIDrawablesListWidget(layout, id), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
	SetHorScrollbar(horBarSample);
	SetVerScrollbar(verBarSample);

	InitializeProperties();
}

UIScrollArea::UIScrollArea(const UIScrollArea& scrollarea):
UIDrawablesListWidget(scrollarea), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
	if (scrollarea.mHorScrollbar)
		SetHorScrollbar(GetWidgetByType<UIScrollBar>(scrollarea.mHorScrollbar->GetId()));
	else
		SetHorScrollbar(NULL);

	if (scrollarea.mVerScrollbar)
		SetVerScrollbar(GetWidgetByType<UIScrollBar>(scrollarea.mVerScrollbar->GetId()));
	else
		SetVerScrollbar(NULL);

	mClippingLayout = scrollarea.mClippingLayout;

	InitializeProperties();
}

UIScrollArea::~UIScrollArea()
{
}

UIWidget* UIScrollArea::Clone() const
{
	return mnew UIScrollArea(*this);
}

void UIScrollArea::SetHorScrollbar(UIScrollBar* scrollbar)
{
	if (mHorScrollbar)
		RemoveChild(mHorScrollbar);

	if (!scrollbar)
		return;

	AddChild(scrollbar);
	mHorScrollbar = scrollbar;
	mHorScrollbar->onValueChangedEvent.Add(callback<UIScrollArea>(this, &UIScrollArea::ScrollChanged));
}

void UIScrollArea::SetVerScrollbar(UIScrollBar* scrollbar)
{
	if (mVerScrollbar)
		RemoveChild(mVerScrollbar);

	if (!scrollbar)
		return;

	AddChild(scrollbar);
	mVerScrollbar = scrollbar;
	mVerScrollbar->onValueChangedEvent.Add(callback<UIScrollArea>(this, &UIScrollArea::ScrollChanged));
}

void UIScrollArea::Draw()
{
	if (!mVisible || mDrawedAtFrame == AppTimer()->GetCurrentFrame())
		return;

	mDrawedAtFrame = AppTimer()->GetCurrentFrame();

	LocalDraw();

	if (mHorScrollbar)
		mHorScrollbar->mDrawedAtFrame = mDrawedAtFrame;

	if (mVerScrollbar)
		mVerScrollbar->mDrawedAtFrame = mDrawedAtFrame;

	AppRender()->SetupScissorRect(mClippingLayout.GetRect());
	AppRender()->EnableScissorTest();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->Draw();

	AppRender()->DisableScissorTest();

	if (mHorScrollbar)
	{
		mHorScrollbar->mDrawedAtFrame = mDrawedAtFrame - 1;
		mHorScrollbar->Draw();
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->mDrawedAtFrame = mDrawedAtFrame - 1;
		mVerScrollbar->Draw();
	}
}

bool UIScrollArea::IsFocusable() const
{
	return true;
}

void UIScrollArea::UpdateLayout()
{
	LocalUpdateLayout();
	mClippingLayout.Update(mGlobalPosition, mSize);

	Vec2F tempGlobPos = mGlobalPosition;
	mGlobalPosition = mClippingLayout.mPosition - mSmoothScroll;

	RectF contentSize = mClippingLayout.GetRect() - mSmoothScroll;
	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		(*it)->UpdateLayout();

		if (*it == mHorScrollbar || *it == mVerScrollbar)
			continue;

		contentSize = contentSize + (*it)->mBounds;
	}

	mGlobalPosition = tempGlobPos;

	contentSize.right += 5.0f;
	contentSize.down += 5.0f;

	if (mHorScrollbar)
	{
		mHorScrollbar->SetValueRange(0.0f, Max(contentSize.getSizeX() - mClippingLayout.mSize.x, 0.1f));
		mHorScrollbar->SetBarSize(mClippingLayout.mSize.x/contentSize.getSizeX()*mHorScrollbar->GetMaxValue());
		mHorScrollbar->UpdateLayout();
		mHorScrollbar->SetVisible(mHorScrollbar->GetBarSize() < mHorScrollbar->GetMaxValue() - mHorScrollbar->GetMinValue());
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->SetValueRange(0.0f, Max(contentSize.getSizeY() - mClippingLayout.mSize.y, 0.1f));
		mVerScrollbar->SetBarSize(mClippingLayout.mSize.y/contentSize.getSizeY()*mVerScrollbar->GetMaxValue());
		mVerScrollbar->UpdateLayout();
		mVerScrollbar->SetVisible(mVerScrollbar->GetBarSize() < mVerScrollbar->GetMaxValue() - mVerScrollbar->GetMinValue());
	}

}

void UIScrollArea::LocalUpdate(float dt)
{
	float scrollingCoef = 20.0f;
	Vec2F lastSmoothScroll = mSmoothScroll;
	mSmoothScroll = Lerp(mSmoothScroll, mScroll, Clamp(dt*scrollingCoef, 0.0f, 1.0f));

	if (!Equals(lastSmoothScroll, mSmoothScroll))
	{
		UpdateLayout();
		onScrolled.Invoke();
	}
}

bool UIScrollArea::LocalProcessInputMessage(const InputMessage& msg)
{
	bool insideClipping = mClippingLayout.GetRect().isInside(msg.GetCursorPos());

	float scrollCoef = 0.1f;
	float mouseWheelDelta = msg.GetMouseWheelDelta()*scrollCoef;
	if (!Equals(mouseWheelDelta, 0.0f))
	{
		if (mVerScrollbar)
			mVerScrollbar->SetValueClamped(mVerScrollbar->GetValue() - mouseWheelDelta);
		else if (mHorScrollbar)
			mHorScrollbar->SetValueClamped(mHorScrollbar->GetValue() - mouseWheelDelta);
	}

	if (insideClipping)
		return false;

	if (mHorScrollbar)
		mHorScrollbar->ProcessInputMessage(msg);

	if (mVerScrollbar)
		mVerScrollbar->ProcessInputMessage(msg);

	return true;
}

void UIScrollArea::OnFocused()
{
	UIDrawablesListWidget::OnFocused();
}

void UIScrollArea::OnFocusLost()
{
	UIDrawablesListWidget::OnFocusLost();
}

void UIScrollArea::SetScroll(const Vec2F& scroll)
{
	mScroll = scroll;
}

Vec2F UIScrollArea::GetScroll() const
{
	return mScroll;
}

void UIScrollArea::ScrollChanged()
{
	if (mHorScrollbar)
		mScroll.x = mHorScrollbar->GetValue();

	if (mVerScrollbar)
		mScroll.y = mVerScrollbar->GetValue();

	UpdateLayout();
}

void UIScrollArea::InitializeProperties()
{
	REG_PROPERTY(UIScrollArea, scroll, SetScroll, GetScroll);
}


CLOSE_O2_NAMESPACE