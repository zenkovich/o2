#include "stdafx.h"
#include "Toggle.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace o2
{
	UIToggle::UIToggle():
		Widget(), DrawableCursorEventsListener(this)
	{}

	UIToggle::UIToggle(const UIToggle& other) :
		Widget(other), DrawableCursorEventsListener(this), mToggleGroup(nullptr), value(this), caption(this),
		toggleGroup(this)
	{
		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();

		mValue = !other.mValue;
		SetValue(other.mValue);

		if (other.IsValueUnknown())
			SetValueUnknown();
	}

	UIToggle& UIToggle::operator=(const UIToggle& other)
	{
		Widget::operator=(other);
		return *this;
	}

	UIToggle::~UIToggle()
	{
		if (mToggleGroup && mToggleGroup->mOwner == this)
			delete mToggleGroup;
	}

	void UIToggle::Update(float dt)
	{
		Widget::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		if (mToggleGroup && mToggleGroup->mPressed && mToggleGroup->mPressedValue != mValue &&
			(mToggleGroup->mType == ToggleGroup::Type::VerOneClick || mToggleGroup->mType == ToggleGroup::Type::HorOneClick))
		{
			Vec2F cursor = o2Input.GetCursorPos();
			bool underPoint = false;

			if (mToggleGroup->mType == ToggleGroup::Type::VerOneClick)
				underPoint = cursor.y > layout->worldBottom && cursor.y < layout->worldTop;
			else
				underPoint = cursor.x > layout->worldLeft && cursor.x < layout->worldRight;

			if (underPoint)
			{
				SetValue(!mValue);
				onClick();
				onToggleByUser(mValue);
				mToggleGroup->mToggled.Add(this);
			}
		}
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
		if (mValue == value && !mValueUnknown)
			return;

		mValue = value;
		mValueUnknown = false;

		auto valueState = state["value"];
		if (valueState)
			*valueState = mValue;

		auto unknownState = state["unknown"];
		if (unknownState)
			*unknownState = false;

		onToggle(mValue);

		if (mToggleGroup)
			mToggleGroup->OnToggled(this);
	}

	void UIToggle::SetValueUnknown()
	{
		mValueUnknown = true;

		auto unknownState = state["unknown"];
		if (unknownState)
			*unknownState = true;

		auto valueState = state["value"];
		if (valueState)
			*valueState = false;
	}

	bool UIToggle::IsValueUnknown() const
	{
		return mValueUnknown;
	}

	bool UIToggle::GetValue() const
	{
		return mValue;
	}

	bool UIToggle::IsFocusable() const
	{
		return true;
	}

	void UIToggle::SetToggleGroup(ToggleGroup* toggleGroup)
	{
		if (mToggleGroup == toggleGroup)
			return;

		if (mToggleGroup)
		{
			if (mToggleGroup->mOwner == this)
			{
				if (mToggleGroup->mToggles.Count() == 1)
				{
					mToggleGroup->mToggles.Clear();
					delete mToggleGroup;
				}
				else
				{
					mToggleGroup->mToggles.Remove(this);
					mToggleGroup->mOwner = mToggleGroup->mToggles[0];
				}
			}
			else mToggleGroup->mToggles.Remove(this);
		}

		mToggleGroup = toggleGroup;
		mToggleGroup->mToggles.Add(this);

		if (!mToggleGroup->mOwner)
		{
			mToggleGroup->mOwner = this;

			if (mToggleGroup->mType == ToggleGroup::Type::OnlySingleTrue)
				SetValue(true);
		}
	}

	ToggleGroup* UIToggle::GetToggleGroup() const
	{
		return mToggleGroup;
	}

	void UIToggle::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.FocusWidget(this);

		if (mToggleGroup)
		{
			SetValue(!mValue);
			onClick();

			mToggleGroup->mPressed = true;
			mToggleGroup->mPressedValue = mValue;
			mToggleGroup->mToggled.Clear();
			mToggleGroup->mToggled.Add(this);
			mToggleGroup->onPressed(mValue);
		}
	}

	void UIToggle::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (Widget::IsUnderPoint(cursor.position) &&
			!(mToggleGroup && (mToggleGroup->mType == ToggleGroup::Type::VerOneClick ||
			mToggleGroup->mType == ToggleGroup::Type::HorOneClick) &&
			mToggleGroup->mPressed))
		{
			SetValue(!mValue);
			onClick();
			onToggleByUser(mValue);
		}

		if (mToggleGroup && (mToggleGroup->mType == ToggleGroup::Type::VerOneClick ||
			mToggleGroup->mType == ToggleGroup::Type::HorOneClick))
		{
			mToggleGroup->mPressed = false;
			mToggleGroup->onReleased(mValue);
		}
	}

	void UIToggle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (mToggleGroup && (mToggleGroup->mType == ToggleGroup::Type::VerOneClick ||
			mToggleGroup->mType == ToggleGroup::Type::HorOneClick))
		{
			mToggleGroup->mPressed = false;
			mToggleGroup->onReleased(mValue);
		}
	}

	void UIToggle::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UIToggle::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UIToggle::OnKeyPressed(const Input::Key& key)
	{
		if (mIsFocused && (key.keyCode == VK_SPACE || key.keyCode == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}

		bool isFocusedEditBox = o2UI.GetFocusedWidget() && o2UI.GetFocusedWidget()->GetType() == TypeOf(EditBox);
		if (shortcut.IsPressed() && !isFocusedEditBox)
		{
			SetValue(!mValue);
			onClick();
			onToggleByUser(mValue);
		}
	}

	void UIToggle::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && (key.keyCode == VK_SPACE || key.keyCode == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			SetValue(!mValue);
			onClick();
			onToggleByUser(mValue);
		}

	}

	void UIToggle::OnDeserialized(const DataNode& node)
	{
		Widget::OnDeserialized(node);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	void UIToggle::CopyData(const Actor& otherActor)
	{
		const UIToggle& other = dynamic_cast<const UIToggle&>(otherActor);

		Widget::CopyData(other);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = FindLayer("back");

		mValue = !other.mValue;
		SetValue(other.mValue);

		if (other.IsValueUnknown())
			SetValueUnknown();

		RetargetStatesAnimations();
	}

	void UIToggle::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mCaptionText = (Text*)layer->GetDrawable();

		if (layer->name == "back")
			mBackLayer = layer;
	}

	void UIToggle::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	ToggleGroup::ToggleGroup(Type type):
		mType(type), mPressed(false)
	{}

	ToggleGroup::~ToggleGroup()
	{
		for (auto toggle : mToggles)
			toggle->mToggleGroup = nullptr;
	}

	void ToggleGroup::AddToggle(UIToggle* toggle)
	{
		mToggles.Add(toggle);
		toggle->mToggleGroup = this;
		toggle->SetValue(true);
	}

	void ToggleGroup::RemoveToggle(UIToggle* toggle)
	{
		mToggles.Remove(toggle);
		toggle->mToggleGroup = nullptr;
	}

	const ToggleGroup::TogglesVec& ToggleGroup::GetToggles() const
	{
		return mToggles;
	}

	const ToggleGroup::TogglesVec& ToggleGroup::GetToggled() const
	{
		return mToggled;
	}

	void ToggleGroup::OnToggled(UIToggle* toggle)
	{
		if (mType == Type::OnlySingleTrue)
		{
			if (toggle->GetValue())
			{
				for (auto ctoggle : mToggles)
				{
					if (ctoggle == toggle)
						continue;

					ctoggle->SetValue(false);
				}
			}
			else
			{
				if (!mToggles.Any([&](auto x) { return x->GetValue(); }))
					toggle->SetValue(true);
			}
		}
	}
}

DECLARE_CLASS(o2::UIToggle);

ENUM_META_(o2::ToggleGroup::Type, Type)
{
	ENUM_ENTRY(HorOneClick);
	ENUM_ENTRY(OnlySingleTrue);
	ENUM_ENTRY(VerOneClick);
}
END_ENUM_META;
