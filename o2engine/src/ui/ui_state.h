#pragma once

#include <map>

#include "public.h"
#include "ui_widget.h"
#include "util/callback.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

/** Widget state interface. Widget can get bool parameter and change some parameters from widget. */
class UIState
{
	friend class UIWidget;
	friend class UIButton;
	friend class UIDrawablesListWidget;

protected:
	String    mName;        /** Name of state. */
	UIWidget* mOwnerWidget; /** Owner widget. */

public:
	PROPERTY(UIState, bool) state; /** State property. Using set/getState. */

	CallbackChain onActiveStateEvent;        /** On state active callbacks. Calls when active state completely setted. */
	CallbackChain onBeginActiveStateEvent;   /** On state begin active callbacks. Calls when state beginning. */
	CallbackChain onDeactiveStateEvent;      /** On state deactive callbacks. Calls when active state completely setted. */
	CallbackChain onBeginDeactiveStateEvent; /** On state begin deactive callbacks. Calls when state beginning. */


	/** ctor. */
	UIState(const String& name);

	/** copy-ctor. */
	UIState(const UIState& state);

	/** dtor. */
	virtual ~UIState();

	/** Returns clone of state. */
	virtual UIState* Clone() const = 0;

	/** Setting state. */
	virtual void SetState(bool state, bool forcible = false) {}

	/** Returns state. */
	virtual bool GetState() const { return false; }

	/** Updating. */
	virtual void Update(float dt) {}

	/** Returns state name. */
	String GetName() const;

protected:
	/** Calls when setting owner widget. */
	virtual void SetOwnerWidget(UIWidget* ownerWidget);

	/** Sets state. */
	void SetStateNonForcible(bool state);

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE
