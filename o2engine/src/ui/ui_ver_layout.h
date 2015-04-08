#pragma once

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class UIVerLayout: public UIWidget 
{
	float      mWidgetsDistance;
	WidgetsVec mFlexibleWidgets;

public:
	//Type definition
	DEFINE_TYPE(UIVerLayout);

	/** ctor. */
	UIVerLayout(const Layout& layout, float widgetsDistance = 10.0f, const String& id = "");

	/** copy-ctor. */
	UIVerLayout(const UIVerLayout& widget);

	/** dtor. */
	virtual ~UIVerLayout();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Adding child widget. If position negative, adding at end. */
	virtual UIWidget* AddChild(UIWidget* widget, int position = -1);

	/** Sets widgets distance. */
	void SetWidgetsDistance(float distance);

	/** Returns widgets distance. */
	float GetWidgetsDistance() const;

protected:
	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();
};

CLOSE_O2_NAMESPACE
