#pragma once

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class UIHorLayout: public UIWidget 
{
	float      mWidgetsDistance;
	WidgetsVec mFlexibleWidgets;

public:
	//Type definition
	DEFINE_TYPE(UIHorLayout);

	/** ctor. */
	UIHorLayout(const Layout& layout, float widgetsDistance = 10.0f, const String& id = "");

	/** copy-ctor. */
	UIHorLayout(const UIHorLayout& widget);

	/** dtor. */
	virtual ~UIHorLayout();

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
