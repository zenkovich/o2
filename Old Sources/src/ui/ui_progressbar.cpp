#include "ui_progressbar.h"

OPEN_O2_NAMESPACE

UIProgressBar::UIProgressBar(const Layout& layout, const String& id /*= ""*/):
UIDrawablesListWidget(layout, id), mBar(NULL)
{
	mMinValue = 0;
	mMaxValue = 1;
	SetValue(mMinValue);
	InitializeProperties();
}

UIProgressBar::UIProgressBar(const UIProgressBar& widget):
UIDrawablesListWidget(widget)
{
	if (widget.mBar)
		mBar = GetDrawable(widget.mBar->GetPathId());
	else
		mBar = NULL;

	mMinValue = widget.mMinValue;
	mMaxValue = widget.mMaxValue;
	SetValue(widget.mValue);

	InitializeProperties();
}

UIProgressBar::~UIProgressBar()
{
}

UIWidget* UIProgressBar::Clone() const
{
	return mnew UIProgressBar(*this);
}

UIDrawablesListWidget::Drawable* UIProgressBar::GetBarDrawable()
{
	return mBar;
}

void UIProgressBar::SetBarDrawable(Drawable* barDrawable)
{
	mBar = barDrawable;
}

void UIProgressBar::SetValueRange(float minValue, float maxValue)
{
	mMinValue = minValue;
	mMaxValue = maxValue;
	UpdateBarLayout();
}

void UIProgressBar::SetMinValue(float value)
{
	mMinValue = value;
	UpdateBarLayout();
}

void UIProgressBar::SetMaxValue(float value)
{
	mMaxValue = value;
	UpdateBarLayout();
}

float UIProgressBar::GetMinValue() const
{
	return mMinValue;
}

float UIProgressBar::GetMaxValue() const
{
	return mMaxValue;
}

void UIProgressBar::SetValue(float value)
{
	if (Equals(value, mValue))
		return;

	mValue = Clamp(value, mMinValue, mMaxValue);
	UpdateBarLayout();
}

float UIProgressBar::GetValue() const
{
	return mValue;
}

void UIProgressBar::LocalUpdate(float dt)
{
}

void UIProgressBar::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(UIProgressBar, minValue, SetMinValue, GetMinValue);
	REG_PROPERTY_SETTER_NONCONST(UIProgressBar, maxValue, SetMaxValue, GetMaxValue);
	REG_PROPERTY_SETTER_NONCONST(UIProgressBar, value, SetValue, GetValue);
}

void UIProgressBar::UpdateBarLayout()
{
	if (!mBar)
		return;

	mBar->SetLayout(Layout(Vec2F(), Vec2F(), Vec2F((mValue - mMinValue)/(mMaxValue - mMinValue), 1.0f)));
	mBar->UpdateLayout();
}

CLOSE_O2_NAMESPACE