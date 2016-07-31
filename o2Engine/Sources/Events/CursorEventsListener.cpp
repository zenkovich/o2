#include "CursorEventsListener.h"

#include "Events/EventSystem.h"
#include "Render/IDrawable.h"
#include "Render/Render.h"

namespace o2
{
	CursorAreaEventsListener::CursorAreaEventsListener()
	{
		INITIALIZE_PROPERTY(CursorAreaEventsListener, interactable, SetInteractable, IsInteractable);
	}

	CursorAreaEventsListener::CursorAreaEventsListener(const CursorAreaEventsListener& other)
	{
		INITIALIZE_PROPERTY(CursorAreaEventsListener, interactable, SetInteractable, IsInteractable);
	}

	CursorAreaEventsListener::~CursorAreaEventsListener()
	{
		o2Events.UnregCursorAreaListener(this);
	}

	bool CursorAreaEventsListener::IsUnderPoint(const Vec2F& point)
	{
		return false;
	}

	bool CursorAreaEventsListener::IsScrollable() const
	{
		return false;
	}

	void CursorAreaEventsListener::SetInteractable(bool interactable)
	{
		if (mInteractable == interactable)
			return;

		mInteractable = interactable;

		if (mInteractable)
			OnBecomeInteractable();
		else
			OnBecomeNotInteractable();
	}

	bool CursorAreaEventsListener::IsInteractable() const
	{
		return mInteractable;
	}

	void CursorAreaEventsListener::OnDrawn()
	{
		if (!mInteractable)
			return;

		mScissorRect = o2Render.GetResScissorRect();
		o2Events.DrawnCursorAreaListener(this);
	}

	void CursorAreaEventsListener::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorPressed(cursor);
	}

	void CursorAreaEventsListener::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorReleased(cursor);
	}

	void CursorAreaEventsListener::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorPressBreak(cursor);
	}

	void CursorAreaEventsListener::OnCursorPressedOutside(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorPressedOutside(cursor);
	}

	void CursorAreaEventsListener::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorReleasedOutside(cursor);
	}

	void CursorAreaEventsListener::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorStillDown(cursor);
	}

	void CursorAreaEventsListener::OnCursorMoved(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorMoved(cursor);
	}

	void CursorAreaEventsListener::OnCursorEnter(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorEnter(cursor);
	}

	void CursorAreaEventsListener::OnCursorExit(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorExit(cursor);
	}

	void CursorAreaEventsListener::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorDblClicked(cursor);
	}

	void CursorAreaEventsListener::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorRightMousePressed(cursor);
	}

	void CursorAreaEventsListener::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorRightMouseReleased(cursor);
	}

	void CursorAreaEventsListener::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorRightMouseStayDown(cursor);
	}

	void CursorAreaEventsListener::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorMiddleMousePressed(cursor);
	}

	void CursorAreaEventsListener::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorMiddleMouseStayDown(cursor);
	}

	void CursorAreaEventsListener::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnCursorMiddleMouseReleased(cursor);
	}

	void CursorAreaEventsListener::OnScrolled(float scroll)
	{
		if (messageFallDownListener)
			messageFallDownListener->OnScrolled(scroll);
	}

	bool CursorAreaEventsListener::IsPressed() const
	{
		return mIsPressed;
	}

	CursorEventsListener::CursorEventsListener()
	{
		if (EventSystem::IsSingletonInitialzed())
			EventSystem::RegCursorListener(this);
	}

	CursorEventsListener::CursorEventsListener(const CursorAreaEventsListener& other)
	{
		if (EventSystem::IsSingletonInitialzed())
			EventSystem::RegCursorListener(this);
	}

	CursorEventsListener::~CursorEventsListener()
	{
		EventSystem::UnregCursorListener(this);
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

	void CursorEventsListener::OnCursorDblClicked(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMouseStillDown(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMouseStillDown(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
	{}

	void CursorEventsListener::OnScrolled(float scroll)
	{}

}