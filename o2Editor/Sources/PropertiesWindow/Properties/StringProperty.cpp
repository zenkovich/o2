#include "stdafx.h"
#include "StringProperty.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	StringProperty::StringProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->GetChildByType<UIEditBox>();
		if (!mEditBox)
			mEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget);

		mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
		mEditBox->text = "--";

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	StringProperty::~StringProperty()
	{
		delete mPropertyWidget;
	}

	void StringProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void StringProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = GetProxy<String>(mValuesProxies[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			if (newCommonValue != GetProxy<String>(mValuesProxies[i].first))
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

		CheckRevertableState();
	}

	void StringProperty::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<String>(ptr.first, GetProxy<String>(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* StringProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	String StringProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool StringProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void StringProperty::SetValue(const String& value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy<String>(ptr.first, value);

		SetCommonValue(value);
	}

	void StringProperty::SetUnknownValue()
	{
		mCommonValue = "";
		mValuesDifferent = true;
		mEditBox->text = "--";

		OnChanged();
	}

	const Type* StringProperty::GetFieldType() const
	{
		return &TypeOf(String);
	}

	void StringProperty::SetCommonValue(const String& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mEditBox->text = value;

		OnChanged();
	}

	void StringProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<String>(ptr.first), GetProxy<String>(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void StringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser(data);
	}

	void StringProperty::SetValueByUser(const String& value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void StringProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void StringProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<String>(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::StringProperty);
