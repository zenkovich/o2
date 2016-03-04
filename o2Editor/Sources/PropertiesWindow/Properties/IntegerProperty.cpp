#include "IntegerProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

EditorIntegerProperty::EditorIntegerProperty()
{
	InitializeWidget();
}

EditorIntegerProperty::EditorIntegerProperty(const Vector<void*>& targets, bool isProperty)
{
	InitializeWidget();
	Setup(targets, isProperty);
}

void EditorIntegerProperty::InitializeWidget()
{
	mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
	mEditBox->layout.minHeight = 10;
	mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &EditorIntegerProperty::OnEdited);
	mEditBox->SetFilterInteger();
}

EditorIntegerProperty::~EditorIntegerProperty()
{
	delete mEditBox;
}

void EditorIntegerProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

void EditorIntegerProperty::Update()
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

UIWidget* EditorIntegerProperty::GetWidget() const
{
	return mEditBox;
}

int EditorIntegerProperty::GetCommonValue() const
{
	return mCommonValue;
}

bool EditorIntegerProperty::IsValuesDifferent() const
{
	return mValuesDifferent;
}

const Type* EditorIntegerProperty::GetFieldType() const
{
	return &TypeOf(int);
}

void EditorIntegerProperty::OnEdited(const WString& data)
{
	if (mValuesDifferent && data == "--")
		return;

	mCommonValue = (const int)data;
	mValuesDifferent = false;

	for (auto ptr : mValuesPointers)
		mAssignFunc(ptr, mCommonValue);
}