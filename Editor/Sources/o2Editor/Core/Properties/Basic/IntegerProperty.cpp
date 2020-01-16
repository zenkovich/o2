#include "o2Editor/stdafx.h"
#include "IntegerProperty.h"

#include "o2/Application/Application.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/Widgets/EditBox.h"

namespace Editor
{
	IntegerProperty::IntegerProperty()
	{}

	IntegerProperty::IntegerProperty(const IntegerProperty& other) :
		TPropertyField<int>(other)
	{
		InitializeControls();
	}

	IntegerProperty& IntegerProperty::operator=(const IntegerProperty& other)
	{
		TPropertyField<int>::operator=(other);
		return *this;
	}

	void IntegerProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<int>::CopyData(otherActor);
		InitializeControls();
	}

	void IntegerProperty::InitializeControls()
	{
		mEditBox = FindChildByType<EditBox>();
		if (mEditBox)
		{
			mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
			mEditBox->text = "--";
			mEditBox->SetFilterInteger();

			auto handleLayer = mEditBox->FindLayer("arrows");
			if (handleLayer)
			{
				mEditBox->onDraw += [&]() { mDragHangle.OnDrawn(); };

				mDragHangle.cursorType = CursorType::SizeNS;
				mDragHangle.isUnderPoint = [=](const Vec2F& point) { return handleLayer->IsUnderPoint(point); };
				mDragHangle.onMoved = THIS_FUNC(OnDragHandleMoved);
				mDragHangle.onCursorPressed = THIS_FUNC(OnMoveHandlePressed);
				mDragHangle.onCursorReleased = THIS_FUNC(OnMoveHandleReleased);
			}
		}
	}

	void IntegerProperty::UpdateValueView()
	{
		if (mValuesDifferent)
			mEditBox->text = "--";
		else
			mEditBox->text = (WString)mCommonValue;
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
		if (!mEditBox)
			return;

		if (mEditBox && !mEditBox->IsFocused())
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
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<int>);

DECLARE_CLASS(Editor::IntegerProperty);
