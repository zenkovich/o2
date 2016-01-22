#include "Button.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
	UIButton::UIButton():
		UIWidget(), mIconSprite(nullptr), mCaptionText(nullptr), mButtonGroup(nullptr)
	{
		InitializeProperties();
	}

	UIButton::UIButton(const UIButton& other):
		UIWidget(other), mButtonGroup(nullptr)
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

		if (IsUnderPoint(cursor.mPosition) && !mButtonGroup)
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
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;

		if (mButtonGroup && mButtonGroup->mPressed)
			onClick();
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

		if (shortcut.IsPressed())
			onClick();
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

	void UIButton::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetType() == *Text::type)
			mCaptionText = (Text*)layer->drawable;

		if (layer->name == "icon" && layer->drawable && layer->drawable->GetType() == *Sprite::type)
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