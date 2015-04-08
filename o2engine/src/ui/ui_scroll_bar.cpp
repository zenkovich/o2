#include "ui_scroll_bar.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIScrollBar);

UIScrollBar::UIScrollBar(const Layout& layout, const String& id /*= ""*/, Type type /*= TP_HORISONTAL*/):
UIDrawablesListWidget(layout, id), mBarHoverState(NULL), mBarPressedState(NULL), mType(type), mBar(NULL),
mBarGeometry(layout::both()), mBackgrGeometry(layout::both())
{
	mMinValue = 0;
	mMaxValue = 1;
	SetValue(mMinValue);
	SetBarSize(mMaxValue);

	InitializeProperties();
}

UIScrollBar::UIScrollBar(const UIScrollBar& scrollbar):
UIDrawablesListWidget(scrollbar)
{
	if (scrollbar.mBar)
		mBar = GetDrawable(scrollbar.mBar->GetPathId());
	else
		mBar = NULL;

	mBarHoverState = GetState("hover");
	mBarPressedState = GetState("pressed");

	mType = scrollbar.mType;
	mMinValue = scrollbar.mMinValue;
	mMaxValue = scrollbar.mMaxValue;
	mBarSize = scrollbar.mBarSize;
	mBarGeometry = scrollbar.mBarGeometry;
	mBackgrGeometry = scrollbar.mBackgrGeometry;
	SetValue(scrollbar.mValue);

	InitializeProperties();
}

UIScrollBar::~UIScrollBar()
{
}

UIWidget* UIScrollBar::Clone() const
{
	return mnew UIScrollBar(*this);
}

void UIScrollBar::SetBarDrawable(Drawable* drawable)
{
	mBar = drawable;
}

UIDrawablesListWidget::Drawable* UIScrollBar::GetBarDrawable()
{
	return mBar;
}

void UIScrollBar::SetValueRange(float minValue, float maxValue)
{
	if (Equals(minValue, mMinValue) && Equals(maxValue, mMaxValue))
		return;

	mMinValue = minValue;
	mMaxValue = maxValue;
	this->minValue.onChangeEvent.Invoke();
	this->maxValue.onChangeEvent.Invoke();
	onValueChangedEvent.Invoke();
	UpdateBarLayout();
}

void UIScrollBar::SetMinValue(float value)
{
	if (Equals(minValue, mMinValue))
		return;

	mMinValue = value;
	UpdateBarLayout();
	onValueChangedEvent.Invoke();
	this->minValue.onChangeEvent.Invoke();
}

void UIScrollBar::SetMaxValue(float value)
{
	if (Equals(maxValue, mMaxValue))
		return;

	mMaxValue = value;
	UpdateBarLayout();
	onValueChangedEvent.Invoke();
	this->maxValue.onChangeEvent.Invoke();
}

float UIScrollBar::GetMinValue() const
{
	return mMinValue;
}

float UIScrollBar::GetMaxValue() const
{
	return mMaxValue;
}

void UIScrollBar::SetValue(float value)
{
	if (Equals(value, mValue))
		return;

	mValue = value;
	onValueChangedEvent.Invoke();
	UpdateBarLayout();
	this->value.onChangeEvent.Invoke();
}

void UIScrollBar::SetValueClamped(float value)
{
	SetValue(Clamp(value, mMinValue, mMaxValue));
}

float UIScrollBar::GetValue() const
{
	return mValue;
}

void UIScrollBar::SetBarSize(float size)
{
	mBarSize = size;
	UpdateBarLayout();
	this->barSize.onChangeEvent.Invoke();
}

float UIScrollBar::GetBarSize() const
{
	return mBarSize;
}

void UIScrollBar::GetBarGeometryLayout(const layout& layout)
{
	mBarGeometry = layout;
}

void UIScrollBar::SetBackgroundGeometryLayout(const layout& layout)
{
	mBackgrGeometry = layout;
}

void UIScrollBar::LocalUpdate(float dt)
{
	if (mBarHoverState)
		mBarHoverState->SetState(mHover && mCursorInside);
}

