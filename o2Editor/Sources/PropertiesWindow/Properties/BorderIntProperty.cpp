#include "BorderIntProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace Editor
{
	BorderIProperty::BorderIProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mWidget = widget;
		else
			mWidget = o2UI.CreateWidget<UIWidget>("rectangle property");

		mLeftEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("left edit"));
		mBottomEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("bottom edit"));
		mRightEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("right edit"));
		mTopEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("top edit"));

		mLeftEditBox->onChangeCompleted = Function<void(const WString&)>(this, &BorderIProperty::OnLeftEdited);
		mLeftEditBox->text = "--";
		mLeftEditBox->SetFilterInteger();

		if (auto leftHandleLayer = mLeftEditBox->GetLayer("arrows"))
		{
			mLeftEditBox->onDraw += [&]() { mLeftDragHangle.OnDrawn(); };

			mLeftDragHangle.cursorType = CursorType::SizeNS;
			mLeftDragHangle.isUnderPoint = [=](const Vec2F& point) { return leftHandleLayer->IsUnderPoint(point); };
			mLeftDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &BorderIProperty::OnLeftDragHandleMoved);
			mLeftDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mLeftDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mBottomEditBox->onChangeCompleted = Function<void(const WString&)>(this, &BorderIProperty::OnBottomEdited);
		mBottomEditBox->text = "--";
		mBottomEditBox->SetFilterInteger();

		if (auto bottomHandleLayer = mBottomEditBox->GetLayer("arrows"))
		{
			mBottomEditBox->onDraw += [&]() { mBottomDragHangle.OnDrawn(); };

			mBottomDragHangle.cursorType = CursorType::SizeNS;
			mBottomDragHangle.isUnderPoint = [=](const Vec2F& point) { return bottomHandleLayer->IsUnderPoint(point); };
			mBottomDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &BorderIProperty::OnBottomDragHandleMoved);
			mBottomDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mBottomDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mRightEditBox->onChangeCompleted = Function<void(const WString&)>(this, &BorderIProperty::OnRightEdited);
		mRightEditBox->text = "--";
		mRightEditBox->SetFilterInteger();

		if (auto rightHandleLayer = mRightEditBox->GetLayer("arrows"))
		{
			mRightEditBox->onDraw += [&]() { mRightDragHangle.OnDrawn(); };

			mRightDragHangle.cursorType = CursorType::SizeNS;
			mRightDragHangle.isUnderPoint = [=](const Vec2F& point) { return rightHandleLayer->IsUnderPoint(point); };
			mRightDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &BorderIProperty::OnRightDragHandleMoved);
			mRightDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mRightDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}

		mTopEditBox->onChangeCompleted = Function<void(const WString&)>(this, &BorderIProperty::OnTopEdited);
		mTopEditBox->text = "--";
		mTopEditBox->SetFilterInteger();

		if (auto topHandleLayer = mTopEditBox->GetLayer("arrows"))
		{
			mTopEditBox->onDraw += [&]() { mTopDragHangle.OnDrawn(); };

			mTopDragHangle.cursorType = CursorType::SizeNS;
			mTopDragHangle.isUnderPoint = [=](const Vec2F& point) { return topHandleLayer->IsUnderPoint(point); };
			mTopDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &BorderIProperty::OnTopDragHandleMoved);
			mTopDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mTopDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}
	}

	BorderIProperty::~BorderIProperty()
	{
		delete mWidget;
	}

	void BorderIProperty::SetValue(const BorderI& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, value);

		SetCommonValue(value);
	}

	void BorderIProperty::SetValueLeft(int value)
	{
		for (auto ptr : mValuesPointers)
			mLeftAssignFunc(ptr, value);

		SetCommonValueLeft(value);
	}

	void BorderIProperty::SetValueRight(int value)
	{
		for (auto ptr : mValuesPointers)
			mRightAssignFunc(ptr, value);

		SetCommonValueRight(value);
	}

	void BorderIProperty::SetValueTop(int value)
	{
		for (auto ptr : mValuesPointers)
			mTopAssignFunc(ptr, value);

		SetCommonValueTop(value);
	}

	void BorderIProperty::SetValueBottom(int value)
	{
		for (auto ptr : mValuesPointers)
			mBottomAssignFunc(ptr, value);

		SetCommonValueBottom(value);
	}

	void BorderIProperty::SetUnknownValue(const BorderI& defaultValue /*= BorderI()*/)
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

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetLeftUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mLeftValuesDifferent = true;
		mCommonValue.left = defaultValue;
		mLeftEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetRightUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mRightValuesDifferent = true;
		mCommonValue.right = defaultValue;
		mRightEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetTopUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mTopValuesDifferent = true;
		mCommonValue.top = defaultValue;
		mTopEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetBottomUnknownValue(int defaultValue /*= 0.0f*/)
	{
		mBottomValuesDifferent = true;
		mCommonValue.bottom = defaultValue;
		mBottomEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const BorderI& value) { *((Property<BorderI>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<BorderI>*)(ptr))->Get(); };

			mLeftAssignFunc = [](void* ptr, int value) { auto p = ((Property<BorderI>*)(ptr)); BorderI v = p->Get(); v.left = value; p->Set(v); };
			mLeftGetFunc = [](void* ptr) { return ((Property<BorderI>*)(ptr))->Get().left; };

			mRightAssignFunc = [](void* ptr, int value) { auto p = ((Property<BorderI>*)(ptr)); BorderI v = p->Get(); v.right = value; p->Set(v); };
			mRightGetFunc = [](void* ptr) { return ((Property<BorderI>*)(ptr))->Get().right; };

			mTopAssignFunc = [](void* ptr, int value) { auto p = ((Property<BorderI>*)(ptr)); BorderI v = p->Get(); v.top = value; p->Set(v); };
			mTopGetFunc = [](void* ptr) { return ((Property<BorderI>*)(ptr))->Get().top; };

			mBottomAssignFunc = [](void* ptr, int value) { auto p = ((Property<BorderI>*)(ptr)); BorderI v = p->Get(); v.bottom = value; p->Set(v); };
			mBottomGetFunc = [](void* ptr) { return ((Property<BorderI>*)(ptr))->Get().bottom; };
		}
		else
		{
			mAssignFunc = [](void* ptr, const BorderI& value) { *((BorderI*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((BorderI*)(ptr)); };

			mLeftAssignFunc = [](void* ptr, int value) { ((BorderI*)(ptr))->left = value; };
			mLeftGetFunc = [](void* ptr) { return ((BorderI*)(ptr))->left; };

			mRightAssignFunc = [](void* ptr, int value) { ((BorderI*)(ptr))->right = value; };
			mRightGetFunc = [](void* ptr) { return ((BorderI*)(ptr))->right; };

			mTopAssignFunc = [](void* ptr, int value) { ((BorderI*)(ptr))->top = value; };
			mTopGetFunc = [](void* ptr) { return ((BorderI*)(ptr))->top; };

			mBottomAssignFunc = [](void* ptr, int value) { ((BorderI*)(ptr))->bottom = value; };
			mBottomGetFunc = [](void* ptr) { return ((BorderI*)(ptr))->bottom; };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void BorderIProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastLeftDifferent = mLeftValuesDifferent;
		auto lastRightDifferent = mRightValuesDifferent;
		auto lastTopDifferent = mTopValuesDifferent;
		auto lastBottomDifferent = mBottomValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0]);
		auto newLeftValuesDifferent = false;
		auto newBottomValuesDifferent = false;
		auto newRightValuesDifferent = false;
		auto newTopValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			auto value = mGetFunc(mValuesPointers[i]);
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

	UIWidget* BorderIProperty::GetWidget() const
	{
		return mWidget;
	}

	BorderI BorderIProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool BorderIProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* BorderIProperty::GetFieldType() const
	{
		return &TypeOf(BorderI);
	}

	void BorderIProperty::SetCommonValue(const BorderI& value)
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

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetCommonValueLeft(int value)
	{
		mLeftValuesDifferent = false;
		mCommonValue.left = value;
		mLeftEditBox->text = (WString)value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetCommonValueRight(int value)
	{
		mRightValuesDifferent = false;
		mCommonValue.right = value;
		mRightEditBox->text = (WString)value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetCommonValueTop(int value)
	{
		mTopValuesDifferent = false;
		mCommonValue.top = value;
		mTopEditBox->text = (WString)value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::SetCommonValueBottom(int value)
	{
		mBottomValuesDifferent = false;
		mCommonValue.bottom = value;
		mBottomEditBox->text = (WString)value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BorderIProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		SetValueLeft((const int)data);
	}

	void BorderIProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		SetValueBottom((const int)data);
	}

	void BorderIProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		SetValueRight((const int)data);
	}

	void BorderIProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		SetValueTop((const int)data);
	}

	void BorderIProperty::OnLeftDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueLeft(mCommonValue.left + (int)cursor.delta.y);
	}

	void BorderIProperty::OnRightDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueRight(mCommonValue.right + (int)cursor.delta.y);
	}

	void BorderIProperty::OnTopDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueTop(mCommonValue.top + (int)cursor.delta.y);
	}

	void BorderIProperty::OnBottomDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueBottom(mCommonValue.bottom + (int)cursor.delta.y);
	}

}

