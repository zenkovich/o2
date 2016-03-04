#include "StringProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

EditorStringProperty::EditorStringProperty()
{
	InitializeWidget();
}

EditorStringProperty::EditorStringProperty(const Vector<void*>& targets, bool isProperty)
{
	InitializeWidget();
	Setup(targets, isProperty);
}

void EditorStringProperty::InitializeWidget()
{
	mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
	mEditBox->layout.minHeight = 10;
	mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &EditorStringProperty::OnEdited);
}

EditorStringProperty::~EditorStringProperty()
{
	delete mEditBox;
}

void EditorStringProperty::Setup(const Vector<void*>& targets, bool isProperty)
{
	if (isProperty)
	{
		mAssignFunc = [](void* ptr, const String& value) { *((Property<String>*)(ptr)) = value; };
		mGetFunc = [](void* ptr) { return ((Property<String>*)(ptr))->Get(); };
	}
	else
	{
		mAssignFunc = [](void* ptr, const String& value) { *((String*)(ptr)) = value; };
		mGetFunc = [](void* ptr) { return *((String*)(ptr)); };
	}

	mValuesPointers = targets;

	Update();
}

void EditorStringProperty::Update()
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
	else mEditBox->text = (WString)mCommonValue;
}

UIWidget* EditorStringProperty::GetWidget() const
{
	return mEditBox;
}

String EditorStringProperty::GetCommonValue() const
{
	return mCommonValue;
}

bool EditorStringProperty::IsValuesDifferent() const
{
	return mValuesDifferent;
}

const Type* EditorStringProperty::GetFieldType() const
{
	return &TypeOf(String);
}

void EditorStringProperty::OnEdited(const WString& data)
{
	if (mValuesDifferent && data == "--")
		return;

	mCommonValue = data;
	mValuesDifferent = false;

	for (auto ptr : mValuesPointers)
		mAssignFunc(ptr, mCommonValue);
}