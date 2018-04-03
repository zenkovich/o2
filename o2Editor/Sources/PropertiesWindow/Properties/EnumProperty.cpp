#include "stdafx.h"
#include "EnumProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	EnumProperty::EnumProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("enum property");

		mDropDown = mPropertyWidget->GetChildByType<UIDropDown>();
		mDropDown->onSelectedText = THIS_FUNC(OnSelectedItem);
		mDropDown->SetState("undefined", true);

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	EnumProperty::~EnumProperty()
	{
		delete mPropertyWidget;
	}

	void EnumProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void EnumProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = GetProxy<int>(mValuesProxies[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			if (newCommonValue != GetProxy<int>(mValuesProxies[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void EnumProperty::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<int>(ptr.first, GetProxy<int>(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* EnumProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	int EnumProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool EnumProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void EnumProperty::SetValue(int value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy<int>(ptr.first, value);

		SetCommonValue(value);
	}

	void EnumProperty::SetUnknownValue()
	{
		mCommonValue = 0;
		mValuesDifferent = true;

		mUpdatingValue = true;
		mDropDown->SelectItemAt(-1);
		mUpdatingValue = false;

		mDropDown->SetState("undefined", true);

		OnChanged();
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

	void EnumProperty::SetCommonValue(int value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		mUpdatingValue = true;
		mDropDown->value = (*mEntries)[mCommonValue];
		mUpdatingValue = false;

		mDropDown->SetState("undefined", false);

		OnChanged();
	}

	void EnumProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<int>(ptr.first), GetProxy<int>(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void EnumProperty::OnSelectedItem(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValueByUser(mEntries->FindValue(name).Key());
	}

	void EnumProperty::SetValueByUser(int value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void EnumProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void EnumProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<int>(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::EnumProperty);
