#pragma once

#include "Events/CursorEventsListener.h"

namespace o2
{
	// --------------------
	// Drag events listener
	// --------------------
	class DragEventsListener: public CursorEventsListener
	{
	public:
		// Default constructor
		DragEventsListener();

		// Destructor
		virtual ~DragEventsListener();

		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		virtual void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when started dragging
		virtual void OnDragStart(const Input::Cursor& cursor);

		// Calls when dragged
		virtual void OnDragged(const Input::Cursor& cursor);

		// Calls when dragging completed
		virtual void OnDragEnd(const Input::Cursor& cursor, DragEventsListener* underDragListener);

		// Calls when some other DragEventListener was dragged to this
		virtual void OnDropped(DragEventsListener* dropped);

		// Returns true if it's dragging
		bool IsDragging() const;

	protected:
		bool  mIsDragging;            // True when dragging
		float mDragDistanceThreshold; // Drag distance threshold: object starts dragging when cursor moves more tan this distance
		Vec2F mPressedCursorPos;      // Cursor pressed position
		int   mPressedCursorId;       // Id of pressed cursor

		friend class EventSystem;
	};
}