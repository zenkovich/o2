#include "CursorEventsListener.h"

#include "Events/EventSystem.h"
#include "Render/IDrawable.h"
#include "Render/Render.h"

namespace o2
{
	CursorEventsListener::CursorEventsListener():
		mIsPressed(false), mIsRightMousePressed(false), mIsMiddleMousePressed(0), mInteractable(true)
	{
		INITIALIZE_PROPERTY(CursorEventsListener, interactable, SetInteractable, IsInteractable);
	}

	CursorEventsListener::CursorEventsListener(const CursorEventsListener& other):
		mIsPressed(false), mIsRightMousePressed(false), mIsMiddleMousePressed(0), mInteractable(other.mInteractable)
	{
		INITIALIZE_PROPERTY(CursorEventsListener, interactable, SetInteractable, IsInteractable);
	}

	CursorEventsListener::~CursorEventsListener()
	{
		o2Events.UnregCursorListener(this);
	}

	bool CursorEventsListener::IsUnderPoint(const Vec2F& point)
	{
		return false;
	}

	bool CursorEventsListener::IsScrollable() const
	{
		return false;
	}

	void CursorEventsListener::SetInteractable(bool interactable)
	{
		if (mInteractable == interactable)
			return;

		mInteractable = interactable;

		if (mInteractable)
			OnBecomeInteractable();
		else
			OnBecomeNotInteractable();
	}

	bool CursorEventsListener::IsInteractable() const
	{
		return mInteractable;
	}

	void CursorEventsListener::OnDrawn()
	{
		if (!mInteractable)
			return;

		mScissorRect = o2Render.GetResScissorRect();
		o2Events.DrawnCursorListener(this);
	}

	void CursorEventsListener::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorStillDown(const Input::Cursor& cursor)
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