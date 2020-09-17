#include "o2/stdafx.h"
#include "Button.h"

#include "o2/Render/Text.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{
	Button::Button():
		Widget(), CursorAreaEventsListener()
	{}

	Button::Button(const Button& other) :
		Widget(other), caption(this), icon(this)
	{
		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");

		RetargetStatesAnimations();

		if (mInteractable)
			OnBecomeInteractable();
		else
			OnBecomeNotInteractable();
	}

	Button& Button::operator=(const Button& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void Button::Draw()
	{
		Widget::Draw();
		CursorAreaEventsListener::OnDrawn();
	}

	void Button::SetCaption(const WString& text)
	{
		if (mCaptionText)
			mCaptionText->SetText(text);
	}

	WString Button::GetCaption() const
	{
		if (mCaptionText)
			return mCaptionText->GetText();

		return WString();
	}

	void Button::SetIcon(Sprite* sprite)
	{
		if (mIconSprite)
			mIconSprite = sprite;
	}

	Sprite* Button::GetIcon() const
	{
		if (mIconSprite)
			return mIconSprite;

		return nullptr;
	}

	bool Button::IsFocusable() const
	{
		return true;
	}

	bool Button::IsUnderPoint(const Vec2F& point)
	{
		if (isPointInside.IsEmpty())
			return Widget::IsUnderPoint(point);
		
		return mDrawingScissorRect.IsInside(point) && isPointInside(point);
	}

	String Button::GetCreateMenuGroup()
	{
		return "Basic";
	}

	void Button::CopyData(const Actor& otherActor)
	{
		const Button& other = dynamic_cast<const Button&>(otherActor);

		Widget::CopyData(other);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");
		RetargetStatesAnimations();
	}

	void Button::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.FocusWidget(this);
	}

	void Button::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.position))
			onClick();
	}

	void Button::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void Button::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void Button::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void Button::OnKeyPressed(const Input::Key& key)
	{
		if (mIsFocused && (key.keyCode == VK_SPACE || key.keyCode == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}

		if (shortcut.IsPressed())
			onClick();
	}

	void Button::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && (key.keyCode == VK_SPACE || key.keyCode == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			onClick();
		}
	}

	void Button::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mCaptionText = (Text*)layer->GetDrawable();

		if (layer->name == "icon" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Sprite))
			mIconSprite = (Sprite*)layer->GetDrawable();
	}

	void Button::OnEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	void Button::OnBecomeInteractable()
	{
		if (auto inactiveState = state["inactive"])
			*inactiveState = false;
	}

	void Button::OnBecomeNotInteractable()
	{
		if (auto inactiveState = state["inactive"])
			*inactiveState = true;
	}

}

DECLARE_CLASS(o2::Button);
