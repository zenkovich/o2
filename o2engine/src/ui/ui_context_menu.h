#pragma once

#include "render_system/texture.h"
#include "ui_drawables_list_widget.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class UIButton;

class UIContextMenu: public UIDrawablesListWidget
{
	UIButton*  mButtonSample;
	UIWidget*  mDelimerSample;

public:
	DEFINE_TYPE(UIContextMenu);

	/** ctor. */
	UIContextMenu(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIContextMenu(const UIContextMenu& contextMenu);

	/** dtor. */
	virtual ~UIContextMenu();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Adding button element. If position negative, adding at end. */
	UIButton* AddButtonElement(const WString& caption, const String& key = "", const String& iconTextureName = "",
							   int position = -1);

	/* Adding delimer element. If position negative, adding at end. */
	UIWidget* AddDelimer(int position = -1);

	/** Adding custom widget element. If position negative, adding at end. */
	UIWidget* AddElement(UIWidget* widget, int position = -1);

	/** Setting up samples elements. */
	void SetupElementsSamples(UIWidget* delimer, UIButton* button);

protected:
	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg);
};

CLOSE_O2_NAMESPACE
