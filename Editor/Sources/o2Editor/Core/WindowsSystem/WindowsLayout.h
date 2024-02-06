#pragma once

#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Serialization/Serializable.h"

using namespace o2;

namespace o2
{
	class Widget;
}

namespace Editor
{
	class DockWindowPlace;

	// ---------------------------------------
	// Class that stores layout of all windows
// ---------------------------------------
	class WindowsLayout : public ISerializable
	{
	public:
		class WindowDockPlace : public ISerializable
		{
		public:
			RectF                   anchors; // Anchors coefficients @SERIALIZABLE
			Vector<String>          windows; // List of windows inside this place @SERIALIZABLE
			String                  active;  // Name active window @SERIALIZABLE
			Vector<WindowDockPlace> childs;	 // Children places @SERIALIZABLE

		public:
			// Gets layout from widget 
			void RetrieveLayout(const Ref<Widget>& widget);

			// Compares two dock places
			bool operator==(const WindowDockPlace& other) const;

			SERIALIZABLE(WindowDockPlace);
		};

	public:
		WindowDockPlace           mainDock; // Root dock place @SERIALIZABLE
		Map<String, WidgetLayout> windows;  // List of non-docked windows @SERIALIZABLE

		// Equal operator
		bool operator==(const WindowsLayout& other) const;

		SERIALIZABLE(WindowsLayout);

	protected:
		// Restores dock recursively
		void RestoreDock(WindowDockPlace& dockDef, WindowDockPlace& dockWidget);

		// Removes all children empty dock places
		void CleanEmptyDocks(WindowDockPlace& dockPlace);

		friend class WindowsManager;
	};

}
// --- META ---

CLASS_BASES_META(Editor::WindowsLayout)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::WindowsLayout)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mainDock);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(windows);
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout)
{

	FUNCTION().PROTECTED().SIGNATURE(void, RestoreDock, WindowDockPlace*, DockWindowPlace*);
	FUNCTION().PROTECTED().SIGNATURE(void, CleanEmptyDocks, DockWindowPlace*);
}
END_META;

CLASS_BASES_META(Editor::WindowsLayout::WindowDockPlace)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::WindowsLayout::WindowDockPlace)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(anchors);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(windows);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(active);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(childs);
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout::WindowDockPlace)
{

	FUNCTION().PUBLIC().SIGNATURE(void, RetrieveLayout, Widget*);
}
END_META;
// --- END META ---
