#include "o2/stdafx.h"
#include "Toggle.h"

#include "o2/Render/Sprite.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{
	Toggle::Toggle():
		Widget(), DrawableCursorEventsListener(this)
	{}

	Toggle::Toggle(const Toggle& other) :
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

	Toggle& Toggle::operator=(const Toggle& other)
	{
		Widget::operator=(other);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = FindLayer("back");

		mValue = !other.mValue;
		SetValue(other.mValue);

		if (other.IsValueUnknown())
			SetValueUnknown();

		RetargetStatesAnimations();

		return *this;
	}

	Toggle::~Toggle()
	{
		if (mToggleGroup && mToggleGroup->mOwner == this)
			delete mToggleGroup;
	}

	void Toggle::Update(float dt)
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

	void Toggle::SetCaption(const WString& text)
	{
		if (mCaptionText)
			mCaptionText->SetText(text);
	}

	WString Toggle::GetCaption() const
	{
		if (mCaptionText)
			return mCaptionText->GetText();

		return WString();
	}

	void Toggle::SetValue(bool value)
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

	void Toggle::SetValueUnknown()
	{
		mValueUnknown = true;

		auto unknownState = state["unknown"];
		if (unknownState)
			*unknownState = true;

		auto valueState = state["value"];
		if (valueState)
			*valueState = false;
	}

	bool Toggle::IsValueUnknown() const
	{
		return mValueUnknown;
	}

	bool Toggle::GetValue() const
	{
		return mValue;
	}

	bool Toggle::IsFocusable() const
	{
		return true;
	}

	void Toggle::SetToggleGroup(ToggleGroup* toggleGroup)
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

	ToggleGroup* Toggle::GetToggleGroup() const
	{
		return mToggleGroup;
	}

	void Toggle::OnCursorPressed(const Input::Cursor& cursor)
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

	void Toggle::OnCursorReleased(const Input::Cursor& cursor)
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

	void Toggle::OnCursorPressBreak(const Input::Cursor& cursor)
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

	void Toggle::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void Toggle::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void Toggle::OnKeyPressed(const Input::Key& key)
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

	void Toggle::OnKeyReleased(const Input::Key& key)
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

	void Toggle::OnDeserialized(const DataValue& node)
	{
		Widget::OnDeserialized(node);

		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	void Toggle::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mCaptionText = (Text*)layer->GetDrawable();

		if (layer->name == "back")
			mBackLayer = layer;
	}

	String Toggle::GetCreateMenuGroup()
	{
		return "Basic";
	}

	void Toggle::OnEnableInHierarchyChanged()
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

	void ToggleGroup::AddToggle(Toggle* toggle)
	{
		mToggles.Add(toggle);
		toggle->mToggleGroup = this;
		toggle->SetValue(true);
	}

	void ToggleGroup::RemoveToggle(Toggle* toggle)
	{
		mToggles.Remove(toggle);
		toggle->mToggleGroup = nullptr;
	}

	const Vector<Toggle*>& ToggleGroup::GetToggles() const
	{
		return mToggles;
	}

	const Vector<Toggle*>& ToggleGroup::GetToggled() const
	{
		return mToggled;
	}

	void ToggleGroup::OnToggled(Toggle* toggle)
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

ENUM_META(o2::ToggleGroup::Type)
{
	ENUM_ENTRY(HorOneClick);
	ENUM_ENTRY(OnlySingleTrue);
	ENUM_ENTRY(VerOneClick);
}
END_ENUM_META;

DECLARE_CLASS(o2::Toggle);
