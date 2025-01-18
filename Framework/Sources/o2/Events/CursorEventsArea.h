#pragma once

#include "o2/Events/CursorAreaEventsListener.h"

namespace o2
{
    // -----------------------------
    // Functional cursor events area 
    // -----------------------------
    class CursorEventsArea: public RefCounterable, public CursorAreaEventsListener
    {
    public:
        Function<bool(const Vec2F&)>         isUnderPoint;           // Function for detecting collision (parameter - cursor position)

        Function<void(const Input::Cursor&)> onMoved;                // Event when cursor was moved (parameter - cursor)
        Function<void(const Input::Cursor&)> onCursorPressed;        // Event when cursor was pressed
        Function<void(const Input::Cursor&)> onCursorPressedOutside; // Event when cursor was pressed outside
        Function<void(const Input::Cursor&)> onCursorReleased;       // Event when cursor was released
        Function<void(const Input::Cursor&)> onCursorEnter;          // Event when cursor was entered to handle
        Function<void(const Input::Cursor&)> onCursorExit;           // Event when cursor was exited to handle
        Function<void(const Input::Cursor&)> onDblClicked;           // Event when cursor double clicked on this

        Function<void(const Input::Cursor&)> onRightMousePressed;    // Event when right mouse button pressed
        Function<void(const Input::Cursor&)> onRightMouseReleased;   // Event when right mouse button released

    public:
        CursorType cursorType; // Cursor type when hovering and dragging

        bool isInputTransparent = false; // Is input transparent, clicks pass through

    public:
        // Default constructor
        CursorEventsArea();

        // Returns true if point is in this object
        bool IsUnderPoint(const Vec2F& point) override;

        // Returns true when input events can be handled by down listeners
        bool IsInputTransparent() const override;

    protected:
        // Called when cursor pressed on this
        void OnCursorPressed(const Input::Cursor& cursor) override;

        // Called when cursor released (only when cursor pressed this at previous time)
        void OnCursorReleased(const Input::Cursor& cursor) override;

        // Called when cursor pressing was broken (when scrolled scroll area or some other)
        void OnCursorPressBreak(const Input::Cursor& cursor) override;

        // Called when cursor stay down during frame
        void OnCursorStillDown(const Input::Cursor& cursor) override;

        // Called when cursor enters this object
        void OnCursorEnter(const Input::Cursor& cursor) override;

        // Called when cursor exits this object
        void OnCursorExit(const Input::Cursor& cursor) override;

        // Called when cursor double clicked
        void OnCursorDblClicked(const Input::Cursor& cursor) override;

        // Called when cursor released outside this(only when cursor pressed this at previous time)
        void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

        // Called when right mouse button was pressed on this
        void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

        // Called when right mouse button was released (only when right mouse button pressed this at previous time)
        void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

        REF_COUNTERABLE_IMPL(RefCounterable);
    };
}
