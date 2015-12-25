#pragma once

#include "ui_drawables_list_widget.h"

#include "util/callback.h"
#include "util/graphics/stretch_rect.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class Text;

class UIButton:public UIDrawablesListWidget
{
protected:
	UIState* mHoverState;    /** Hover state, must have name "hover". Activates when cursor hover widget. */
	UIState* mFocusedState;  /** Focused state, must have name "focus". Activates when widget is focused. */
	UIState* mPressedState;  /** Pressed state, must have name "pressed". Activates when button pressed. */

	Text*    mCaption;

	bool     mHover;
	bool     mPressed;
	bool     mPressedByButton;
	bool     mHinting;
	float    mUnderCursorTime;

public:
	DEFINE_TYPE(UIButton);

	PROPERTY(UIButton, String) ccaption; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(UIButton, WString) caption; /** Caption property. Using set/getCaption. */

	CallbackChain onClickEvent;     /** On click event. */
	CallbackChain onHoverEvent;     /** On hover event. */
	CallbackChain onHoverOffEvent;  /** On hover off event. */
	CallbackChain onFocusedEvent;   /** On focused event. */
	CallbackChain onFocusLostEvent; /** On focus lost event. */


	/** ctor. */
	UIButton(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIButton(const UIButton& button);

	/** dtor. */
	~UIButton();

	/** Returns copy of widget. */
	UIWidget* Clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool IsFocusable() const;

	/** Sets c text caption. */
	void SetCCaption(const String& caption);

	/** Returns c text caption. */
	String GetCCaption() const;

	/** Sets caption text. */
	void SetCaption(const WString& caption);

	/** Returns caption text. */
	WString GetCaption() const;

protected:
	/** Calls when added some state. */
	virtual void AddedState(UIState* state);

	/** Updating current widget. */
	virtual void LocalUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg);

	/** Calls when widget focused. */
	virtual void OnFocused();

	/** Calls when widget lost focus. */
	virtual void OnFocusLost();

	/** Calls when added drawable. */
	virtual void AddedDrawable(Drawable* drawable);

	/** Calls when clicked. */
	virtual void OnClicked();

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
