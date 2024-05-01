#include "o2Editor/stdafx.h"
#include "StringProperty.h"

#include "o2/Scene/UI/Widgets/EditBox.h"

namespace Editor
{
	StringProperty::StringProperty(RefCounter* refCounter):
		TPropertyField<String>(refCounter)
	{}

	StringProperty::StringProperty(RefCounter* refCounter, const StringProperty& other) :
		TPropertyField<String>(refCounter, other)
	{
		InitializeControls();
	}

	StringProperty& StringProperty::operator=(const StringProperty& other)
	{
		TPropertyField<String>::operator=(other);
		InitializeControls();
		return *this;
	}

	void StringProperty::InitializeControls()
	{
		mEditBox = FindChildByType<EditBox>();
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

DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::String>);
DECLARE_TEMPLATE_CLASS(o2::ActorRef<Editor::StringProperty>);
DECLARE_TEMPLATE_CLASS(o2::ActorRef<Editor::TPropertyField<o2::String>>);
// --- META ---

DECLARE_CLASS(Editor::StringProperty, Editor__StringProperty);
// --- END META ---
