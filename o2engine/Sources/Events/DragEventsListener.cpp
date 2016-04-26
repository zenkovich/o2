#include "DragEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	DragEventsListener::DragEventsListener():
		mIsDragging(false), mDragDistanceThreshold(2.0f), CursorEventsListener()
	{}

	DragEventsListener::~DragEventsListener()
	{
		EventSystem::UnregDragListener(this);
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
			OnDragEnd(cursor);

			auto underCursorListener = o2Events.GetCursorListenerUnderCursor(cursor.mId);
			auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

			if (dragArea)
			{
				OnDropped(dragArea);
				dragArea->OnDropped(this);
			}
		}

		mIsDragging = false;
	}

	void DragEventsListener::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsDragging = false;
	}

	void DragEventsListener::OnCursorMoved(const Input::Cursor& cursor)
	{
		if (!mIsPressed)
			return;

		if (cursor.mId != mPressedCursorId)
			return;

		auto underCursorListener = o2Events.GetCursorListenerUnderCursor(cursor.mId);
		auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

		if (!mIsDragging)
		{
			float delta = (cursor.mPosition - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold)
			{
				mIsDragging = true;
				OnDragged(cursor, dragArea);
			}
		}
		else OnDragged(cursor, dragArea);
	}

	void DragEventsListener::OnDragStart(const Input::Cursor& cursor)
	{}

	void DragEventsListener::OnDragged(const Input::Cursor& cursor, DragDropArea* area)
	{}

	void DragEventsListener::OnDragEnd(const Input::Cursor& cursor)
	{}

	void DragEventsListener::OnDropped(DragDropArea* area)
	{}

	bool DragEventsListener::IsDragging() const
	{
		return mIsDragging;
	}

	void DragEventsListener::OnDrawn()
	{
		CursorEventsListener::OnDrawn();
		EventSystem::RegDragListener(this);
	}

	void DragDropArea::OnDropped(DragEventsListener* draggable)
	{}

	void DragDropArea::OnDraggedAbove(DragEventsListener* draggable)
	{}

}