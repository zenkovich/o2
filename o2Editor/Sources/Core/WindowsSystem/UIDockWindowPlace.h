#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "UI/Widget.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Math/Layout.h"
#include "Utils/Math/Rect.h"

using namespace o2;

namespace Editor
{
	class UIDockableWindow;

	// ------------------------------------
	// Dockable windows place for attaching
	// ------------------------------------
	class UIDockWindowPlace: public o2::UIWidget, public DrawableCursorEventsListener
	{
	public:
		// Default constructor
		UIDockWindowPlace();

		// Copy-constructor
		UIDockWindowPlace(const UIDockWindowPlace& other);

		// Destructor
		~UIDockWindowPlace();

		// Copy-operator
		UIDockWindowPlace& operator=(const UIDockWindowPlace& other);

		// Draws widget
		void Draw() override;

		// Sets resizible side and configures drag handle when draggable is true
		void SetResizibleDir(TwoDirection dir, float border,
							 UIDockWindowPlace* neighborMin, UIDockWindowPlace* neighborMax);

		// Returns resizible side
		TwoDirection GetResizibleDir() const;

		// Arranging child windows as tabs
		void ArrangeChildWindows();

		// Sets target window as active tab
		void SetActiveTab(UIDockableWindow* window);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Updates layout
		void UpdateLayout(bool withChildren = true) override;

		SERIALIZABLE(UIDockWindowPlace);

	protected:
		TwoDirection       mResizibleDir;        // Resizible dragable side

		UIDockWindowPlace* mNeighborMin;         // Resizing neighbor, using when dragging this side
		CursorEventsArea   mDragHandleMin;       // Separator drag handle
		Layout             mDragHandleLayoutMin; // Separator drag handle layout
		RectF              mDragHandleAreaMin;   // Separator drag handle area calculated from mDragHandleLayout

		UIDockWindowPlace* mNeighborMax;         // Resizing neighbor, using when dragging this side
		CursorEventsArea   mDragHandleMax;       // Separator drag handle
		Layout             mDragHandleLayoutMax; // Separator drag handle layout
		RectF              mDragHandleAreaMax;   // Separator drag handle area calculated from mDragHandleLayout

	protected:
		// It is called when cursor drag handle was moved
		void OnDragHandleMinMoved(const Vec2F& delta);

		// It is called when cursor drag handle was moved
		void OnDragHandleMaxMoved(const Vec2F& delta);

		// Checks interactable. If childs more than 0 this will be not interactable
		void CheckInteractable();

		// Initializes drag handle
		void InitializeDragHandle();

		friend class UIDockableWindow;
		friend class WindowsLayout;
	};
}
