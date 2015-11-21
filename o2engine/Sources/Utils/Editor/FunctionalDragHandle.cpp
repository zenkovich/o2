#include "FunctionalDragHandle.h"

namespace o2
{
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
	}

	void FunctionalDragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		onCursorReleased();
	}

	void FunctionalDragHandle::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.mDelta != Vec2F())
			onMoved(cursor);
	}

	void FunctionalDragHandle::OnCursorEnter(const Input::Cursor& cursor)
	{
		onCursorEnter();
	}

	void FunctionalDragHandle::OnCursorExit(const Input::Cursor& cursor)
	{
		onCursorExit();
	}

}