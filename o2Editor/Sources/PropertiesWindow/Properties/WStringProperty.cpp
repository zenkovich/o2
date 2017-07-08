#include "WStringProperty.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	WStringProperty::WStringProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->FindChild<UIEditBox>();
		if (!mEditBox)
			mEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget);

		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &WStringProperty::OnEdited);
		mEditBox->text = "--";

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = Function<void()>(this, &WStringProperty::Revert);
	}

	WStringProperty::~WStringProperty()
	{
		delete mPropertyWidget;
	}

	void WStringProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const WString& value) { *((Property<WString>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<WString>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const WString& value) { *((WString*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((WString*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void WStringProperty::Refresh()
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
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	void WStringProperty::Revert()
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

	UIWidget* WStringProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	WString WStringProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool WStringProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void WStringProperty::SetValue(const WString& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void WStringProperty::SetUnknownValue()
	{
		mCommonValue = "";
		mValuesDifferent = true;
		mEditBox->text = "--";

		OnChanged();
	}

	const Type* WStringProperty::GetFieldType() const
	{
		return &TypeOf(WString);
	}

	void WStringProperty::SetCommonValue(const WString& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mEditBox->text = value;

		OnChanged();
	}

	void WStringProperty::CheckRevertableState()
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

	void WStringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser(data);
	}

	void WStringProperty::SetValueByUser(const WString& value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void WStringProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void WStringProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::WStringProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(WString, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const WString&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const WString&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
	PROTECTED_FUNCTION(void, SetValueByUser, const WString&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
