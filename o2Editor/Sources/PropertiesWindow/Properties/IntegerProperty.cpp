#include "IntegerProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

namespace Editor
{
	IntegerProperty::IntegerProperty()
	{
		InitializeWidget();
	}

	IntegerProperty::IntegerProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	void IntegerProperty::InitializeWidget()
	{
		mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
		mEditBox->layout.minHeight = 10;
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &IntegerProperty::OnEdited);
		mEditBox->SetFilterInteger();
	}

	IntegerProperty::~IntegerProperty()
	{
		delete mEditBox;
	}

	void IntegerProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const int& value) { *((Property<int>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<int>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const int& value) { *((int*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((int*)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void IntegerProperty::Update()
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
			mCommonValue = 0;
		}
		else mEditBox->text = (WString)mCommonValue;
	}

	UIWidget* IntegerProperty::GetWidget() const
	{
		return mEditBox;
	}

	int IntegerProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool IntegerProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* IntegerProperty::GetFieldType() const
	{
		return &TypeOf(int);
	}

	void IntegerProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		mCommonValue = (const int)data;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);
	}
}