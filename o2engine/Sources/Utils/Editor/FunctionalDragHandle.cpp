#include "FunctionalDragHandle.h"

#include "Application/Application.h"
#include "Utils/Debug.h"

namespace o2
{

	FunctionalDragHandle::FunctionalDragHandle():
		cursorType(CursorType::Arrow)
	{}

	bool FunctionalDragHandle::IsUnderPoint(const Vec2F& point)
	{
		return isUnderPoint(point);
	}

	float FunctionalDragHandle::Depth()
	{
		return getDepth();
	}

	void FunctionalDragHandle::OnCursorPressed(const Input::Cursor& cursor)
	{
		onCursorPressed();
	}

	void FunctionalDragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		onCursorReleased();
		if (!IsUnderPoint(cursor.mPosition))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void FunctionalDragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		onCursorReleased();
		if (!IsUnderPoint(cursor.mPosition))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void FunctionalDragHandle::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.mDelta != Vec2F())
			onMoved(cursor);
	}

	void FunctionalDragHandle::OnCursorEnter(const Input::Cursor& cursor)
	{
		onCursorEnter();
		if (!cursor.mPressed)
			o2Application.SetCursor(cursorType);
	}

	void FunctionalDragHandle::OnCursorExit(const Input::Cursor& cursor)
	{
		onCursorExit();
		if (!IsPressed() && !cursor.mPressed)
			o2Application.SetCursor(CursorType::Arrow);
	}

}