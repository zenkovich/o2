#pragma once

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class UIScrollBar: public UIDrawablesListWidget
{	
	friend class UIStdSkinInitializer;

public:
	enum Type { TP_HORISONTAL = 0, TP_VERTICAL };

protected:
	float     mMinValue;        /** Min value. */
	float     mMaxValue;        /** Max value. */
	float     mValue;           /** Current value. */
	float     mBarSize;         /** Current bar size. */
	Type      mType;            /** Scrolling type. */

	UIState*  mBarHoverState;   /** Bar hover state. */
	UIState*  mBarPressedState; /** Bar pressed state. */

	Drawable* mBar;             /** Bar drawable. */
	Layout    mBarGeometry;     /** bar clicking geometry. */
	Layout    mBackgrGeometry;  /** Background geometry. */
	 
	bool      mPressed;         /** True, when bar pressed. */
	bool      mHover;           /** True, when cursor hover bar. */

public:
	DEFINE_TYPE(UIScrollBar);

	PROPERTY(UIScrollBar, float) minValue; /** Min value property. Uses set/getMinValue. */
	PROPERTY(UIScrollBar, float) maxValue; /** Max value property. Uses set/getMaxValue. */
	PROPERTY(UIScrollBar, float) value;    /** Current value. Uses set/getValue. */
	PROPERTY(UIScrollBar, float) barSize;  /** Current bar size. */

	CallbackChain onValueChangedEvent; /** On changed value event. */

	/** ctor. */
	UIScrollBar(const Layout& layout, const String& id = "", Type type = TP_HORISONTAL);

	/** copy-ctor. */
	UIScrollBar(const UIScrollBar& scrollbar);

	/** dtor. */
	virtual ~UIScrollBar();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

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

	/** Sets current value with clamping by min and max. */
	void SetValueClamped(float value);

	/** Returns current value. */
	float GetValue() const;

	/* Sets bar size. */
	void SetBarSize(float size);

	/** Returns bar size. */
	float GetBarSize() const;

	/** Sets bar geomatry layout. */
	void GetBarGeometryLayout(const Layout& layout);

	/** Sets background geometry layout. */
	void SetBackgroundGeometryLayout(const Layout& layout);

protected:
	/** Returns bar drawable. */
	Drawable* GetBarDrawable();

	/** Sets bar drawable. */
	void SetBarDrawable(Drawable* drawable);

	/** Updating current widget. */
	virtual void LocalUpdate(float dt);

	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg);

	/** Returns true, if point inside current widget. */
	virtual bool IsLocalInside(const Vec2F& point) const;

	/** Updates bar drawable layout. */
	void UpdateBarLayout();

	/** Initialize properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