CLASS_META(Editor::BorderIProperty)
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
	PROTECTED_FIELD(mWidget);
	PROTECTED_FIELD(mLeftEditBox);
	PROTECTED_FIELD(mBottomEditBox);
	PROTECTED_FIELD(mRightEditBox);
	PROTECTED_FIELD(mTopEditBox);
	PROTECTED_FIELD(mLeftDragHangle);
	PROTECTED_FIELD(mRightDragHangle);
	PROTECTED_FIELD(mTopDragHangle);
	PROTECTED_FIELD(mBottomDragHangle);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const BorderI&);
	PUBLIC_FUNCTION(void, SetValueLeft, int);
	PUBLIC_FUNCTION(void, SetValueRight, int);
	PUBLIC_FUNCTION(void, SetValueTop, int);
	PUBLIC_FUNCTION(void, SetValueBottom, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const BorderI&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, int);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, int);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, int);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, int);
	PUBLIC_FUNCTION(BorderI, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const BorderI&);
	PROTECTED_FUNCTION(void, SetCommonValueLeft, int);
	PROTECTED_FUNCTION(void, SetCommonValueRight, int);
	PROTECTED_FUNCTION(void, SetCommonValueTop, int);
	PROTECTED_FUNCTION(void, SetCommonValueBottom, int);
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