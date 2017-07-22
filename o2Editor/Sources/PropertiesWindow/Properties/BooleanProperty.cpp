#include "BooleanProperty.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"

namespace Editor
{
	BooleanProperty::BooleanProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("boolean property");

		mToggle = mPropertyWidget->FindChild<UIToggle>();

		if (!mToggle)
			mToggle = dynamic_cast<UIToggle*>(mPropertyWidget);

		mToggle->layout.minHeight = 10;
		mToggle->onToggleByUser = THIS_FUNC(SetValueByUser);
		mToggle->SetValueUnknown();

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	BooleanProperty::~BooleanProperty()
	{
		delete mPropertyWidget;
	}

	void BooleanProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((Property<bool>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<bool>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((bool*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((bool*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void BooleanProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue(newCommonValue);
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void BooleanProperty::Revert()
	{
		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				mAssignFunc(ptr.first, mGetFunc(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* BooleanProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	bool BooleanProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool BooleanProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void BooleanProperty::SetValue(bool value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void BooleanProperty::SetUnknownValue(bool defaultValue /*= false*/)
	{
		mValuesDifferent = true;
		mToggle->value = defaultValue;
		mToggle->SetValueUnknown();
		mCommonValue = defaultValue;

		OnChanged();
	}

	const Type* BooleanProperty::GetFieldType() const
	{
		return &TypeOf(bool);
	}

	void BooleanProperty::SetCommonValue(bool value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mToggle->value = value;

		OnChanged();
	}

	void BooleanProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second && !Math::Equals(mGetFunc(ptr.first), mGetFunc(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void BooleanProperty::SetValueByUser(bool value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void BooleanProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void BooleanProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::BooleanProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mToggle);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(bool, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, bool);
	PUBLIC_FUNCTION(void, SetUnknownValue, bool);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, bool);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, SetValueByUser, bool);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
