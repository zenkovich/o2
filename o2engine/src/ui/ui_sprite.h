#pragma once

#include "ui_widget.h"
#include "render_system/sprite.h"

OPEN_O2_NAMESPACE

/** UI sprite widget. Just drawing a sprite. */
class UISprite:public UIWidget
{
public:
	Sprite mSprite; /** Sprite drawable. */

public:
	/** ctor. */
	UISprite(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UISprite(const UISprite& spriteWidget);

	/** dtor. */
	~UISprite();

	/** Returns clone of widget. */
	UIWidget* Clone() const;

private:
	/** Drawing current widget. */
	void LocalDraw();

	/** Calls when widget layout was updated. */
	void LayoutUpdated();

	/** Calls when parameter "transparency" was changed. */
	void TransparencyChanged();
};

CLOSE_O2_NAMESPACE
