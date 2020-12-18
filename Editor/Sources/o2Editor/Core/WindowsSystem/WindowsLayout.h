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

	class WindowsLayout: public ISerializable
	{
	public:
		class WindowDockPlace: public ISerializable
		{
		public:
			RectF                   anchors; // @SERIALIZABLE
			Vector<String>          windows; // @SERIALIZABLE
			String                  active;  // @SERIALIZABLE
			Vector<WindowDockPlace> childs;	 // @SERIALIZABLE

		public:
			void RetrieveLayout(Widget* widget);

			bool operator==(const WindowDockPlace& other) const;

			SERIALIZABLE(WindowDockPlace);
		};

	public:
		WindowDockPlace           mainDock; // @SERIALIZABLE
		Map<String, WidgetLayout> windows;  // @SERIALIZABLE

		bool operator==(const WindowsLayout& other) const;

		SERIALIZABLE(WindowsLayout);

	protected:
		// Restores dock recursively
		void RestoreDock(WindowDockPlace* dockDef, DockWindowPlace* dockWidget);

		// Removes all children empty dock places
		void CleanEmptyDocks(DockWindowPlace* dockPlace);

		friend class WindowsManager;
	};

}

CLASS_BASES_META(Editor::WindowsLayout)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::WindowsLayout)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mainDock).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(windows).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout)
{

	PROTECTED_FUNCTION(void, RestoreDock, WindowDockPlace*, DockWindowPlace*);
	PROTECTED_FUNCTION(void, CleanEmptyDocks, DockWindowPlace*);
}
END_META;

CLASS_BASES_META(Editor::WindowsLayout::WindowDockPlace)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::WindowsLayout::WindowDockPlace)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(anchors).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(windows).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(active).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(childs).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout::WindowDockPlace)
{

	PUBLIC_FUNCTION(void, RetrieveLayout, Widget*);
}
END_META;
