#include "Button.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetState.h"

namespace o2
{
	UIButton::UIButton():
		UIWidget(), CursorAreaEventsListener()
	{
		InitializeProperties();
	}

	UIButton::UIButton(const UIButton& other):
		UIWidget(other)
	{
		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");

		RetargetStatesAnimations();
		InitializeProperties();
	}

	UIButton& UIButton::operator=(const UIButton& other)
	{
		CopyData(other);
		return *this;
	} 

	void UIButton::Draw()
	{
		UIWidget::Draw();
		CursorAreaEventsListener::OnDrawn();
	}

	void UIButton::SetCaption(const WString& text)
	{
		if (mCaptionText)
			mCaptionText->SetText(text);
	}

	WString UIButton::GetCaption() const
	{
		if (mCaptionText)
			return mCaptionText->GetText();

		return WString();
	}

	void UIButton::SetIcon(Sprite* sprite)
	{
		if (mIconSprite)
			mIconSprite = sprite;
	}

	Sprite* UIButton::GetIcon() const
	{
		if (mIconSprite)
			return mIconSprite;

		return nullptr;
	}

	bool UIButton::IsFocusable() const
	{
		return true;
	}

	bool UIButton::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void UIButton::CopyData(const Actor& otherActor)
	{
		const UIButton& other = dynamic_cast<const UIButton&>(otherActor);

		UIWidget::CopyData(other);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");
		RetargetStatesAnimations();
	}

	void UIButton::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.FocusWidget(this);
	}

	void UIButton::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (UIWidget::IsUnderPoint(cursor.position))
			onClick();
	}

	void UIButton::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIButton::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UIButton::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UIButton::OnKeyPressed(const Input::Key& key)
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

	void UIButton::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && (key.keyCode == VK_SPACE || key.keyCode == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			onClick();
		}
	}

	void UIButton::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetType() == TypeOf(Text))
			mCaptionText = (Text*)layer->drawable;

		if (layer->name == "icon" && layer->drawable && layer->drawable->GetType() == TypeOf(Sprite))
			mIconSprite = (Sprite*)layer->drawable;
	}

	void UIButton::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIButton::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIButton, caption, SetCaption, GetCaption);
		INITIALIZE_PROPERTY(UIButton, icon, SetIcon, GetIcon);
	}
}

DECLARE_CLASS(o2::UIButton);
