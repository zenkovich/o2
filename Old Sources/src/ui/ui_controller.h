#pragma once

#include "public.h"

#include "ui_widget.h"
#include "ui_hint_controller.h"

OPEN_O2_NAMESPACE

/** User interface controller. Processing and drawing widgets. */
class UIController
{
	friend class UIStdSkinInitializer;

	typedef Array<UIWidget*> WidgetsArr;

	UIWidget         mBasicWidget;        /** Basic widget. */
	UIHintController mHintController;     /** Hint controller. */

	UIWidget*        mFocusWidget;        /** Focused widget. */
	bool             mChangedFocusWidget; /** True, if focus widget need to change. */

public:
	/** ctor. */
	UIController();

	/* dtor. */
	~UIController();

	/** Updating widgets and processing input message. */
	void Update(float dt);

	/** Drawing widgets. */
	void Draw();

	/** Adding widget. */
	UIWidget* AddWidget(UIWidget* widget);

	/** Adding widget and returning type. */
	template<typename T>
	T* AddTWidget(T* widget) 
	{
		return static_cast<T*>(AddWidget(widget));
	}

	/** Removing widget. */
	bool RemoveWidget(UIWidget* widget);

	/** Removing all widgets. */
	bool RemoveAllWidgets();

	/** Returns widget with id path. */
	UIWidget* GetWidget(const String& idPath);

	/** Sets focus on specified widget. */
	void FocusOnWidget(UIWidget* widget);

	/** Returns widget with id path, casting to reque type. */
	template<typename T>
	T* GetWidgetByType(const String& idPath)
	{
		return GetWidget(idPath);
	}
	
	/** Shows hint. */
	void ShowHint(const String& hintText, const Vec2F& position);

	/** Hides hint. */
	void HideHint();

	/** Returns client screen rect size. */
	Vec2F GetClientRectSize() const;

private:
};

CLOSE_O2_NAMESPACE
