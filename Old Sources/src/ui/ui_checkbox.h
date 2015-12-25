#pragma once

#include "ui_button.h"
#include "util/callback.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class Text;

class UICheckBox: public UIButton
{
	UIState* mCheckedState;  /** Checked state, must have name "checked". Activates when checkbox checked. */
	bool     mChecked;       /** True, if checked. */

public:
	DEFINE_TYPE(UICheckBox);

	PROPERTY(UICheckBox, bool) checked;  /** Check property. Using set/isChecked. */

	CallbackChain onCheckOn;             /** On check on event. */
	CallbackChain onCheckOff;            /** On check off event. */


	/** ctor. */
	UICheckBox(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UICheckBox(const UICheckBox& checkbox);

	/** dtor. */
	~UICheckBox();

	/** Returns copy of widget. */
	UIWidget* Clone() const;

	/** Sets checkbox checking. */
	void SetChecked(bool checked);

	/** Returns true, when checked. */
	bool IsChecked() const;

protected:
	/** Calls when clicked. */
	virtual void OnClicked();

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
