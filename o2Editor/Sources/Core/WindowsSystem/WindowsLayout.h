#pragma once

#include "Scene/UI/WidgetLayout.h"
#include "Utils/Serialization/Serializable.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	class UIDockWindowPlace;

	class WindowsLayout: public ISerializable
	{
	public:
		class WindowDockPlace: public ISerializable
		{
		public:
			RectF                   anchors; // @SERIALIZABLE
			Vector<String>          windows; // @SERIALIZABLE
			Vector<WindowDockPlace> childs;	 // @SERIALIZABLE

			void RetrieveLayout(UIWidget* widget);

			bool operator==(const WindowDockPlace& other) const;

			SERIALIZABLE(WindowDockPlace);
		};

	public:
		WindowDockPlace                    mainDock; // @SERIALIZABLE
		Dictionary<String, UIWidgetLayout> windows;  // @SERIALIZABLE

		bool operator==(const WindowsLayout& other) const;

		SERIALIZABLE(WindowsLayout);

	protected:
		// Restores dock recursively
		void RestoreDock(WindowDockPlace* dockDef, UIDockWindowPlace* dockWidget);

		// Removes all children empty dock places
		void CleanEmptyDocks(UIDockWindowPlace* dockPlace);

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
	PUBLIC_FIELD(mainDock).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(windows).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout)
{

	PROTECTED_FUNCTION(void, RestoreDock, WindowDockPlace*, UIDockWindowPlace*);
	PROTECTED_FUNCTION(void, CleanEmptyDocks, UIDockWindowPlace*);
}
END_META;

CLASS_BASES_META(Editor::WindowsLayout::WindowDockPlace)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::WindowsLayout::WindowDockPlace)
{
	PUBLIC_FIELD(anchors).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(windows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(childs).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::WindowsLayout::WindowDockPlace)
{

	PUBLIC_FUNCTION(void, RetrieveLayout, UIWidget*);
}
END_META;
