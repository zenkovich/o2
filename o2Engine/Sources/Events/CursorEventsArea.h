#pragma once

#include "Events/CursorEventsListener.h"

namespace o2
{
	// ----------------------
	// Functional drag handle
	// ----------------------
	class CursorEventsArea: public CursorAreaEventsListener
	{
	public:
		Function<bool(const Vec2F&)>         isUnderPoint;     // Function for detecting collision (parameter - cursor position)
		Function<void(const Input::Cursor&)> onMoved;          // Event when handle was moved (parameter - cursor)
		Function<void(const Input::Cursor&)> onCursorPressed;  // Event when handle was pressed
		Function<void(const Input::Cursor&)> onCursorReleased; // Event when handle was released
		Function<void(const Input::Cursor&)> onCursorEnter;	   // Event when cursor was entered to handle
		Function<void(const Input::Cursor&)> onCursorExit;	   // Event when cursor was exited to handle
		Function<void(const Input::Cursor&)> onDblClicked;     // Event when cursor double clicked on this

	public:
		CursorType                           cursorType;       // Cursor type when hovering and dragging

	public:
		// Default constructor
		CursorEventsArea();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

	protected:
		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor);
	};
}
