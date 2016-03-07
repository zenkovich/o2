#include "FloatProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

namespace Editor
{
	FloatProperty::FloatProperty()
	{
		InitializeWidget();
	}

	FloatProperty::FloatProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	void FloatProperty::InitializeWidget()
	{
		mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
		mEditBox->layout.minHeight = 10;
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &FloatProperty::OnEdited);
		mEditBox->SetFilterFloat();
	}

	FloatProperty::~FloatProperty()
	{
		delete mEditBox;
	}

	void FloatProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

	void FloatProperty::Update()
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

	UIWidget* FloatProperty::GetWidget() const
	{
		return mEditBox;
	}

	float FloatProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool FloatProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* FloatProperty::GetFieldType() const
	{
		return &TypeOf(float);
	}

	void FloatProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		mCommonValue = (const float)data;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);
	}
}