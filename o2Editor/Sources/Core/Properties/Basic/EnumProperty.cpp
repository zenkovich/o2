#include "stdafx.h"
#include "EnumProperty.h"

#include "Scene/UI/DropDown.h"


namespace Editor
{
	EnumProperty::EnumProperty()
	{}

	EnumProperty::EnumProperty(const EnumProperty& other) :
		TPropertyField<int>(other)
	{
		InitializeControls();
	}

	EnumProperty& EnumProperty::operator=(const EnumProperty& other)
	{
		TPropertyField<int>::operator=(other);
		return *this;
	}

	void EnumProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<int>::CopyData(otherActor);
		InitializeControls();
	}

	void EnumProperty::InitializeControls()
	{
		mDropDown = FindChildByType<UIDropDown>();
		if (mDropDown)
		{
			mDropDown->onSelectedText = THIS_FUNC(OnSelectedItem);
			mDropDown->SetState("undefined", true);
		}
	}

	const Type* EnumProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void EnumProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Property)
			mEnumType = dynamic_cast<const EnumType*>(((const PropertyType*)type)->GetValueType());
		else
			mEnumType = dynamic_cast<const EnumType*>(type);

		if (mEnumType)
		{
			mEntries = &mEnumType->GetEntries();

			for (auto kv : *mEntries)
				mDropDown->AddItem(kv.Value());
		}
	}

	void EnumProperty::UpdateValueView()
	{
		mUpdatingValue = true;

		if (mValuesDifferent)
		{
			mDropDown->value = (*mEntries)[mCommonValue];
			mDropDown->SetState("undefined", true);
		}
		else
		{
			mDropDown->value = (*mEntries)[mCommonValue];
			mDropDown->SetState("undefined", false);
		}

		mUpdatingValue = false;
	}

	void EnumProperty::OnSelectedItem(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValueByUser(mEntries->FindValue(name).Key());
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<int>);

DECLARE_CLASS(Editor::EnumProperty);
