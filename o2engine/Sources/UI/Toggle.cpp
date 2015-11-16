#include "Toggle.h"

#include "Render/Sprite.h"
#include "Render/Text.h"

namespace o2
{
	IOBJECT_CPP(UIToggle);

	UIToggle::UIToggle():
		mValue(false)
	{
		InitializeProperties();
	}

	UIToggle::UIToggle(const UIToggle& other):
		UIWidget(other)
	{
		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		SetValue(other.mValue);
		InitializeProperties();
	}

	UIToggle& UIToggle::operator=(const UIToggle& other)
	{
		UIWidget::operator=(other);
		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = GetLayer("back");
		SetValue(other.mValue);
		RetargetStatesAnimations();
		return *this;
	}

	void UIToggle::SetCaption(const WString& text)
	{
		if (mCaptionText)
			mCaptionText->SetText(text);
	}

	WString UIToggle::GetCaption() const
	{
		if (mCaptionText)
			return mCaptionText->GetText();

		return WString();
	}

	void UIToggle::SetValue(bool value)
	{
		mValue = value;

		auto valueState = state["value"];
		if (valueState)
			*valueState = mValue;
	}

	bool UIToggle::GetValue() const
	{
		return mValue;
	}

	bool UIToggle::IsUnderPoint(const Vec2F& point)
	{
		if (mBackLayer && mBackLayer->IsUnderPoint(point))
			return true;

		if (mCaptionText && mCaptionText->IsPointInside(point))
			return true;

		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UIToggle::Depth()
	{
		return GetMaxDrawingDepth();
	}

	void UIToggle::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void UIToggle::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition))
		{
			onClick();
			SetValue(!mValue);
		}
	}

	void UIToggle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIToggle::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIToggle::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UIToggle::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetTypeId() == Text::type->ID())
			mCaptionText = layer->drawable.Cast<Text>();

		if (layer->name == "back")
			mBackLayer = layer;
	}

	void UIToggle::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIToggle, caption, SetCaption, GetCaption);
		INITIALIZE_PROPERTY(UIToggle, value, SetValue, GetValue);
	}
}