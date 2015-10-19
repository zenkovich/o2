#include "Button.h"

namespace o2
{

	bool Button::IsUnderPoint(const Vec2F& point)
	{
		return point.x >= mAbsolutePosition.x && point.x <= mAbsolutePosition.x + mSize.x &&
			point.y >= mAbsolutePosition.y && point.y <= mAbsolutePosition.y + mSize.y;
	}

	float Button::Depth()
	{
		if (mDrawingLayers.Count() > 0)
			return mDrawingLayers.Last()->drawable->GetDrawingDepth();

		return 0.0f;
	}

	void Button::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void Button::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition))
			onClick();
	}

	void Button::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void Button::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

}