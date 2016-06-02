#pragma once

#include "Events/CursorEventsListener.h"

namespace o2
{
	class DragableObject;
	class ISelectableDragableObjectsGroup;

	// --------------------------------
	// Drag objects drop area interface
	// --------------------------------
	class DragDropArea: virtual public CursorAreaEventsListener
	{
	protected:
		// Calls when some DragEventsListener was dropped to this
		virtual void OnDropped(DragableObject* draggable);

		// Calls when some selectable listeners was dropped to this
		virtual void OnDropped(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listener was entered to this area
		virtual void OnDragEnter(DragableObject* draggable);

		// Calls when some drag listeners was entered to this area
		virtual void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listener was dragged above this area
		virtual void OnDraggedAbove(DragableObject* draggable);

		// Calls when some drag listeners was dragged above this area
		virtual void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listener was exited from this area
		virtual void OnDragExit(DragableObject* draggable);

		// Calls when some drag listeners was exited from this area
		virtual void OnDragExit(ISelectableDragableObjectsGroup* group);

		friend class DragableObject;
		friend class SelectableDragableObject;
	};

	// --------------------------
	// Draggable object interface
	// --------------------------
	class DragableObject: public CursorAreaEventsListener
	{
	public:
		// Default constructor
		DragableObject();

		// Destructor
		virtual ~DragableObject();

		// Returns true if it's dragging
		bool IsDragging() const;

		// Calls when listener was drawn
		virtual void OnDrawn();

		// Returns last cursor pressed point
		Vec2F GetCursorPressedPoint() const;

	protected:
		bool          mIsDragging = false;           // True when dragging
		float         mDragDistanceThreshold = 3.0f; // Drag distance threshold: object starts dragging when cursor moves more tan this distance
		Vec2F         mPressedCursorPos;             // Cursor pressed position
		int           mPressedCursorId;              // Id of pressed cursor

		DragDropArea* mDragDropArea = nullptr;       // Drag drop area under this when dragging

	protected:
		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		virtual void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when started dragging
		virtual void OnDragStart(const Input::Cursor& cursor);

		// Calls when dragged
		virtual void OnDragged(const Input::Cursor& cursor, DragDropArea* area);

		// Calls when dragging completed
		virtual void OnDragEnd(const Input::Cursor& cursor);

		// Calls when this was dropped
		virtual void OnDropped(DragDropArea* area);

		friend class EventSystem;
		friend class DragDropArea;
	};

	class SelectableDragableObject;

	// --------------------------------------------
	// Selectable draggable objects group interface
	// --------------------------------------------
	class ISelectableDragableObjectsGroup: virtual public CursorAreaEventsListener
	{
	public:
		typedef Vector<SelectableDragableObject*> SelectDragObjectsVec;

	public:
		// Returns selected objects in group
		virtual SelectDragObjectsVec GetSelectedDragObjects() const = 0;

		// Returns all objects in group 
		virtual SelectDragObjectsVec GetAllObjects() const = 0;

		// Selects object
		virtual void Select(SelectableDragableObject* object) = 0;

		// Deselects object
		virtual void Deselect(SelectableDragableObject* object) = 0;

		// Adds selectable object to group
		virtual void AddSelectableObject(SelectableDragableObject* object) = 0;

		// Removes selectable object from group
		virtual void RemoveSelectableObject(SelectableDragableObject* object) = 0;

		// Deselects all in group
		void DeselectAll();

		// Selects all in group
		void SelectAll();

	protected:
		// Calls when selectable draggable object was pressed
		virtual void OnSelectableObjectCursorPressed(SelectableDragableObject* object, const Input::Cursor& cursor) {}

		// Calls when selectable draggable object was released
		virtual void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) {}

		// Calls when selectable object was began to drag
		virtual void OnSelectableObjectBeganDragging(SelectableDragableObject* object) {}

		friend class SelectableDragableObject;
	};

	// ----------------------------------
	// Selectable draggable objects group
	// ----------------------------------
	class SelectableDragableObjectsGroup: public ISelectableDragableObjectsGroup
	{
	public:
		typedef Vector<SelectableDragableObject*> SelectDragObjectsVec;

	public:
		// Destructor
		virtual ~SelectableDragableObjectsGroup();

		// Returns selected listeners in group
		SelectDragObjectsVec GetSelectedDragObjects() const;

		// Returns all objects in group 
		SelectDragObjectsVec GetAllObjects() const;

		// Selects object
		void Select(SelectableDragableObject* object);

		// Deselects object
		void Deselect(SelectableDragableObject* object);

		// Adds selectable object to group
		void AddSelectableObject(SelectableDragableObject* object);

		// Removes selectable object from group
		void RemoveSelectableObject(SelectableDragableObject* object);

		// Returns true if point is in this object
		virtual bool IsUnderPoint(const Vec2F& point);

	protected:
		SelectDragObjectsVec mObjects;         // Listeners in group
		SelectDragObjectsVec mSelectedObjects; // Selected listeners in group

	protected:
		// Calls when selectable draggable object was pressed
		virtual void OnSelectableObjectCursorPressed(SelectableDragableObject* object, const Input::Cursor& cursor);

		friend class SelectableDragableObject;
	};

	// -------------------------------------
	// Selectable draggable object interface
	// -------------------------------------
	class SelectableDragableObject: public DragableObject
	{
	public:
		// Virtual destructor. Removes himself from selectable group, if need
		virtual ~SelectableDragableObject();

		// Returns is this selected
		bool IsSelected() const;

		// Sets is this selected
		void SetSelected(bool selected);

		// Selects this
		void Select();

		// Deselects this
		void Deselect();

		// Sets selection group
		void SetSelectionGroup(ISelectableDragableObjectsGroup* group);

		// Returns selection group
		ISelectableDragableObjectsGroup* GetSelectionGroup() const;

	protected:
		bool                             mIsSelected  = false;   // Is this selected
		ISelectableDragableObjectsGroup* mSelectGroup = nullptr; // Selection group

	protected:
		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor released outside this(only when cursor pressed this at previous time)
		virtual void OnCursorReleasedOutside(const Input::Cursor& cursor);

		// Calls when this was selected
		virtual void OnSelected();

		// Calls when this was unselected
		virtual void OnDeselected();

		friend class ISelectableDragableObjectsGroup;
		friend class SelectableDragableObjectsGroup;
	};
}