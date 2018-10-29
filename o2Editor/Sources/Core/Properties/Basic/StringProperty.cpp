#include "stdafx.h"
#include "StringProperty.h"

#include "UI/EditBox.h"

namespace Editor
{
	StringProperty::StringProperty()
	{}

	StringProperty::StringProperty(const StringProperty& other) :
		TPropertyField<String>(other)
	{
		InitializeControls();
	}

	StringProperty& StringProperty::operator=(const StringProperty& other)
	{
		TPropertyField<String>::operator=(other);
		return *this;
	}

	void StringProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<String>::CopyData(otherActor);
		InitializeControls();
	}

	void StringProperty::InitializeControls()
	{
		mEditBox = FindChildByType<UIEditBox>();
		if (mEditBox)
		{
			mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
			mEditBox->text = "--";
		}
	}

	void StringProperty::UpdateValueView()
	{
		if (mValuesDifferent)
			mEditBox->text = "--";
		else
			mEditBox->text = mCommonValue;
	}

	void StringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser(data);
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::String>);

DECLARE_CLASS(Editor::StringProperty);
