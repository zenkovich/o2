#pragma once

#include "Events/CursorEventsListener.h"

namespace o2
{
	class DragEventsListener: public CursorEventsListener
	{
	public:
		DragEventsListener();
		virtual ~DragEventsListener();

		virtual void OnCursorPressed(const Input::Cursor& cursor);
		virtual void OnCursorReleased(const Input::Cursor& cursor);
		virtual void OnCursorMoved(const Input::Cursor& cursor);

		virtual void OnDragStart(const Input::Cursor& cursor);
		virtual void OnDragged(const Input::Cursor& cursor);
		virtual void OnDragEnd(const Input::Cursor& cursor, Ptr<DragEventsListener> underDragListener);
		virtual void OnDropped(Ptr<DragEventsListener> dropped);

		bool IsDragging() const;

	protected:
		bool  mIsDragging;
		float mDragDistanceThreshold;
		Vec2F mPressedCursorPos;
		int   mPressedCursorId;

		friend class EventSystem;
	};
}