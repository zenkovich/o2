#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "UI/Widget.h"
#include "Events/CursorEventsArea.h"
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
		void UpdateTransform(bool withChildren = true) override;

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

		friend class UIDockableWindow;
		friend class WindowsLayout;
	};
}

CLASS_BASES_META(Editor::UIDockWindowPlace)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::UIDockWindowPlace)
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
CLASS_METHODS_META(Editor::UIDockWindowPlace)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetResizibleDir, TwoDirection, float, UIDockWindowPlace*, UIDockWindowPlace*);
	PUBLIC_FUNCTION(TwoDirection, GetResizibleDir);
	PUBLIC_FUNCTION(void, ArrangeChildWindows);
	PUBLIC_FUNCTION(void, SetActiveTab, UIDockableWindow*);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDragHandleMinMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnDragHandleMaxMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, CheckInteractable);
	PROTECTED_FUNCTION(void, InitializeDragHandle);
}
END_META;
