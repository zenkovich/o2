#include "stdafx.h"
#include "WStringProperty.h"

#include "Scene/UI/Widgets/EditBox.h"

namespace Editor
{
	WStringProperty::WStringProperty()
	{}

	WStringProperty::WStringProperty(const WStringProperty& other):
		TPropertyField<WString>(other)
	{
		InitializeControls();
	}

	WStringProperty& WStringProperty::operator=(const WStringProperty& other)
	{
		TPropertyField<WString>::operator=(other);
		return *this;
	}

	void WStringProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<WString>::CopyData(otherActor);
		InitializeControls();
	}

	void WStringProperty::InitializeControls()
	{
		mEditBox = FindChildByType<EditBox>();
		if (mEditBox)
		{
			mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
			mEditBox->text = "--";
		}
	}

	void WStringProperty::UpdateValueView()
	{
		if (mValuesDifferent)
			mEditBox->text = "--";
		else
			mEditBox->text = mCommonValue;
	}

	void WStringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser(data);
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::WString>);

DECLARE_CLASS(Editor::WStringProperty);
