#include "WStringProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

namespace Editor
{
	WStringProperty::WStringProperty()
	{
		InitializeWidget();
	}

	WStringProperty::WStringProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	void WStringProperty::InitializeWidget()
	{
		mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
		mEditBox->layout.minHeight = 10;
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &WStringProperty::OnEdited);
	}

	WStringProperty::~WStringProperty()
	{
		delete mEditBox;
	}

	void WStringProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

		Update();
	}

	void WStringProperty::Update()
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
		{
			mEditBox->text = "--";
			mCommonValue = "";
		}
		else mEditBox->text = mCommonValue;
	}

	UIWidget* WStringProperty::GetWidget() const
	{
		return mEditBox;
	}

	WString WStringProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool WStringProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* WStringProperty::GetFieldType() const
	{
		return &TypeOf(WString);
	}

	void WStringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		mCommonValue = data;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);
	}
}