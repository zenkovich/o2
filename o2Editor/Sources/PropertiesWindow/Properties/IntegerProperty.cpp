#include "IntegerProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	IntegerProperty::IntegerProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->FindChild<UIEditBox>();
		mEditBox->onChangeCompleted = Func(this, &IntegerProperty::OnEdited);
		mEditBox->text = "--";
		mEditBox->SetFilterInteger();

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = Func(this, &IntegerProperty::Revert);

		auto handleLayer = mEditBox->GetLayer("arrows");

		if (handleLayer)
		{
			mEditBox->onDraw += [&]() { mDragHangle.OnDrawn(); };

			mDragHangle.cursorType = CursorType::SizeNS;
			mDragHangle.isUnderPoint = [=](const Vec2F& point) { return handleLayer->IsUnderPoint(point); };
			mDragHangle.onMoved = Func(this, &IntegerProperty::OnDragHandleMoved);
			mDragHangle.onCursorPressed = Func(this, &IntegerProperty::OnMoveHandlePressed);
			mDragHangle.onCursorReleased = Func(this, &IntegerProperty::OnMoveHandleReleased);
		}
	}

	IntegerProperty::~IntegerProperty()
	{
		delete mPropertyWidget;
	}

	void IntegerProperty::SetValue(int value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void IntegerProperty::SetUnknownValue(int defaultValue /*= 0*/)
	{
		mValuesDifferent = true;
		mCommonValue = defaultValue;
		mEditBox->text = "--";

		OnChanged();
	}

	void IntegerProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
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

		Refresh();
	}

	void IntegerProperty::Refresh()
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
				SetUnknownValue(newCommonValue);
		}
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	void IntegerProperty::Revert()
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

	UIWidget* IntegerProperty::GetWidget() const
	{
		return mPropertyWidget;
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

	void IntegerProperty::SetCommonValue(int value)
	{
		mValuesDifferent = false;
		mCommonValue = value;
		mEditBox->text = (WString)mCommonValue;

		OnChanged();
	}

	void IntegerProperty::CheckRevertableState()
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

	void IntegerProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser((const int)data);
	}

	void IntegerProperty::OnDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValue(mCommonValue + (int)cursor.delta.y);
	}

	void IntegerProperty::OnKeyReleased(const Input::Key& key)
	{
		if (!mEditBox->IsFocused())
			return;

		if (key == VK_UP)
		{
			SetValueByUser(mCommonValue + 1);
			mEditBox->SelectAll();
		}

		if (key == VK_DOWN)
		{
			SetValueByUser(mCommonValue - 1);
			mEditBox->SelectAll();
		}
	}

	void IntegerProperty::OnMoveHandlePressed(const Input::Cursor& cursor)
	{
		mBeforeChangeValues = mCommonValue;
		o2Application.SetCursorInfiniteMode(true);
	}

	void IntegerProperty::OnMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void IntegerProperty::SetValueByUser(int value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void IntegerProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void IntegerProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::IntegerProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDragHangle);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, int);
	PUBLIC_FUNCTION(int, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, int);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
	PROTECTED_FUNCTION(void, OnDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetValueByUser, int);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
