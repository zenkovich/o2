#include "TagProperty.h"

#include "Scene/Scene.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	TagsProperty::TagsProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->FindChild<UIEditBox>();
		if (!mEditBox)
			mEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget);

		mEditBox->onFocused = [&]() { UpdateContextData(""); };
		mEditBox->onChangeCompleted = Func(this, &TagsProperty::OnEditBoxChangeCompleted);
		mEditBox->onChanged = Func(this, &TagsProperty::OnEditBoxChanged);

		mTagsContext = o2UI.CreateWidget<UIContextMenu>();
		mTagsContext->SetMaxItemsVisible(10);
		mEditBox->AddChild(mTagsContext);

		mEditBox->text = "--";

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = Func(this, &TagsProperty::Revert);
	}

	TagsProperty::~TagsProperty()
	{
		delete mPropertyWidget;
	}

	void TagsProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const TagGroup& value) { *((Property<TagGroup>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<TagGroup>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const TagGroup& value) { *((TagGroup*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((TagGroup*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void TagsProperty::UpdateContextData(const WString& filter)
	{
		if (mPushingTag)
			return;

		mTagsContext->RemoveAllItems();

		mTagsContext->Show(mEditBox->layout.GetAbsoluteLeftBottom());

		for (auto tag : o2Scene.GetTags())
		{
			if (filter.IsEmpty() || tag->GetName().CountOf(filter) > 0)
				mTagsContext->AddItem(tag->GetName(), [=]() { PushTag(tag->GetName()); });
		}
	}

	void TagsProperty::Refresh()
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
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	void TagsProperty::Revert()
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

	void TagsProperty::SetCommonValue(const TagGroup& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		WString res;
		for (auto tag : mCommonValue.GetTagsNames())
			res += tag + " ";

		mPushingTag = true;
		mEditBox->text = res;
		mPushingTag = false;

		OnChanged();
	}

	void TagsProperty::CheckRevertableState()
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

	void TagsProperty::OnEditBoxChanged(const WString& text)
	{
		if (mPushingTag || !mEditBox->IsFocused())
			return;

		WString lastTagText;

		int spacePos = text.FindLast(" ");
		if (spacePos < 0)
			lastTagText = text;
		else
			lastTagText = text.SubStr(spacePos + 1);

		UpdateContextData(lastTagText);
	}

	void TagsProperty::OnEditBoxChangeCompleted(const WString& text)
	{
		SetTags(text);
	}

	void TagsProperty::SetTags(const WString &text)
	{
		auto tagsNames = text.Split(" ");

		TagGroup tagsValue;

		for (auto tagName : tagsNames)
		{
			if (!tagName.IsEmpty())
				tagsValue.AddTag(tagName);
		}

		SetValueByUser(tagsValue);
	}

	void TagsProperty::PushTag(String name)
	{
		String editText = (WString)mEditBox->text;

		int spaceIdx = editText.FindLast(" ");
		if (spaceIdx >= 0)
			editText = editText.SubStr(0, spaceIdx);
		else
			editText = "";

		if (!editText.IsEmpty())
			editText += " ";

		editText += name;

		SetTags(editText);
	}

	UIWidget* TagsProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	const TagGroup& TagsProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool TagsProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void TagsProperty::SetValue(const TagGroup& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void TagsProperty::SetUnknownValue()
	{
		mValuesDifferent = true;
		mEditBox->text = "--";

		OnChanged();
	}

	const Type* TagsProperty::GetFieldType() const
	{
		return &TypeOf(TagGroup);
	}

	void TagsProperty::SetValueByUser(const TagGroup& value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void TagsProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void TagsProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::TagsProperty)
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
	PROTECTED_FIELD(mTagsContext);
	PROTECTED_FIELD(mPushingTag);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const TagGroup&, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const TagGroup&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const TagGroup&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, UpdateContextData, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChanged, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChangeCompleted, const WString&);
	PROTECTED_FUNCTION(void, SetTags, const WString&);
	PROTECTED_FUNCTION(void, PushTag, String);
	PROTECTED_FUNCTION(void, SetValueByUser, const TagGroup&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
