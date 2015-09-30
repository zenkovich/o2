#include "DragEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	DragEventsListener::DragEventsListener():
		mIsDragging(false), mDragDistanceThreshold(2.0f), CursorEventsListener()
	{
		o2Events.mDragListeners.Add(this);
	}

	DragEventsListener::~DragEventsListener()
	{
		o2Events.mDragListeners.Remove(this);
	}

	void DragEventsListener::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressedCursorPos = cursor.mPosition;
		mPressedCursorId = cursor.mId;
	}

	void DragEventsListener::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			auto underCursorListener = o2Events.GetListenerUnderCursor(cursor.mId);
			if (underCursorListener)
			{
				underCursorListener->OnDropped(this);
				OnDragEnd(cursor, underCursorListener);
			}
		}

		mIsDragging = false;
	}

	void DragEventsListener::OnCursorMoved(const Input::Cursor& cursor)
	{
		if (!mIsPressed)
			return;

		if (cursor.mId != mPressedCursorId)
			return;

		if (!mIsDragging)
		{
			float delta = (cursor.mPosition - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold)
			{
				mIsDragging = true;
				OnDragged(cursor);
			}
		}
		else OnDragged(cursor);
	}

	void DragEventsListener::OnDragStart(const Input::Cursor& cursor)
	{}

	void DragEventsListener::OnDragged(const Input::Cursor& cursor)
	{}

	void DragEventsListener::OnDragEnd(const Input::Cursor& cursor, Ptr<DragEventsListener> underDragListener)
	{}

	void DragEventsListener::OnDropped(Ptr<DragEventsListener> dropped)
	{}

	bool DragEventsListener::IsDragging() const
	{
		return mIsDragging;
	}
}