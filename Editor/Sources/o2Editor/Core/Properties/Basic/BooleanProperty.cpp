#include "o2Editor/stdafx.h"
#include "BooleanProperty.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Toggle.h"


namespace Editor
{
	BooleanProperty::BooleanProperty()
	{}

	BooleanProperty::BooleanProperty(const BooleanProperty& other) :
		TPropertyField<bool>(other)
	{
		InitializeControls();
	}

	BooleanProperty& BooleanProperty::operator=(const BooleanProperty& other)
	{
		TPropertyField<bool>::operator=(other);
		return *this;
	}

	void BooleanProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<bool>::CopyData(otherActor);
		InitializeControls();
	}

	void BooleanProperty::InitializeControls()
	{
		mToggle = FindChildByType<Toggle>();
		if (mToggle)
		{
			mToggle->layout->minHeight = 10;
			mToggle->onToggleByUser = [&](bool value) { SetValueByUser(value); };
			mToggle->SetValueUnknown();
		}
	}

	void BooleanProperty::UpdateValueView()
	{
		mToggle->value = mCommonValue;
		if (mValuesDifferent)
			mToggle->SetValueUnknown();
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<bool>);

DECLARE_CLASS(Editor::BooleanProperty);
