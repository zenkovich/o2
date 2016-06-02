#include "TagProperty.h"

#include "Scene/Scene.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	TagsProperty::TagsProperty()
	{
		InitializeWidget();
	}

	TagsProperty::TagsProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	TagsProperty::~TagsProperty()
	{
		delete mEditBox;
	}

	void TagsProperty::InitializeWidget()
	{
		mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");

		mEditBox->onFocused = [&]() { UpdateContextData(""); };
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &TagsProperty::OnEditBoxChangeCompleted);
		mEditBox->onChanged = Function<void(const WString&)>(this, &TagsProperty::OnEditBoxChanged);

		mTagsContext = o2UI.CreateWidget<UIContextMenu>();
		mTagsContext->SetMaxItemsVisible(10);
		mEditBox->AddChild(mTagsContext);
	}

	void TagsProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

		Update();
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

	void TagsProperty::Update()
	{
		if (mValuesPointers.IsEmpty())
			return;

		mCommonValue = mGetFunc(mValuesPointers[0]);
		mValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (mCommonValue != mGetFunc(mValuesPointers[i]))
			{
				mValuesDifferent = true;
				break;
			}
		}

		if (mValuesDifferent)
			mEditBox->text = "--";
		else
		{
			WString res;
			for (auto tag : mCommonValue.GetTagsNames())
				res += tag + " ";

			mPushingTag = true;
			mEditBox->text = res;
			mPushingTag = false;
		}
	}

	void TagsProperty::OnEditBoxChanged(const WString& text)
	{
		if (mPushingTag)
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

		mCommonValue.Clear();

		for (auto tagName : tagsNames)
		{
			if (!tagName.IsEmpty())
				mCommonValue.AddTag(tagName);
		}

		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		mTagsContext->Hide();
	}

	void TagsProperty::PushTag(String name)
	{
		mPushingTag = true;

		String editText = (WString)mEditBox->text;

		int spaceIdx = editText.FindLast(" ");
		if (spaceIdx >= 0)
			editText = editText.SubStr(0, spaceIdx);
		else
			editText = "";

		if (!editText.IsEmpty())
			editText += " ";

		editText += name;

		mEditBox->text = editText;

		mPushingTag = false;

		SetTags(editText);
	}

	UIWidget* TagsProperty::GetWidget() const
	{
		return mEditBox;
	}

	const TagGroup& TagsProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool TagsProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* TagsProperty::GetFieldType() const
	{
		return &TypeOf(TagGroup);
	}
}