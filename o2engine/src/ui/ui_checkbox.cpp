#include "ui_checkbox.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UICheckBox);

UICheckBox::UICheckBox( const Layout& layout, const String& id /*= ""*/ ):
	UIButton(layout, id), mCheckedState(NULL), mChecked(false)
{
	InitializeProperties();
}

UICheckBox::UICheckBox( const UICheckBox& checkbox ):
	UIButton(checkbox)
{
	mCheckedState = GetState("checked");
	onCheckOn = checkbox.onCheckOn;
	onCheckOff = checkbox.onCheckOff;

	SetChecked(checkbox.mChecked);
}

UICheckBox::~UICheckBox()
{
}

UIWidget* UICheckBox::Clone() const
{
	return mnew UICheckBox(*this);
}

void UICheckBox::SetChecked( bool checked )
{
	if (mChecked == checked)
		return;

	this->checked.onChangeEvent.Invoke();

	mChecked = checked;
	if (mCheckedState)
		mCheckedState->SetState(mChecked);
}

bool UICheckBox::IsChecked() const
{
	return mChecked;
}

void UICheckBox::OnClicked()
{
	SetChecked(!mChecked);
}

void UICheckBox::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(UICheckBox, checked, SetChecked, IsChecked);
}

CLOSE_O2_NAMESPACE