#include "ui_button.h"

#include "app/application.h"
#include "render_system/text.h"
#include "ui_controller.h"
#include "util/graphics/stretch_rect.h"
#include "util/string.h"
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIButton);

UIButton::UIButton(const Layout& layout, const String& id /*= ""*/):
	UIDrawablesListWidget(layout, id), mHoverState(NULL), mFocusedState(NULL), mPressedState(NULL), mPressed(false),
	mPressedByButton(false), mHover(false), mUnderCursorTime(0), mHinting(false)
{
	InitializeProperties();
}

UIButton::UIButton(const UIButton& button):
	UIDrawablesListWidget(button), mPressed(false), mPressedByButton(false), mHover(false), mUnderCursorTime(0), 
	mHinting(false)
{	
	onClickEvent = button.onClickEvent;
	onHoverEvent = button.onHoverEvent;
	onHoverOffEvent = button.onHoverOffEvent;
	onFocusedEvent = button.onFocusedEvent;
	onFocusLostEvent = button.onFocusLostEvent;

	mHoverState = GetState("hover");
	mPressedState = GetState("pressed");
	mFocusedState = GetState("focus");

	mCaption = static_cast<Text*>(GetDrawable("caption")->GetDrawable());
	mHint = "Button copy hint";

	InitializeProperties();
}

UIButton::~UIButton()
{
}

UIWidget* UIButton::Clone() const
{
	return mnew UIButton(*this);
}

bool UIButton::IsFocusable() const
{
	return !(mFocusedState);
}

void UIButton::AddedState(UIState* state)
{
	if (state->GetName() == "hover")
		mHoverState = state;
	else if (state->GetName() == "focus")
		mFocusedState = state;
	else if (state->GetName() == "pressed")
		mPressedState = state;
}

void UIButton::LocalUpdate(float dt)
{
	if (mHoverState)
		mHoverState->SetState(mCursorInside);

	if (mCursorInside && !mHover)
		onHoverEvent.Invoke();

	if (!mCursorInside && mHover)
		onHoverOffEvent.Invoke();

	mHover = mCursorInside;

	if (mPressedState)
		mPressedState->SetState((mCursorInside || mPressedByButton) && mPressed);

	if (mFocusedState)
		mFocusedState->SetState(mFocused && !mPressed);

	if (!mCursorInside) 
	{
		mUnderCursorTime = 0.0f;
		if (mHinting)
			AppUI()->HideHint();
		mHinting = false;
	}
	else
		mUnderCursorTime += dt;

	if (mUnderCursorTime > 1.0f && !mHint.empty() && !mHinting)
	{
		AppUI()->ShowHint(mHint, AppInput()->GetCursorPos());
		mHinting = true;
	}
}

bool UIButton::LocalProcessInputMessage(const InputMessage& msg)
{
	/*hlog("button processing %.i cursorPos %i %i cursor %i", 
		timeUtils()->getCurrentFrame(), (int)(appInput()->getCursorPos()).x, (int)(appInput()->getCursorPos()).y,
		(int)mPressedByButton);*/

	bool pressedCursor = msg.IsCursorPressed() && mCursorInside;
	bool pressedKey = mFocused && (msg.IsKeyPressed(VK_SPACE) || msg.IsKeyPressed(VK_RETURN));

	if (pressedCursor || pressedKey)
	{
		if (mHinting)
			AppUI()->HideHint();

		mPressed = true;
		mPressedByButton = pressedKey;

		MakeFocused();
	}
	else 
	{
		bool releasedCursor = msg.IsCursorReleased() && mFocused;
		bool releasedKey = mFocused && (msg.IsKeyReleased(VK_SPACE) || msg.IsKeyReleased(VK_RETURN));

		if (releasedCursor || releasedKey)
		{
			mPressed = false;

			if (mCursorInside || releasedKey)
			{
				OnClicked();
				onClickEvent.Invoke();
				if (!mPressedByButton)
					ReleaseFocus();
			}
		}
	}

	return false;
}

void UIButton::OnClicked()
{
}

void UIButton::OnFocused()
{
	UIWidget::OnFocused();
	onFocusedEvent.Invoke();
}

void UIButton::OnFocusLost()
{
	UIWidget::OnFocusLost();

	if (mPressedState)
		mPressedState->SetState(false);

	onFocusLostEvent.Invoke();
}

void UIButton::AddedDrawable(Drawable* drawable)
{
	if (drawable->GetName() == "caption")
		mCaption = static_cast<Text*>(drawable->GetDrawable());
}

void UIButton::SetCCaption( const String& caption )
{
	if (mCaption)
		mCaption->SetCText(caption);
}

String UIButton::GetCCaption() const
{
	if (mCaption)
		return mCaption->GetCText();

	return "";
}

void UIButton::SetCaption( const WString& caption )
{
	if (mCaption)
		mCaption->GetText(caption);
}

WString UIButton::GetCaption() const
{
	if (mCaption)
		return mCaption->GetText();

	return ConvString2Wide("");
}

void UIButton::InitializeProperties()
{
	REG_PROPERTY(UIButton, ccaption, SetCCaption, GetCCaption);
	REG_PROPERTY(UIButton, caption, SetCaption, GetCaption);
}

CLOSE_O2_NAMESPACE
