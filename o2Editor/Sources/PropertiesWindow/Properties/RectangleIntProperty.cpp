#include "RectangleIntProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace Editor
{
	RectIProperty::RectIProperty(UIWidget* widget /*= nullptr*/)
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
			mRevertBtn->onClick = Function<void()>(this, &RectIProperty::Revert);

		mLeftEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnLeftEdited);
		mLeftEditBox->text = "--";
		mLeftEditBox->SetFilterInteger();

		if (auto leftHandleLayer = mLeftEditBox->GetLayer("arrows"))
		{
			mLeftEditBox->onDraw += [&]() { mLeftDragHangle.OnDrawn(); };

			mLeftDragHangle.cursorType = CursorType::SizeNS;
			mLeftDragHangle.isUnderPoint = [=](const Vec2F& point) { return leftHandleLayer->IsUnderPoint(point); };
			mLeftDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &RectIProperty::OnLeftDragHandleMoved);
			mLeftDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mLeftDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mBottomEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnBottomEdited);
		mBottomEditBox->text = "--";
		mBottomEditBox->SetFilterInteger();

		if (auto bottomHandleLayer = mBottomEditBox->GetLayer("arrows"))
		{
			mBottomEditBox->onDraw += [&]() { mBottomDragHangle.OnDrawn(); };

			mBottomDragHangle.cursorType = CursorType::SizeNS;
			mBottomDragHangle.isUnderPoint = [=](const Vec2F& point) { return bottomHandleLayer->IsUnderPoint(point); };
			mBottomDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &RectIProperty::OnBottomDragHandleMoved);
			mBottomDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mBottomDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mRightEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnRightEdited);
		mRightEditBox->text = "--";
		mRightEditBox->SetFilterInteger();

		if (auto rightHandleLayer = mRightEditBox->GetLayer("arrows"))
		{
			mRightEditBox->onDraw += [&]() { mRightDragHangle.OnDrawn(); };

			mRightDragHangle.cursorType = CursorType::SizeNS;
			mRightDragHangle.isUnderPoint = [=](const Vec2F& point) { return rightHandleLayer->IsUnderPoint(point); };
			mRightDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &RectIProperty::OnRightDragHandleMoved);
			mRightDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mRightDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mTopEditBox->onChangeCompleted = Function<void(const WString&)>(this, &RectIProperty::OnTopEdited);
		mTopEditBox->text = "--";
		mTopEditBox->SetFilterInteger();

		if (auto topHandleLayer = mTopEditBox->GetLayer("arrows"))
		{
			mTopEditBox->onDraw += [&]() { mTopDragHangle.OnDrawn(); };

			mTopDragHangle.cursorType = CursorType::SizeNS;
			mTopDragHangle.isUnderPoint = [=](const Vec2F& point) { return topHandleLayer->IsUnderPoint(point); };
			mTopDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &RectIProperty::OnTopDragHandleMoved);
			mTopDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mTopDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}
	}

	RectIProperty::~RectIProperty()
	{
		delete mPropertyWidget;
	}

	void RectIProperty::SetValue(const RectI& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void RectIProperty::SetValueLeft(int value)
	{
		for (auto ptr : mValuesPointers)
			mLeftAssignFunc(ptr.first, value);

		SetCommonValueLeft(value);
	}

	void RectIProperty::SetValueRight(int value)
	{
		for (auto ptr : mValuesPointers)
			mRightAssignFunc(ptr.first, value);

		SetCommonValueRight(value);
	}

	void RectIProperty::SetValueTop(int value)
	{
		for (auto ptr : mValuesPointers)
			mTopAssignFunc(ptr.first, value);

		SetCommonValueTop(value);
	}

	void RectIProperty::SetValueBottom(int value)
	{
		for (auto ptr : mValuesPointers)
			mBottomAssignFunc(ptr.first, value);

		SetCommonValueBottom(value);
	}

	void RectIProperty::SetUnknownValue(const RectI& defaultValue /*= RectI()*/)
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

	void RectIProperty::SetLeftUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mLeftValuesDifferent = true;
		mCommonValue.left = defaultValue;
		mLeftEditBox->text = "--";

		OnChanged();
	}

	void RectIProperty::SetRightUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mRightValuesDifferent = true;
		mCommonValue.right = defaultValue;
		mRightEditBox->text = "--";

		OnChanged();
	}

	void RectIProperty::SetTopUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mTopValuesDifferent = true;
		mCommonValue.top = defaultValue;
		mTopEditBox->text = "--";

		OnChanged();
	}

	void RectIProperty::SetBottomUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mBottomValuesDifferent = true;
		mCommonValue.bottom = defaultValue;
		mBottomEditBox->text = "--";

		OnChanged();
	}

	void RectIProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const RectI& value) { *((Property<RectI>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get(); };

			mLeftAssignFunc = [](void* ptr, int value) { auto p = ((Property<RectI>*)(ptr)); RectI v = p->Get(); v.left = value; p->Set(v); };
			mLeftGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get().left; };

			mRightAssignFunc = [](void* ptr, int value) { auto p = ((Property<RectI>*)(ptr)); RectI v = p->Get(); v.right = value; p->Set(v); };
			mRightGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get().right; };

			mTopAssignFunc = [](void* ptr, int value) { auto p = ((Property<RectI>*)(ptr)); RectI v = p->Get(); v.top = value; p->Set(v); };
			mTopGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get().top; };

			mBottomAssignFunc = [](void* ptr, int value) { auto p = ((Property<RectI>*)(ptr)); RectI v = p->Get(); v.bottom = value; p->Set(v); };
			mBottomGetFunc = [](void* ptr) { return ((Property<RectI>*)(ptr))->Get().bottom; };
		}
		else
		{
			mAssignFunc = [](void* ptr, const RectI& value) { *((RectI*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((RectI*)(ptr)); };

			mLeftAssignFunc = [](void* ptr, int value) { ((RectI*)(ptr))->left = value; };
			mLeftGetFunc = [](void* ptr) { return ((RectI*)(ptr))->left; };

			mRightAssignFunc = [](void* ptr, int value) { ((RectI*)(ptr))->right = value; };
			mRightGetFunc = [](void* ptr) { return ((RectI*)(ptr))->right; };

			mTopAssignFunc = [](void* ptr, int value) { ((RectI*)(ptr))->top = value; };
			mTopGetFunc = [](void* ptr) { return ((RectI*)(ptr))->top; };

			mBottomAssignFunc = [](void* ptr, int value) { ((RectI*)(ptr))->bottom = value; };
			mBottomGetFunc = [](void* ptr) { return ((RectI*)(ptr))->bottom; };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void RectIProperty::Refresh()
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
	}

	void RectIProperty::Revert()
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

	UIWidget* RectIProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	RectI RectIProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool RectIProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* RectIProperty::GetFieldType() const
	{
		return &TypeOf(RectI);
	}

	void RectIProperty::SetCommonValue(const RectI& value)
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

	void RectIProperty::SetCommonValueLeft(int value)
	{
		mLeftValuesDifferent = false;
		mCommonValue.left = value;
		mLeftEditBox->text = (WString)value;

		OnChanged();
	}

	void RectIProperty::SetCommonValueRight(int value)
	{
		mRightValuesDifferent = false;
		mCommonValue.right = value;
		mRightEditBox->text = (WString)value;

		OnChanged();
	}

	void RectIProperty::SetCommonValueTop(int value)
	{
		mTopValuesDifferent = false;
		mCommonValue.top = value;
		mTopEditBox->text = (WString)value;

		OnChanged();
	}

	void RectIProperty::SetCommonValueBottom(int value)
	{
		mBottomValuesDifferent = false;
		mCommonValue.bottom = value;
		mBottomEditBox->text = (WString)value;

		OnChanged();
	}

	void RectIProperty::CheckRevertableState()
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

	void RectIProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		SetValueLeft((const int)data);
	}

	void RectIProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		SetValueBottom((const int)data);
	}

	void RectIProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		SetValueRight((const int)data);
	}

	void RectIProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		SetValueTop((const int)data);
	}

	void RectIProperty::OnLeftDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueLeft(mCommonValue.left + (int)cursor.delta.y);
	}

	void RectIProperty::OnRightDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueRight(mCommonValue.right + (int)cursor.delta.y);
	}

	void RectIProperty::OnTopDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueTop(mCommonValue.top + (int)cursor.delta.y);
	}

	void RectIProperty::OnBottomDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueBottom(mCommonValue.bottom + (int)cursor.delta.y);
	}

}

CLASS_META(Editor::RectIProperty)
{
	BASE_CLASS(Editor::IPropertyField);

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
	PUBLIC_FUNCTION(void, SetValue, const RectI&);
	PUBLIC_FUNCTION(void, SetValueLeft, int);
	PUBLIC_FUNCTION(void, SetValueRight, int);
	PUBLIC_FUNCTION(void, SetValueTop, int);
	PUBLIC_FUNCTION(void, SetValueBottom, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const RectI&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, int);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, int);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, int);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, int);
	PUBLIC_FUNCTION(RectI, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const RectI&);
	PROTECTED_FUNCTION(void, SetCommonValueLeft, int);
	PROTECTED_FUNCTION(void, SetCommonValueRight, int);
	PROTECTED_FUNCTION(void, SetCommonValueTop, int);
	PROTECTED_FUNCTION(void, SetCommonValueBottom, int);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
	PROTECTED_FUNCTION(void, OnLeftDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomDragHandleMoved, const Input::Cursor&);
}
END_META;
 