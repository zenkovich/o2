#pragma once

#include "Application/Input.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	// -----------------------
	// Cursors events listener
	// -----------------------
	class CursorEventsListener
	{
	public:
		// Default constructor
		CursorEventsListener();

		// Copy-constructor
		CursorEventsListener(const CursorEventsListener& other);

		// Virtual destructor
		virtual ~CursorEventsListener();

		// Returns true if point is in this object
		virtual bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		virtual float Depth();

		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		virtual void OnCursorStayDown(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		virtual void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		virtual void OnCursorExit(const Input::Cursor& cursor);

		// Calls when right mouse button was pressed on this
		virtual void OnCursorRightMousePressed(const Input::Cursor& cursor);

		// Calls when right mouse button stay down on this
		virtual void OnCursorRightMouseStayDown(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		virtual void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when middle mouse button was pressed on this
		virtual void OnCursorMiddleMousePressed(const Input::Cursor& cursor);

		// Calls when middle mouse button stay down on this
		virtual void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor);

		// Calls when middle mouse button was released (only when middle mouse button pressed this at previous time)
		virtual void OnCursorMiddleMouseReleased(const Input::Cursor& cursor);

		// Calls when scrolling
		virtual void OnScrolled(float scroll);

		// Returns true if this was pressed by some cursor
		bool IsPressed() const;
		
	protected:
		bool mIsPressed;            // True when was pressed by cursor
		bool mIsRightMousePressed;  // True when was pressed by right mouse button
		bool mIsMiddleMousePressed; // True when was pressed by middle mouse button

		friend class EventSystem;
	};
}
