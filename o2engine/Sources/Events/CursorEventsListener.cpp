#include "CursorEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	CursorEventsListener::CursorEventsListener():
		mIsPressed(false), mIsRightMousePressed(false), mIsMiddleMousePressed(0)
	{
		EventSystem::RegCursorListener(this);
	}

	CursorEventsListener::CursorEventsListener(const CursorEventsListener& other):
		mIsPressed(false), mIsRightMousePressed(false), mIsMiddleMousePressed(0)
	{
		EventSystem::RegCursorListener(this);
	}

	CursorEventsListener::~CursorEventsListener()
	{
		EventSystem::UnregCursorListener(this);
	}

	bool CursorEventsListener::IsUnderPoint(const Vec2F& point)
	{
		return false;
	}

	float CursorEventsListener::Depth()
	{
		return 0.0f;
	}

	void CursorEventsListener::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorStayDown(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorEnter(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorExit(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnScrolled(float scroll)
	{}

	bool CursorEventsListener::IsPressed() const
	{
		return mIsPressed;
	}
}