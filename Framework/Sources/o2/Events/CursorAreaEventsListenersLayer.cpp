#include "o2/stdafx.h"
#include "CursorAreaEventsListenersLayer.h"

#include "o2/Events/EventSystem.h"
#include "o2/Utils/Editor/DragAndDrop.h"

namespace o2
{
	void CursorAreaEventListenersLayer::OnBeginDraw()
	{
		viewPortBasis = o2Render.GetCamera().GetBasis();
		o2Events.SetCursorAreaEventsListenersLayer(this);
	}

	void CursorAreaEventListenersLayer::OnEndDraw()
	{
		renderBasis = Camera().GetBasis();
		o2Events.SetCursorAreaEventsListenersLayer(nullptr);
	}

	void CursorAreaEventListenersLayer::OnDrawn(const Basis& transform)
	{
		drawnTransform = transform;
		mLocalToWorldTransform = viewPortBasis.Inverted()*drawnTransform;

		CursorAreaEventsListener::OnDrawn();
	}

	Vec2F CursorAreaEventListenersLayer::ToLocal(const Vec2F& point)
	{
		return point*mLocalToWorldTransform.Inverted();
	}

	Vec2F CursorAreaEventListenersLayer::FromLocal(const Vec2F& point)
	{
		return point*mLocalToWorldTransform;
	}

	void CursorAreaEventListenersLayer::Update()
	{
		cursorEventAreaListeners.Reverse();
		mDragListeners.Reverse();

		mLastUnderCursorListeners = mUnderCursorListeners;
		mUnderCursorListeners.Clear();

		for (const Input::Cursor& cursor : o2Input.GetCursors())
			ProcessCursorTracing(cursor);

		ProcessCursorExit();
		ProcessCursorEnter();
		ProcessScrolling();

		for (const Input::Cursor& cursor : o2Input.GetCursors())
		{
			if (cursor.pressedTime < FLT_EPSILON && cursor.isPressed)
				ProcessCursorPressed(cursor);
			else
				ProcessCursorDown(cursor);
		}

		for (const Input::Cursor& cursor : o2Input.GetReleasedCursors())
		{
			ProcessCursorReleased(cursor);
			mPressedListeners.Remove(cursor.id);
		}

		for (const Input::Key& key : o2Input.GetPressedKeys())
		{
			if (key.keyCode == -1)
				ProcessRBPressed();
			else if (key.keyCode == -2)
				ProcessMBPressed();
		}

		for (const Input::Key& key : o2Input.GetDownKeys())
		{
			if (key.keyCode == -1)
				ProcessRBDown();
			else if (key.keyCode == -2)
				ProcessMBDown();
		}

		for (const Input::Key& key : o2Input.GetReleasedKeys())
		{
			if (key.keyCode == -1)
				ProcessRBReleased();
			else if (key.keyCode == -2)
				ProcessMBReleased();
		}
	}

	void CursorAreaEventListenersLayer::PostUpdate()
	{
		cursorEventAreaListeners.Clear();
		mDragListeners.Clear();
	}

	void CursorAreaEventListenersLayer::BreakCursorEvent()
	{
		for (auto& kv : mPressedListeners)
		{
			for (auto listener : kv.second)
			{
				listener->OnCursorPressBreak(*o2Input.GetCursor(kv.first));
				listener->mIsPressed = false;
			}
		}

		mPressedListeners.Clear();
	}

	void CursorAreaEventListenersLayer::UnregCursorAreaListener(CursorAreaEventsListener* listener)
	{
		cursorEventAreaListeners.Remove(listener);
		mRightButtonPressedListeners.Remove(listener);
		mMiddleButtonPressedListeners.Remove(listener);

		for (auto& kv : mPressedListeners)
			kv.second.Remove(listener);

		for (auto& kv : mUnderCursorListeners)
			kv.second.Remove(listener);

		for (auto& kv : mLastUnderCursorListeners)
			kv.second.Remove(listener);
	}

	void CursorAreaEventListenersLayer::UnregDragListener(DragableObject* listener)
	{
		mDragListeners.Remove(listener);
	}

	Vector<CursorAreaEventsListener*> CursorAreaEventListenersLayer::GetAllCursorListenersUnderCursor(CursorId cursorId) const
	{
		return cursorEventAreaListeners;
	}

	void CursorAreaEventListenersLayer::ProcessCursorTracing(const Input::Cursor& cursor)
	{
		for (auto listener : cursorEventAreaListeners)
		{
			if (!listener->IsUnderPoint(cursor.position) || !listener->mScissorRect.IsInside(cursor.position))
				continue;

			auto drag = dynamic_cast<DragableObject*>(listener);
			if (drag && drag->IsDragging())
				continue;

			if (!mUnderCursorListeners.ContainsKey(cursor.id))
				mUnderCursorListeners.Add(cursor.id, {});

			mUnderCursorListeners[cursor.id].Add(listener);

			if (!listener->IsInputTransparent())
				return;
		}
	}

