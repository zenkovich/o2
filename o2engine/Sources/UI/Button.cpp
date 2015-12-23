#include "Button.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
	UIButton::UIButton():
		UIWidget()
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
		UIWidget::operator=(other);
		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");
		RetargetStatesAnimations();
		return *this;
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

	void UIButton::SetIcon(Ptr<Sprite> sprite)
	{
		if (mIconSprite)
			mIconSprite = sprite;
	}

	Ptr<Sprite> UIButton::GetIcon() const
	{
		if (mIconSprite)
			return mIconSprite;

		return nullptr;
	}

	bool UIButton::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UIButton::Depth()
	{
		return GetMaxDrawingDepth();
	}

	bool UIButton::IsSelectable() const
	{
		return true;
	}

	void UIButton::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.SelectWidget(this);
	}

	void UIButton::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition))
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
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIButton::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UIButton::OnKeyPressed(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}
	}

	void UIButton::OnKeyReleased(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			onClick();
		}
	}

	void UIButton::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetType() == *Text::type)
			mCaptionText = layer->drawable.Cast<Text>();

		if (layer->name == "icon" && layer->drawable && layer->drawable->GetType() == *Sprite::type)
			mIconSprite = layer->drawable.Cast<Sprite>();
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