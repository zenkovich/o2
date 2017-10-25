#pragma once

#include "Utils/Singleton.h"
#include "Events/ApplicationEventsListener.h"

using namespace o2;

namespace o2
{
	class UIWidget;
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
		UIWidget* AddWidget(UIWidget* widget);

		// Removes widget from root
		void RemoveWidget(UIWidget* widget);

		// Removes all widgets from root
		void RemoveAllWidgets();

		// Returns root widget
		UIWidget* GetRootWidget();

	private:
		UIWidget* mRootWidget = nullptr;

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
