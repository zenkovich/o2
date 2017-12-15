#include "Button.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

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
		UIWidget::operator=(other);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mIconSprite = GetLayerDrawable<Sprite>("icon");
		RetargetStatesAnimations();

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

	void UIButton::SetButtonGroup(UIButtonGroup* group)
	{
		if (mButtonGroup)
		{
			if (mButtonGroup->mOwner == this)
			{
				if (mButtonGroup->mButtons.Count() == 1)
				{
					mButtonGroup->mButtons.Clear();
					delete mButtonGroup;
				}
				else
				{
					mButtonGroup->mButtons.Remove(this);
					mButtonGroup->mOwner = mButtonGroup->mButtons[0];
				}
			}
			else mButtonGroup->mButtons.Remove(this);
		}

		mButtonGroup = group;
		mButtonGroup->mButtons.Add(this);

		if (!mButtonGroup->mOwner)
		{
			mButtonGroup->mOwner = this;
		}
	}

	UIButtonGroup* UIButton::GetButtonGroup() const
	{
		return mButtonGroup;
	}

	bool UIButton::IsFocusable() const
	{
		return true;
	}

	bool UIButton::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void UIButton::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.FocusWidget(this);

		if (mButtonGroup)
		{
			mButtonGroup->mPressed = true;
			onClick();
		}
	}

	void UIButton::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (UIWidget::IsUnderPoint(cursor.position) && !mButtonGroup)
			onClick();

		if (mButtonGroup && mButtonGroup->mPressed)
			mButtonGroup->mPressed = false;
	}

	void UIButton::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (mButtonGroup && mButtonGroup->mPressed)
			mButtonGroup->mPressed = false;
	}

	void UIButton::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;

		if (mButtonGroup && mButtonGroup->mPressed)
			onClick();
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
		INITIALIZE_PROPERTY(UIButton, buttonsGroup, SetButtonGroup, GetButtonGroup);
	}

	UIButtonGroup::UIButtonGroup():
		mPressed(false), mOwner(nullptr)
	{}

	UIButtonGroup::~UIButtonGroup()
	{
		for (auto btn : mButtons)
			btn->mButtonGroup = nullptr;
	}

	void UIButtonGroup::AddButton(UIButton* toggle)
	{
		mButtons.Add(toggle);
		toggle->mButtonGroup = this;
	}

	void UIButtonGroup::RemoveButton(UIButton* toggle)
	{
		mButtons.Remove(toggle);
		toggle->mButtonGroup = nullptr;
	}

	const UIButtonGroup::ButtonsVec& UIButtonGroup::GetButtons() const
	{
		return mButtons;
	}
}

CLASS_META(o2::UIButton)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::CursorAreaEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);

	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(icon);
	PUBLIC_FIELD(buttonsGroup);
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(shortcut);
	PROTECTED_FIELD(mCaptionText);
	PROTECTED_FIELD(mIconSprite);
	PROTECTED_FIELD(mButtonGroup);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(void, SetButtonGroup, UIButtonGroup*);
	PUBLIC_FUNCTION(UIButtonGroup*, GetButtonGroup);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