bool UIScrollBar::LocalProcessInputMessage(const InputMessage& msg)
{
	bool res = false;
	RectF barRect = mBarGeometry.GetRect();
	Vec2F cursorPos = msg.GetCursorPos();
	mHover = barRect.isInside(msg.GetCursorPos());

	if (msg.IsCursorPressed())
	{
		if (barRect.isInside(cursorPos))
		{
			mPressed = true;

			if (mBarPressedState)
				mBarPressedState->SetState(mPressed);

		}
		else if (mBackgrGeometry.GetRect().isInside(cursorPos) && mFocused)
		{
			bool cursorUpper = (mType == TP_HORISONTAL) ? cursorPos.x < barRect.left:cursorPos.y < barRect.top;
			bool cursorLower = (mType == TP_HORISONTAL) ? cursorPos.x > barRect.right:cursorPos.y > barRect.down;

			if (cursorUpper)
				SetValue(Clamp(mValue - mBarSize, mMinValue, mMaxValue));

			if (cursorLower)
				SetValue(Clamp(mValue + mBarSize, mMinValue, mMaxValue));
		}

		if (mCursorInside)
			MakeFocused();
		else
			ReleaseFocus();

		res = mCursorInside;
	}
	else if (msg.IsCursorDown() && mFocused)
	{
		if (mPressed)
		{
			float delta = (mType == TP_HORISONTAL) ? msg.GetCursorDelta().x:msg.GetCursorDelta().y;
			float length = (mType == TP_HORISONTAL) ? mBasicDrawable.GetLayout().getRect().width():
				mBasicDrawable.GetLayout().getRect().height();

			float range = mMaxValue - mMinValue;
			length = length - (mBarSize/range)*length;
			SetValue(Clamp(mValue + delta/length*range, mMinValue, mMaxValue));
		}
		else if (msg.GetCursorDelta().length() > 1.0f)
		{
			bool cursorUpper = (mType == TP_HORISONTAL) ? cursorPos.x < barRect.left:cursorPos.y < barRect.top;
			bool cursorLower = (mType == TP_HORISONTAL) ? cursorPos.x > barRect.right:cursorPos.y > barRect.down;

			if (cursorUpper)
				SetValue(Clamp(mValue - mBarSize, mMinValue, mMaxValue));

			if (cursorLower)
				SetValue(Clamp(mValue + mBarSize, mMinValue, mMaxValue));
		}

		res = true;
	}
	else if (msg.IsCursorReleased() && mPressed)
	{
		mPressed = false;

		if (mBarPressedState)
			mBarPressedState->SetState(mPressed);
	}

	return res;
}

bool UIScrollBar::IsLocalInside(const Vec2F& point) const
{
	return mBackgrGeometry.GetRect().isInside(point);
}

void UIScrollBar::UpdateBarLayout()
{
	if (!mBar)
		return;

	float range = mMaxValue - mMinValue;
	float locRange = Max(range - mBarSize, 0.00001f);
	float relBarSize = mBarSize/range;
	float topCoef = Clamp((mValue - mMinValue)/range*(1.0f - relBarSize), 0.0f, 1.0f);
	float bottomCoef = Clamp(topCoef + relBarSize, topCoef, 1.0f);

	if (mType == TP_HORISONTAL)
		mBar->SetLayout(Layout(Vec2F(topCoef, 0.0f), Vec2F(), Vec2F(bottomCoef, 1.0f)));
	else
		mBar->SetLayout(Layout(Vec2F(0.0f, topCoef), Vec2F(), Vec2F(1.0f, bottomCoef)));

	mBar->UpdateLayoutManual(mGlobalPosition, mSize);
	mBarGeometry.Update(mBar->GetLayout().mPosition, mBar->GetLayout().mSize);
	mBackgrGeometry.Update(mGlobalPosition, mSize);
}

void UIScrollBar::LayoutUpdated()
{
	UpdateBarLayout();
	UIDrawablesListWidget::LayoutUpdated();
}

void UIScrollBar::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(UIScrollBar, minValue, SetMinValue, GetMinValue);
	REG_PROPERTY_SETTER_NONCONST(UIScrollBar, maxValue, SetMaxValue, GetMaxValue);
	REG_PROPERTY_SETTER_NONCONST(UIScrollBar, value, SetValue, GetValue);
	REG_PROPERTY_SETTER_NONCONST(UIScrollBar, barSize, SetBarSize, GetBarSize);
}

CLOSE_O2_NAMESPACE