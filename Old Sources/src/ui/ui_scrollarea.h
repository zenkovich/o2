#pragma once

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class UIScrollBar;

class UIScrollArea: public UIDrawablesListWidget
{
	friend class UIStdSkinInitializer;

	UIScrollBar* mHorScrollbar;   /** Horizontal scroll bar child widget. */
	UIScrollBar* mVerScrollbar;   /** Vertical scroll bar child widget. */
	Vec2F        mSmoothScroll;   /** Smooth scroll. */
	Vec2F        mScroll;         /** Current scrolling. */
	Vec2F        mScrollSpeed;    /** Current scrolling speed. */
	Layout       mClippingLayout; /** Current clipping layout. */

public:
	//Type definition
	DEFINE_TYPE(UIScrollArea);

	CallbackChain onScrolled;

	//properties
	PROPERTY(UIScrollArea, Vec2F) scroll; /**< Scrolled offset property. Using setScroll/getScroll. */


	/** ctor. */
	UIScrollArea(const Layout& layout, UIScrollBar* horBarSample = NULL, UIScrollBar* verBarSample = NULL, 
		         const String& id = "");

	/** copy-ctor. */
	UIScrollArea(const UIScrollArea& scrollarea);

	/** dtor. */
	virtual ~UIScrollArea();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Draw widget and childes. */
	virtual void Draw();

	/** Returns true, if widget can take focus. */
	virtual bool IsFocusable() const;

	/** Sets horizontal scroll bar sample. */
	void SetHorScrollbar(UIScrollBar* scrollbar);

	/** Sets vertical scroll bar sample. */
	void SetVerScrollbar(UIScrollBar* scrollbar);

	/** Sets current scroll. */
	void SetScroll(const Vec2F& scroll);

	/** Returns current scroll. */
	Vec2F GetScroll() const;

protected:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void UpdateLayout();

	/** Updating current widget. */
	virtual void LocalUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg);

	/** Calls when widget focused. */
	virtual void OnFocused();

	/** Calls when widget lost focus. */
	virtual void OnFocusLost();

	/** Calls when hor or vertical bar moved. */
	void ScrollChanged();

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
