#pragma once

#include "Events/CursorEventsListener.h"

namespace o2
{
	// ----------------------
	// Functional drag handle
	// ----------------------
	class FunctionalDragHandle: public CursorEventsListener
	{
	public:
		Function<float()>                    getDepth;         // Function for getting depth
		Function<bool(const Vec2F&)>         isUnderPoint;     // Function for detecting collision (parameter - cursor position)
		Function<void(const Input::Cursor&)> onMoved;          // Event when handle was moved (parameter - cursor)
		Function<void()>                     onCursorPressed;  // Event when handle was pressed
		Function<void()>                     onCursorReleased; // Event when handle was released
		Function<void()>                     onCursorEnter;	   // Event when cursor was entered to handle
		Function<void()>                     onCursorExit;	   // Event when cursor was exited to handle

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

	protected:
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);
	};
}