#include "Toggle.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
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

	bool UIToggle::IsSelectable() const
	{
		return true;
	}

	void UIToggle::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.SelectWidget(this);
	}

	void UIToggle::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition))
		{
			SetValue(!mValue);
			onClick();
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

	void UIToggle::OnKeyPressed(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}
	}

	void UIToggle::OnKeyReleased(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			SetValue(!mValue);
			onClick();
		}
	}

	void UIToggle::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetType() == *Text::type)
			mCaptionText = layer->drawable.Cast<Text>();

		if (layer->name == "back")
			mBackLayer = layer;
	}

	void UIToggle::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIToggle::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIToggle, caption, SetCaption, GetCaption);
		INITIALIZE_PROPERTY(UIToggle, value, SetValue, GetValue);
	}
}