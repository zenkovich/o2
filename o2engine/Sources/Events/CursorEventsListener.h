#pragma once

#include "Application/Input.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	class CursorEventsListener
	{
	public:
		CursorEventsListener();
		virtual ~CursorEventsListener();

		virtual bool IsUnderPoint(const Vec2F& point);
		virtual float Depth();

		virtual void OnCursorPressed(const Input::Cursor& cursor);
		virtual void OnCursorReleased(const Input::Cursor& cursor);
		virtual void OnCursorStayDown(const Input::Cursor& cursor);
		virtual void OnCursorMoved(const Input::Cursor& cursor);
		virtual void OnCursorEnter(const Input::Cursor& cursor);
		virtual void OnCursorExit(const Input::Cursor& cursor);
		virtual void OnCursorRightMousePressed(const Input::Cursor& cursor);
		virtual void OnCursorRightMouseStayDown(const Input::Cursor& cursor);
		virtual void OnCursorRightMouseReleased(const Input::Cursor& cursor);
		virtual void OnCursorMiddleMousePressed(const Input::Cursor& cursor);
		virtual void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor);
		virtual void OnCursorMiddleMouseReleased(const Input::Cursor& cursor);

		virtual void OnScrolled(float scroll);

		bool IsPressed() const;
		
	protected:
		bool mIsPressed;
		bool mIsRightMousePressed;
		bool mIsMiddleMousePressed;

		friend class EventSystem;
	};
}
