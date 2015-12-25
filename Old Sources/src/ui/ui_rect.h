#pragma once

#include "ui_widget.h"
#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

/** UI rect. Widget that drawing cStretchRect. */
class UIRect:public UIWidget
{
public:
	DEFINE_TYPE(UIRect);

	StretchRect mStretchRect; /** Stretching rect drawable. */

public:
	/** ctor. */
	UIRect(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIRect(const UIRect& rectWidget);

	/** dtor. */
	~UIRect();

	/** Returns clone of widget. */
	UIWidget* Clone() const;

private:
	/** Drawing current widget. */
	void LocalDraw();

	/** Calls when widget layout updated. Here updating stretch rect position and size. */
	void LayoutUpdated();

	/** Calls when parameter "transparency" was changed. */
	void TransparencyChanged();
};

CLOSE_O2_NAMESPACE
