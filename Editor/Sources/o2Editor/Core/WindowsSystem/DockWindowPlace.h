#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Math/Layout.h"
#include "o2/Utils/Math/Rect.h"

using namespace o2;

namespace Editor
{
	class DockableWindow;

	// ------------------------------------
	// Dockable windows place for attaching
	// ------------------------------------
	class DockWindowPlace: public o2::Widget, public DrawableCursorEventsListener
	{
	public:
		// Default constructor
		DockWindowPlace();

		// Copy-constructor
		DockWindowPlace(const DockWindowPlace& other);

		// Destructor
		~DockWindowPlace();

		// Copy-operator
		DockWindowPlace& operator=(const DockWindowPlace& other);

		// Draws widget
		void Draw() override;

		// Sets resizible side and configures drag handle when draggable is true
		void SetResizibleDir(TwoDirection dir, float border,
							 DockWindowPlace* neighborMin, DockWindowPlace* neighborMax);

		// Returns resizible side
		TwoDirection GetResizibleDir() const;

		// Arranging child windows as tabs
		void ArrangeChildWindows();

		// Sets target window as active tab
		void SetActiveTab(DockableWindow* window);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(DockWindowPlace);

	protected:
		TwoDirection mResizibleDir; // Resizible dragable side

		DockWindowPlace* mNeighborMin;         // Resizing neighbor, using when dragging this side
		CursorEventsArea mDragHandleMin;       // Separator drag handle
		Layout           mDragHandleLayoutMin; // Separator drag handle layout
		RectF            mDragHandleAreaMin;   // Separator drag handle area calculated from mDragHandleLayout

		DockWindowPlace* mNeighborMax;         // Resizing neighbor, using when dragging this side
		CursorEventsArea mDragHandleMax;       // Separator drag handle
		Layout           mDragHandleLayoutMax; // Separator drag handle layout
		RectF            mDragHandleAreaMax;   // Separator drag handle area calculated from mDragHandleLayout

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when cursor drag handle was moved
		void OnDragHandleMinMoved(const Vec2F& delta);

		// It is called when cursor drag handle was moved
		void OnDragHandleMaxMoved(const Vec2F& delta);

		// Checks interactable. If childs more than 0 this will be not interactable
		void CheckInteractable();

		// Initializes drag handle
		void InitializeDragHandle();

		friend class DockableWindow;
		friend class WindowsLayout;
	};
}

CLASS_BASES_META(Editor::DockWindowPlace)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::DockWindowPlace)
{
	PROTECTED_FIELD(mResizibleDir);
	PROTECTED_FIELD(mNeighborMin);
	PROTECTED_FIELD(mDragHandleMin);
	PROTECTED_FIELD(mDragHandleLayoutMin);
	PROTECTED_FIELD(mDragHandleAreaMin);
	PROTECTED_FIELD(mNeighborMax);
	PROTECTED_FIELD(mDragHandleMax);
	PROTECTED_FIELD(mDragHandleLayoutMax);
	PROTECTED_FIELD(mDragHandleAreaMax);
}
END_META;
CLASS_METHODS_META(Editor::DockWindowPlace)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetResizibleDir, TwoDirection, float, DockWindowPlace*, DockWindowPlace*);
	PUBLIC_FUNCTION(TwoDirection, GetResizibleDir);
	PUBLIC_FUNCTION(void, ArrangeChildWindows);
	PUBLIC_FUNCTION(void, SetActiveTab, DockableWindow*);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDragHandleMinMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnDragHandleMaxMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, CheckInteractable);
	PROTECTED_FUNCTION(void, InitializeDragHandle);
}
END_META;
