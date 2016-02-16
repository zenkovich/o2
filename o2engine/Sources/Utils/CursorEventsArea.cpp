#include "CursorEventsArea.h"

#include "Application/Application.h"
#include "Utils/Debug.h"

namespace o2
{

	CursorEventsArea::CursorEventsArea():
		cursorType(CursorType::Arrow)
	{}

	bool CursorEventsArea::IsUnderPoint(const Vec2F& point)
	{
		return isUnderPoint(point);
	}

	void CursorEventsArea::OnCursorPressed(const Input::Cursor& cursor)
	{
		onCursorPressed(cursor);
	}

	void CursorEventsArea::OnCursorReleased(const Input::Cursor& cursor)
	{
		onCursorReleased(cursor);
		if (!IsUnderPoint(cursor.mPosition))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void CursorEventsArea::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		onCursorReleased(cursor);
		if (!IsUnderPoint(cursor.mPosition))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void CursorEventsArea::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.mDelta != Vec2F())
			onMoved(cursor);
	}

	void CursorEventsArea::OnCursorEnter(const Input::Cursor& cursor)
	{
		onCursorEnter(cursor);
		if (!cursor.mPressed)
			o2Application.SetCursor(cursorType);
	}

	void CursorEventsArea::OnCursorExit(const Input::Cursor& cursor)
	{
		onCursorExit(cursor);
		if (!IsPressed() && !cursor.mPressed)
			o2Application.SetCursor(CursorType::Arrow);
	}

}