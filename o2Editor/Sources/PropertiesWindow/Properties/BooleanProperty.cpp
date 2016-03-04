#include "BooleanProperty.h"

#include "UI/UIManager.h"
#include "UI/Toggle.h"

EditorBooleanProperty::EditorBooleanProperty()
{
	InitializeWidget();
}

EditorBooleanProperty::EditorBooleanProperty(const Vector<void*>& targets, bool isProperty)
{
	InitializeWidget();
	Setup(targets, isProperty);
}

void EditorBooleanProperty::InitializeWidget()
{
	mToggle = o2UI.CreateWidget<UIToggle>("without caption");
	mToggle->layout.minHeight = 10;
	mToggle->onToggleByUser = Function<void(bool)>(this, &EditorBooleanProperty::OnEdited);
}

EditorBooleanProperty::~EditorBooleanProperty()
{
	delete mToggle;
}

void EditorBooleanProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

	Update();
}

void EditorBooleanProperty::Update()
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
		mToggle->value = false;
		mToggle->SetValueUnknown();
		mCommonValue = false;
	}
	else mToggle->value = mCommonValue;
}

UIWidget* EditorBooleanProperty::GetWidget() const
{
	return mToggle;
}

bool EditorBooleanProperty::GetCommonValue() const
{
	return mCommonValue;
}

bool EditorBooleanProperty::IsValuesDifferent() const
{
	return mValuesDifferent;
}

const Type* EditorBooleanProperty::GetFieldType() const
{
	return &TypeOf(bool);
}

void EditorBooleanProperty::OnEdited(bool value)
{
	mCommonValue = value;
	mValuesDifferent = false;

	for (auto ptr : mValuesPointers)
		mAssignFunc(ptr, mCommonValue);
}