	void CursorAreaEventListenersLayer::ProcessCursorEnter()
	{
		for (auto underCursorListeners : mUnderCursorListeners)
		{
			bool lastListenersHasSameCursor = mLastUnderCursorListeners.ContainsKey(underCursorListeners.first);
			for (auto listener : underCursorListeners.second)
			{
				if (!lastListenersHasSameCursor || !mLastUnderCursorListeners[underCursorListeners.first].Contains(listener))
					listener->OnCursorEnter(*o2Input.GetCursor(underCursorListeners.first));
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessCursorExit()
	{
		for (auto lastUnderCursorListeners : mLastUnderCursorListeners)
		{
			bool listenersHasSameCursor = mUnderCursorListeners.ContainsKey(lastUnderCursorListeners.first);
			for (auto listener : lastUnderCursorListeners.second)
			{
				if (!listenersHasSameCursor || !mUnderCursorListeners[lastUnderCursorListeners.first].Contains(listener))
					listener->OnCursorExit(*o2Input.GetCursor(lastUnderCursorListeners.first));
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessCursorPressed(const Input::Cursor& cursor)
	{
		for (auto listener : cursorEventAreaListeners)
		{
			if (!listener->IsUnderPoint(cursor.position))
				listener->OnCursorPressedOutside(cursor);
		}

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		for (auto listener : mUnderCursorListeners[cursor.id])
		{
			float time = o2Time.GetApplicationTime();
			if (time - listener->mLastPressedTime < o2Events.GetDoubleClickTime())
				listener->OnCursorDblClicked(cursor);
			else
			{
				if (!mPressedListeners.ContainsKey(cursor.id))
					mPressedListeners.Add(cursor.id, {});

				mPressedListeners[cursor.id].Add(listener);
				listener->OnCursorPressed(cursor);
				listener->mIsPressed = true;
				listener->mLastPressedTime = time;
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessCursorDown(const Input::Cursor& cursor)
	{
		if (mPressedListeners.ContainsKey(cursor.id))
		{
			bool broken = false;
			for (auto it = mPressedListeners[cursor.id].begin(); it != mPressedListeners[cursor.id].end();)
			{
				if (!broken)
				{
					(*it)->OnCursorStillDown(cursor);

					if (!(*it)->IsInputTransparent())
						broken = true;

					++it;
				}
				else
				{
					(*it)->OnCursorPressBreak(*o2Input.GetCursor(cursor.id));
					(*it)->mIsPressed = false;

					it = mPressedListeners[cursor.id].Remove(it);
				}
			}
		}

		if (cursor.delta.Length() > FLT_EPSILON)
		{
			if (mUnderCursorListeners.ContainsKey(cursor.id))
			{
				for (auto listener : mUnderCursorListeners[cursor.id])
					listener->OnCursorMoved(cursor);
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessCursorReleased(const Input::Cursor& cursor)
	{
		for (auto listener : cursorEventAreaListeners)
		{
			if (!listener->IsUnderPoint(cursor.position))
				listener->OnCursorReleasedOutside(cursor);
		}

		if (mPressedListeners.ContainsKey(cursor.id))
		{
			for (auto listener : mPressedListeners[cursor.id])
			{
				listener->mIsPressed = false;
				listener->OnCursorReleased(cursor);
			}

			mPressedListeners.Remove(cursor.id);
		}
	}

	void CursorAreaEventListenersLayer::ProcessRBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		mRightButtonPressedListeners.Clear();

		auto listeners = mUnderCursorListeners[cursor.id];
		for (auto listener : listeners)
		{
			mRightButtonPressedListeners.Add(listener);

			listener->OnCursorRightMousePressed(cursor);
			listener->mIsRightMousePressed = true;

			if (!listener->IsInputTransparent())
				break;
		}
	}

	void CursorAreaEventListenersLayer::ProcessRBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		bool broken = false;
		for (auto it = mRightButtonPressedListeners.begin(); it != mRightButtonPressedListeners.end();)
		{
			if (!broken)
			{
				(*it)->OnCursorRightMouseStayDown(cursor);

				if (!(*it)->IsInputTransparent())
					broken = true;

				++it;
			}
			else
			{
				(*it)->OnCursorPressBreak(*o2Input.GetCursor(cursor.id));
				(*it)->mIsPressed = false;

				it = mRightButtonPressedListeners.Remove(it);
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessRBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mRightButtonPressedListeners)
		{
			listener->OnCursorRightMouseReleased(cursor);
			listener->mIsRightMousePressed = false;
		}
	}

	void CursorAreaEventListenersLayer::ProcessMBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		mMiddleButtonPressedListeners.Clear();

		auto listeners = mUnderCursorListeners[cursor.id];
		for (auto listener : listeners)
		{
			mMiddleButtonPressedListeners.Add(listener);

			listener->OnCursorMiddleMousePressed(cursor);
			listener->mIsMiddleMousePressed = true;

			if (!listener->IsInputTransparent())
				break;
		}
	}

	void CursorAreaEventListenersLayer::ProcessMBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		bool broken = false;
		for (auto it = mMiddleButtonPressedListeners.begin(); it != mMiddleButtonPressedListeners.end();)
		{
			if (!broken)
			{
				(*it)->OnCursorMiddleMouseStayDown(cursor);

				if (!(*it)->IsInputTransparent())
					broken = true;

				++it;
			}
			else
			{
				(*it)->OnCursorPressBreak(*o2Input.GetCursor(cursor.id));
				(*it)->mIsMiddleMousePressed = false;

				it = mMiddleButtonPressedListeners.Remove(it);
			}
		}
	}

	void CursorAreaEventListenersLayer::ProcessMBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mMiddleButtonPressedListeners)
		{
			listener->OnCursorRightMouseReleased(cursor);
			listener->mIsMiddleMousePressed = false;
		}
	}

	void CursorAreaEventListenersLayer::ProcessScrolling()
	{
		float scroll = o2Input.GetMouseWheelDelta();
		if (!Math::Equals(scroll, 0.0f))
		{
			for (auto& kv : mUnderCursorListeners)
			{
				for (auto listener : kv.second)
				{
					if (!listener->IsScrollable())
						continue;

					listener->OnScrolled(scroll);

					if (!listener->IsInputTransparent())
						break;
				}
			}
		}
	}
}