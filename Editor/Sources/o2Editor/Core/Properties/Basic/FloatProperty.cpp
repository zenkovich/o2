#include "o2Editor/stdafx.h"
#include "FloatProperty.h"

#include "o2/Application/Application.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/Widgets/EditBox.h"


namespace Editor
{
	FloatProperty::FloatProperty()
	{}

	FloatProperty::FloatProperty(const FloatProperty& other) :
		TPropertyField<float>(other)
	{
		InitializeControls();
	}

	const Ref<EditBox>& FloatProperty::GetEditBox() const
	{
		return mEditBox;
	}

	FloatProperty& FloatProperty::operator=(const FloatProperty& other)
	{
		TPropertyField<float>::operator=(other);
		InitializeControls();
		return *this;
	}

	void FloatProperty::InitializeControls()
	{
		mEditBox = FindChildByType<EditBox>();
		if (mEditBox)
		{
			mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
			mEditBox->text = "--";
			mEditBox->SetFilterFloat();

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

	void FloatProperty::UpdateValueView()
	{
		if (!mEditBox)
			return;

		if (mValuesDifferent)
			mEditBox->text = "--";
		else
			mEditBox->text = (WString)mCommonValue;
	}

	void FloatProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValueByUser((const float)data);
	}

	void FloatProperty::OnDragHandleMoved(const Input::Cursor& cursor)
	{
		float multiplier = Math::Abs(cursor.delta.y) < 20 ? 0.01f : 0.05f;
		SetValue(mCommonValue + cursor.delta.y*multiplier);
	}

	void FloatProperty::OnKeyReleased(const Input::Key& key)
	{
		if (!mEditBox)
			return;

		if (mEditBox && !mEditBox->IsFocused())
			return;

		if (key == VK_UP)
		{
			SetValueByUser(Math::Ceil(mCommonValue + 0.01f));
			mEditBox->SelectAll();
		}

		if (key == VK_DOWN)
		{
			SetValueByUser(Math::Floor(mCommonValue - 0.01f));
			mEditBox->SelectAll();
		}
	}

	void FloatProperty::OnMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void FloatProperty::OnMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}
}

DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<float>);
DECLARE_TEMPLATE_CLASS(o2::Ref<Editor::FloatProperty>);
DECLARE_TEMPLATE_CLASS(o2::Ref<Editor::TPropertyField<float>>);
// --- META ---

DECLARE_CLASS(Editor::FloatProperty, Editor__FloatProperty);
// --- END META ---
