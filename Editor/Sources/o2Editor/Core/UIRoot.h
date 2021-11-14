#pragma once

#include "o2/Events/ApplicationEventsListener.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Widget;
}

// Editor UI root accessor macros
#define EditorUIRoot UIRoot::Instance()

namespace Editor
{
	// --------------
	// Editor UI root
	// --------------
	class UIRoot: public Singleton<UIRoot>, ApplicationEventsListener
	{
	public:
		// Adds widget to root
		Widget* AddWidget(Widget* widget);

		// Removes widget from root
		void RemoveWidget(Widget* widget);

		// Removes all widgets from root
		void RemoveAllWidgets();

		// Returns root widget
		Widget* GetRootWidget();

	private:
		Widget* mRootWidget = nullptr;

	private:
		// Default constructor, creates root widget
		UIRoot();

		// Destructor
		~UIRoot();

		// Draws root widget
		void Draw();

		// Updates root widget
		void Update(float dt);

		// It is called when application frame was resized, updates root size
		void OnApplicationSized() override;

		friend class EditorApplication;
	};
}
