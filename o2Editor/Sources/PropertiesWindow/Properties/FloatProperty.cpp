#include "FloatProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

EditorFloatProperty::EditorFloatProperty()
{
	InitializeWidget();
}

EditorFloatProperty::EditorFloatProperty(const Vector<void*>& targets, bool isProperty)
{
	InitializeWidget();
	Setup(targets, isProperty);
}

void EditorFloatProperty::InitializeWidget()
{
	mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
	mEditBox->layout.minHeight = 10;
	mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &EditorFloatProperty::OnEdited);
	mEditBox->SetFilterFloat();
}

EditorFloatProperty::~EditorFloatProperty()
{
	delete mEditBox;
}

void EditorFloatProperty::Setup(const Vector<void*>& targets, bool isProperty)
{
	if (isProperty)
	{
		mAssignFunc = [](void* ptr, const float& value) { *((Property<float>*)(ptr)) = value; };
		mGetFunc = [](void* ptr) { return ((Property<float>*)(ptr))->Get(); };
	}
	else
	{
		mAssignFunc = [](void* ptr, const float& value) { *((float*)(ptr)) = value; };
		mGetFunc = [](void* ptr) { return *((float*)(ptr)); };
	}

	mValuesPointers = targets;

	Update();
}

void EditorFloatProperty::Update()
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
		mCommonValue = 0.0f;
	}
	else mEditBox->text = (WString)mCommonValue;
}

UIWidget* EditorFloatProperty::GetWidget() const
{
	return mEditBox;
}

float EditorFloatProperty::GetCommonValue() const
{
	return mCommonValue;
}

bool EditorFloatProperty::IsValuesDifferent() const
{
	return mValuesDifferent;
}

const Type* EditorFloatProperty::GetFieldType() const
{
	return &TypeOf(float);
}

void EditorFloatProperty::OnEdited(const WString& data)
{
	if (mValuesDifferent && data == "--")
		return;

	mCommonValue = (const float)data;
	mValuesDifferent = false;

	for (auto ptr : mValuesPointers)
		mAssignFunc(ptr, mCommonValue);
}