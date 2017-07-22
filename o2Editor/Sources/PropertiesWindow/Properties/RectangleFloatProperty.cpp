#include "RectangleFloatProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace Editor
{
	RectFProperty::RectFProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("rectangle property");

		mLeftEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/left edit"));
		mBottomEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/bottom edit"));
		mRightEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/right edit"));
		mTopEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/top edit"));

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mLeftEditBox->onChangeCompleted = THIS_FUNC(OnLeftEdited);
		mLeftEditBox->text = "--";
		mLeftEditBox->SetFilterFloat();

		if (auto leftHandleLayer = mLeftEditBox->GetLayer("arrows"))
		{
			mLeftEditBox->onDraw += [&]() { mLeftDragHangle.OnDrawn(); };

			mLeftDragHangle.cursorType = CursorType::SizeNS;
			mLeftDragHangle.isUnderPoint = [=](const Vec2F& point) { return leftHandleLayer->IsUnderPoint(point); };
			mLeftDragHangle.onMoved = THIS_FUNC(OnLeftDragHandleMoved);
			mLeftDragHangle.onCursorPressed = THIS_FUNC(OnLeftMoveHandlePressed);
			mLeftDragHangle.onCursorReleased = THIS_FUNC(OnLeftMoveHandleReleased);
		}

		mBottomEditBox->onChangeCompleted = THIS_FUNC(OnBottomEdited);
		mBottomEditBox->text = "--";
		mBottomEditBox->SetFilterFloat();

		if (auto bottomHandleLayer = mBottomEditBox->GetLayer("arrows"))
		{
			mBottomEditBox->onDraw += [&]() { mBottomDragHangle.OnDrawn(); };

			mBottomDragHangle.cursorType = CursorType::SizeNS;
			mBottomDragHangle.isUnderPoint = [=](const Vec2F& point) { return bottomHandleLayer->IsUnderPoint(point); };
			mBottomDragHangle.onMoved = THIS_FUNC(OnBottomDragHandleMoved);
			mBottomDragHangle.onCursorPressed = THIS_FUNC(OnBottomMoveHandlePressed);
			mBottomDragHangle.onCursorReleased = THIS_FUNC(OnBottomMoveHandleReleased);
		}

		mRightEditBox->onChangeCompleted = THIS_FUNC(OnRightEdited);
		mRightEditBox->text = "--";
		mRightEditBox->SetFilterFloat();

		if (auto rightHandleLayer = mRightEditBox->GetLayer("arrows"))
		{
			mRightEditBox->onDraw += [&]() { mRightDragHangle.OnDrawn(); };

			mRightDragHangle.cursorType = CursorType::SizeNS;
			mRightDragHangle.isUnderPoint = [=](const Vec2F& point) { return rightHandleLayer->IsUnderPoint(point); };
			mRightDragHangle.onMoved = THIS_FUNC(OnRightDragHandleMoved);
			mRightDragHangle.onCursorPressed = THIS_FUNC(OnRightMoveHandlePressed);
			mRightDragHangle.onCursorReleased = THIS_FUNC(OnRightMoveHandleReleased);
		}

		mTopEditBox->onChangeCompleted = THIS_FUNC(OnTopEdited);
		mTopEditBox->text = "--";
		mTopEditBox->SetFilterFloat();

		if (auto topHandleLayer = mTopEditBox->GetLayer("arrows"))
		{
			mTopEditBox->onDraw += [&]() { mTopDragHangle.OnDrawn(); };

			mTopDragHangle.cursorType = CursorType::SizeNS;
			mTopDragHangle.isUnderPoint = [=](const Vec2F& point) { return topHandleLayer->IsUnderPoint(point); };
			mTopDragHangle.onMoved = THIS_FUNC(OnTopDragHandleMoved);
			mTopDragHangle.onCursorPressed = THIS_FUNC(OnTopMoveHandlePressed);
			mTopDragHangle.onCursorReleased = THIS_FUNC(OnTopMoveHandleReleased);
		}
	}

	RectFProperty::~RectFProperty()
	{
		delete mPropertyWidget;
	}

	void RectFProperty::SetValue(const RectF& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void RectFProperty::SetValueLeft(float value)
	{
		for (auto ptr : mValuesPointers)
			mLeftAssignFunc(ptr.first, value);

		SetCommonValueLeft(value);
	}

	void RectFProperty::SetValueRight(float value)
	{
		for (auto ptr : mValuesPointers)
			mRightAssignFunc(ptr.first, value);

		SetCommonValueRight(value);
	}

	void RectFProperty::SetValueTop(float value)
	{
		for (auto ptr : mValuesPointers)
			mTopAssignFunc(ptr.first, value);

		SetCommonValueTop(value);
	}

	void RectFProperty::SetValueBottom(float value)
	{
		for (auto ptr : mValuesPointers)
			mBottomAssignFunc(ptr.first, value);

		SetCommonValueBottom(value);
	}

	void RectFProperty::SetUnknownValue(const RectF& defaultValue /*= RectF()*/)
	{
		mLeftValuesDifferent = true;
		mBottomValuesDifferent = true;
		mRightValuesDifferent = true;
		mTopValuesDifferent = true;

		mCommonValue = defaultValue;

		mLeftEditBox->text = "--";
		mBottomEditBox->text = "--";
		mRightEditBox->text = "--";
		mTopEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetLeftUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mLeftValuesDifferent = true;
		mCommonValue.left = defaultValue;
		mLeftEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetRightUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mRightValuesDifferent = true;
		mCommonValue.right = defaultValue;
		mRightEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetTopUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mTopValuesDifferent = true;
		mCommonValue.top = defaultValue;
		mTopEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetBottomUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mBottomValuesDifferent = true;
		mCommonValue.bottom = defaultValue;
		mBottomEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const RectF& value) { *((Property<RectF>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get(); };

			mLeftAssignFunc = [](void* ptr, float value) { auto p = ((Property<RectF>*)(ptr)); RectF v = p->Get(); v.left = value; p->Set(v); };
			mLeftGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get().left; };

			mRightAssignFunc = [](void* ptr, float value) { auto p = ((Property<RectF>*)(ptr)); RectF v = p->Get(); v.right = value; p->Set(v); };
			mRightGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get().right; };

			mTopAssignFunc = [](void* ptr, float value) { auto p = ((Property<RectF>*)(ptr)); RectF v = p->Get(); v.top = value; p->Set(v); };
			mTopGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get().top; };

			mBottomAssignFunc = [](void* ptr, float value) { auto p = ((Property<RectF>*)(ptr)); RectF v = p->Get(); v.bottom = value; p->Set(v); };
			mBottomGetFunc = [](void* ptr) { return ((Property<RectF>*)(ptr))->Get().bottom; };
		}
		else
		{
			mAssignFunc = [](void* ptr, const RectF& value) { *((RectF*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((RectF*)(ptr)); };

			mLeftAssignFunc = [](void* ptr, float value) { ((RectF*)(ptr))->left = value; };
			mLeftGetFunc = [](void* ptr) { return ((RectF*)(ptr))->left; };

			mRightAssignFunc = [](void* ptr, float value) { ((RectF*)(ptr))->right = value; };
			mRightGetFunc = [](void* ptr) { return ((RectF*)(ptr))->right; };

			mTopAssignFunc = [](void* ptr, float value) { ((RectF*)(ptr))->top = value; };
			mTopGetFunc = [](void* ptr) { return ((RectF*)(ptr))->top; };

			mBottomAssignFunc = [](void* ptr, float value) { ((RectF*)(ptr))->bottom = value; };
			mBottomGetFunc = [](void* ptr) { return ((RectF*)(ptr))->bottom; };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void RectFProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastLeftDifferent = mLeftValuesDifferent;
		auto lastRightDifferent = mRightValuesDifferent;
		auto lastTopDifferent = mTopValuesDifferent;
		auto lastBottomDifferent = mBottomValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newLeftValuesDifferent = false;
		auto newBottomValuesDifferent = false;
		auto newRightValuesDifferent = false;
		auto newTopValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			auto value = mGetFunc(mValuesPointers[i].first);
			if (!Math::Equals(newCommonValue.left, value.left))
				newLeftValuesDifferent = true;

			if (!Math::Equals(newCommonValue.bottom, value.bottom))
				newBottomValuesDifferent = true;

			if (!Math::Equals(newCommonValue.right, value.right))
				newRightValuesDifferent = true;

			if (!Math::Equals(newCommonValue.top, value.top))
				newTopValuesDifferent = true;
		}

		if (newLeftValuesDifferent)
		{
			if (!lastLeftDifferent)
				SetLeftUnknownValue(newCommonValue.left);
		}
		else if (!Math::Equals(lastCommonValue.left, newCommonValue.left) || lastLeftDifferent)
			SetCommonValueLeft(newCommonValue.left);

		if (newRightValuesDifferent)
		{
			if (!lastRightDifferent)
				SetRightUnknownValue(newCommonValue.right);
		}
		else if (!Math::Equals(lastCommonValue.right, newCommonValue.right) || lastRightDifferent)
			SetCommonValueRight(newCommonValue.right);

		if (newTopValuesDifferent)
		{
			if (!lastTopDifferent)
				SetTopUnknownValue(newCommonValue.top);
		}
		else if (!Math::Equals(lastCommonValue.top, newCommonValue.top) || lastTopDifferent)
			SetCommonValueTop(newCommonValue.top);

		if (newBottomValuesDifferent)
		{
			if (!lastBottomDifferent)
				SetBottomUnknownValue(newCommonValue.bottom);
		}
		else if (!Math::Equals(lastCommonValue.bottom, newCommonValue.bottom) || lastBottomDifferent)
			SetCommonValueBottom(newCommonValue.bottom);

		CheckRevertableState();
	}

	void RectFProperty::Revert()
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

	UIWidget* RectFProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	RectF RectFProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool RectFProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* RectFProperty::GetFieldType() const
	{
		return &TypeOf(RectF);
	}

	void RectFProperty::SetCommonValue(const RectF& value)
	{
		mLeftValuesDifferent = false;
		mBottomValuesDifferent = false;
		mRightValuesDifferent = false;
		mTopValuesDifferent = false;

		mCommonValue = value;

		mLeftEditBox->text = (WString)mCommonValue.left;
		mBottomEditBox->text = (WString)mCommonValue.bottom;
		mRightEditBox->text = (WString)mCommonValue.right;
		mTopEditBox->text = (WString)mCommonValue.top;

		OnChanged();
	}

	void RectFProperty::SetCommonValueLeft(float value)
	{
		mLeftValuesDifferent = false;
		mCommonValue.left = value;
		mLeftEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueRight(float value)
	{
		mRightValuesDifferent = false;
		mCommonValue.right = value;
		mRightEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueTop(float value)
	{
		mTopValuesDifferent = false;
		mCommonValue.top = value;
		mTopEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueBottom(float value)
	{
		mBottomValuesDifferent = false;
		mCommonValue.bottom = value;
		mBottomEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::CheckRevertableState()
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

	void RectFProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		SetLeftValueByUser((const float)data);
	}

	void RectFProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		SetBottomValueByUser((const float)data);
	}

	void RectFProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		SetRightValueByUser((const float)data);
	}

	void RectFProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		SetTopValueByUser((const float)data);
	}

	float RectFProperty::GetValueMultiplier(float delta) const
	{
		return Math::Abs(delta) < 20 ? 0.01f : 0.05f;
	}

	void RectFProperty::OnLeftDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueLeft(mCommonValue.left + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnRightDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueRight(mCommonValue.right + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnTopDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueTop(mCommonValue.top + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnBottomDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueBottom(mCommonValue.bottom + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnKeyReleased(const Input::Key& key)
	{
		auto func =[&](UIEditBox* editbox, float value, void(RectFProperty::*setter)(float))
		{
			if (editbox->IsFocused())
			{
				if (key == VK_UP)
				{
					(this->*setter)(Math::Ceil(value + 0.01f));
					editbox->SelectAll();
				}

				if (key == VK_DOWN)
				{
					(this->*setter)(Math::Floor(value - 0.01f));
					editbox->SelectAll();
				}
			}
		};

		func(mLeftEditBox, mCommonValue.left, &RectFProperty::SetLeftValueByUser);
		func(mRightEditBox, mCommonValue.right, &RectFProperty::SetRightValueByUser);
		func(mTopEditBox, mCommonValue.top, &RectFProperty::SetTopValueByUser);
		func(mBottomEditBox, mCommonValue.bottom, &RectFProperty::SetBottomValueByUser);
	}

	void RectFProperty::OnLeftMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnLeftMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnRightMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnRightMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnTopMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnTopMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnBottomMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnBottomMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetLeftValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueLeft(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetRightValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueRight(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetBottomValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueBottom(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetTopValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueTop(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void RectFProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::RectFProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mLeftAssignFunc);
	PROTECTED_FIELD(mLeftGetFunc);
	PROTECTED_FIELD(mRightAssignFunc);
	PROTECTED_FIELD(mRightGetFunc);
	PROTECTED_FIELD(mTopAssignFunc);
	PROTECTED_FIELD(mTopGetFunc);
	PROTECTED_FIELD(mBottomAssignFunc);
	PROTECTED_FIELD(mBottomGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mLeftValuesDifferent);
	PROTECTED_FIELD(mBottomValuesDifferent);
	PROTECTED_FIELD(mRightValuesDifferent);
	PROTECTED_FIELD(mTopValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mLeftEditBox);
	PROTECTED_FIELD(mBottomEditBox);
	PROTECTED_FIELD(mRightEditBox);
	PROTECTED_FIELD(mTopEditBox);
	PROTECTED_FIELD(mLeftDragHangle);
	PROTECTED_FIELD(mRightDragHangle);
	PROTECTED_FIELD(mTopDragHangle);
	PROTECTED_FIELD(mBottomDragHangle);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const RectF&);
	PUBLIC_FUNCTION(void, SetValueLeft, float);
	PUBLIC_FUNCTION(void, SetValueRight, float);
	PUBLIC_FUNCTION(void, SetValueTop, float);
	PUBLIC_FUNCTION(void, SetValueBottom, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const RectF&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, float);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, float);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, float);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, float);
	PUBLIC_FUNCTION(RectF, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const RectF&);
	PROTECTED_FUNCTION(void, SetCommonValueLeft, float);
	PROTECTED_FUNCTION(void, SetCommonValueRight, float);
	PROTECTED_FUNCTION(void, SetCommonValueTop, float);
	PROTECTED_FUNCTION(void, SetCommonValueBottom, float);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
	PROTECTED_FUNCTION(float, GetValueMultiplier, float);
	PROTECTED_FUNCTION(void, OnLeftDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetLeftValueByUser, float);
	PROTECTED_FUNCTION(void, SetRightValueByUser, float);
	PROTECTED_FUNCTION(void, SetBottomValueByUser, float);
	PROTECTED_FUNCTION(void, SetTopValueByUser, float);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
