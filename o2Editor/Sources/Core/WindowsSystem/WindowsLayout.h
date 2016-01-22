#pragma once

#include "UI/WidgetLayout.h"
#include "Utils/Serialization.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

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

	friend class WindowsManager;
};
