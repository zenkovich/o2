#include "o2/stdafx.h"
#include "CursorEventsArea.h"

#include "o2/Application/Application.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{

    CursorEventsArea::CursorEventsArea():
        cursorType(CursorType::Arrow)
    {}

    bool CursorEventsArea::IsUnderPoint(const Vec2F& point)
    {
        return isUnderPoint(point);
    }

    bool CursorEventsArea::IsInputTransparent() const
    {
        return isInputTransparent;
    }

    void CursorEventsArea::OnCursorPressed(const Input::Cursor& cursor)
    {
        onCursorPressed(cursor);
    }

    void CursorEventsArea::OnCursorReleased(const Input::Cursor& cursor)
    {
        onCursorReleased(cursor);
        if (!IsUnderPoint(cursor.position))
            o2Application.SetCursor(CursorType::Arrow);
    }

    void CursorEventsArea::OnCursorPressBreak(const Input::Cursor& cursor)
    {
        onCursorReleased(cursor);
        if (!IsUnderPoint(cursor.position))
            o2Application.SetCursor(CursorType::Arrow);
    }

    void CursorEventsArea::OnCursorStillDown(const Input::Cursor& cursor)
    {
        if (cursor.delta != Vec2F())
            onMoved(cursor);
    }

    void CursorEventsArea::OnCursorEnter(const Input::Cursor& cursor)
    {
        onCursorEnter(cursor);
        if (!cursor.isPressed)
            o2Application.SetCursor(cursorType);
    }

    void CursorEventsArea::OnCursorExit(const Input::Cursor& cursor)
    {
        onCursorExit(cursor);
        if (!IsPressed() && !cursor.isPressed)
            o2Application.SetCursor(CursorType::Arrow);
    }

    void CursorEventsArea::OnCursorDblClicked(const Input::Cursor& cursor)
    {
        onDblClicked(cursor);
    }

    void CursorEventsArea::OnCursorReleasedOutside(const Input::Cursor& cursor)
    {
        onCursorPressedOutside(cursor);
    }

	void CursorEventsArea::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
        onRightMousePressed(cursor);
	}

	void CursorEventsArea::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
        onRightMouseReleased(cursor);
	}

}
