#pragma once

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class UIProgressBar: public UIDrawablesListWidget
{	
protected:
	float     mMinValue;   /** Min value. */
	float     mMaxValue;   /** Max value. */
	float     mValue;      /** Current value. */

	Drawable* mBar;        /** Bar drawable. */

public:
	PROPERTY(UIProgressBar, float) minValue; /** Min value property. Uses set/getMinValue. */
	PROPERTY(UIProgressBar, float) maxValue; /** Max value property. Uses set/getMaxValue. */
	PROPERTY(UIProgressBar, float) value;    /** Current value. Uses set/getValue. */

	/** ctor. */
	UIProgressBar(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIProgressBar(const UIProgressBar& widget);

	/** dtor. */
	virtual ~UIProgressBar();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Returns bar drawable. */
	Drawable* GetBarDrawable();

	/** Sets bar drawable. */
	void SetBarDrawable(Drawable* barDrawable);

	/** Sets value range. */
	void SetValueRange(float minValue, float maxValue);

	/** Sets min value. */
	void SetMinValue(float value);

	/** Sets max value. */
	void SetMaxValue(float value);

	/** Returns min value. */
	float GetMinValue() const;

	/** Returns max value. */
	float GetMaxValue() const;

	/** Sets current value. */
	void SetValue(float value);

	/** Returns current value. */
	float GetValue() const;

protected:
	/** Updating current widget. */
	virtual void LocalUpdate(float dt);

	/** Updates bar drawable layout. */
	void UpdateBarLayout();

	/** Initialize properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
