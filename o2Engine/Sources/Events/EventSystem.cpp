#include "EventSystem.h"

#include "Events/ApplicationEventsListener.h"
#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "Render/Render.h"
#include "UI/Widget.h"
#include "Utils/Debug.h"
#include "Utils/DragAndDrop.h"
#include "Utils/Time.h"

namespace o2
{
#undef DrawText

	DECLARE_SINGLETON(EventSystem);

	EventSystem::EventSystem():
		mRightButtonPressedListener(nullptr), mMiddleButtonPressedListener(nullptr)
	{}

	EventSystem::~EventSystem()
	{}

	float EventSystem::GetDoubleClickTime() const
	{
		return mDblClickTime;
	}

	void EventSystem::Update(float dt)
	{
		mAreaCursorListeners.Reverse();
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
			else
				ProcessKeyPressed(key);
		}

		for (const Input::Key& key : o2Input.GetDownKeys())
		{
			if (key.keyCode == -1)
				ProcessRBDown();
			else if (key.keyCode == -2)
				ProcessMBDown();
			else
				ProcessKeyDown(key);
		}

		for (const Input::Key& key : o2Input.GetReleasedKeys())
		{
			if (key.keyCode == -1)
				ProcessRBReleased();
			else if (key.keyCode == -2)
				ProcessMBReleased();
			else
				ProcessKeyReleased(key);
		}

		if (o2Input.IsKeyDown(VK_F1))
		{
			int line = 0;
			auto allUnderCursor = GetAllCursorListenersUnderCursor(0);
			for (auto listener : allUnderCursor)
			{
				String name = "unknown";

				if (auto widget = dynamic_cast<UIWidget*>(listener))
				{
					name = widget->name;
					o2Debug.DrawRect(widget->layout.GetAbsoluteRect(), Color4::Red());
				}

				o2Debug.DrawText(Vec2F(-o2Render.GetResolution().x*0.5f, o2Render.GetResolution().y*0.5f - (float)line), name);

				line += 20;
			}
		}

		mAreaCursorListeners.Clear();
		mDragListeners.Clear();
	}

	void EventSystem::OnApplicationStarted()
	{
		for (auto listener : mApplicationListeners)
			listener->OnApplicationStarted();
	}

	void EventSystem::OnApplicationClosing()
	{
		for (auto listener : mApplicationListeners)
			listener->OnApplicationClosing();
	}

	void EventSystem::OnApplicationActivated()
	{
		for (auto listener : mApplicationListeners)
			listener->OnApplicationActivated();
	}

	void EventSystem::OnApplicationDeactivated()
	{
		for (auto listener : mApplicationListeners)
			listener->OnApplicationDeactivated();
	}

	void EventSystem::OnApplicationSized()
	{
		for (auto listener : mApplicationListeners)
			listener->OnApplicationSized();
	}

	void EventSystem::ProcessCursorTracing(const Input::Cursor& cursor)
	{
		for (auto listener : mAreaCursorListeners)
		{
			if (!listener->IsUnderPoint(cursor.position) || !listener->mScissorRect.IsInside(cursor.position))
				continue;

			auto drag = dynamic_cast<DragableObject*>(listener);
			if (drag && drag->IsDragging())
				continue;

			mUnderCursorListeners.Add(cursor.id, listener);

			return;
		}
	}

	void EventSystem::ProcessCursorEnter()
	{
		for (auto underCursor : mUnderCursorListeners)
		{
			if (!(mLastUnderCursorListeners.ContainsKey(underCursor.Key()) &&
				  mLastUnderCursorListeners[underCursor.Key()] == underCursor.Value()))
			{
				underCursor.Value()->OnCursorEnter(*o2Input.GetCursor(underCursor.Key()));
			}
		}
	}

	void EventSystem::ProcessCursorExit()
	{
		for (auto lastUnderCursor : mLastUnderCursorListeners)
		{
			if (!(mUnderCursorListeners.ContainsKey(lastUnderCursor.Key()) &&
				  mUnderCursorListeners[lastUnderCursor.Key()] == lastUnderCursor.Value()))
			{
				lastUnderCursor.Value()->OnCursorExit(*o2Input.GetCursor(lastUnderCursor.Key()));
			}
		}
	}

	CursorAreaEventsListener* EventSystem::GetCursorListenerUnderCursor(CursorId cursorId) const
	{
		if (mUnderCursorListeners.ContainsKey(cursorId))
			return mUnderCursorListeners[cursorId];

		return nullptr;
	}

	EventSystem::CursorAreaEventsListenersVec EventSystem::GetAllCursorListenersUnderCursor(CursorId cursorId) const
	{
		CursorAreaEventsListenersVec res;
		Vec2F cursorPos = o2Input.GetCursorPos(cursorId);
		for (auto listener : mAreaCursorListeners)
		{
			if (!listener->IsUnderPoint(cursorPos) || !listener->mScissorRect.IsInside(cursorPos) || !listener->mInteractable)
				continue;

			res.Add(listener);
		}

		return res;
	}

	void EventSystem::BreakCursorEvent()
	{
		for (auto kv : mPressedListeners)
		{
			kv.Value()->OnCursorPressBreak(*o2Input.GetCursor(kv.Key()));
			kv.Value()->mIsPressed = false;
		}

		mPressedListeners.Clear();
	}

	void EventSystem::ProcessCursorPressed(const Input::Cursor& cursor)
	{
		for (auto listener : mCursorListeners)
			listener->OnCursorPressed(cursor);

		for (auto listener : mAreaCursorListeners)
			if (!listener->IsUnderPoint(cursor.position))
				listener->OnCursorPressedOutside(cursor);

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		auto listener = mUnderCursorListeners[cursor.id];

		float time = o2Time.GetApplicationTime();
		if (time - listener->mLastPressedTime < mDblClickTime)
			listener->OnCursorDblClicked(cursor);
		else
		{
			mPressedListeners.Add(cursor.id, listener);
			listener->OnCursorPressed(cursor);
			listener->mIsPressed = true;
			listener->mLastPressedTime = time;
		}
	}

	void EventSystem::ProcessCursorDown(const Input::Cursor& cursor)
	{
		for (auto listener : mCursorListeners)
			listener->OnCursorStillDown(cursor);

		if (mPressedListeners.ContainsKey(cursor.id))
			mPressedListeners[cursor.id]->OnCursorStillDown(cursor);

		if (cursor.delta.Length() > FLT_EPSILON)
		{
			if (mUnderCursorListeners.ContainsKey(cursor.id))
				mUnderCursorListeners[cursor.id]->OnCursorMoved(cursor);
		}
	}

	void EventSystem::ProcessCursorReleased(const Input::Cursor& cursor)
	{
		for (auto listener : mCursorListeners)
			listener->OnCursorReleased(cursor);

		for (auto listener : mAreaCursorListeners)
			if (!listener->IsUnderPoint(cursor.position))
				listener->OnCursorReleasedOutside(cursor);

		if (mPressedListeners.ContainsKey(cursor.id))
		{
			mPressedListeners[cursor.id]->mIsPressed = false;
			mPressedListeners[cursor.id]->OnCursorReleased(cursor);
			mPressedListeners.Remove(cursor.id);
		}
	}

	void EventSystem::ProcessRBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMousePressed(cursor);

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		auto listener = mUnderCursorListeners[cursor.id];

		mRightButtonPressedListener = listener;

		listener->OnCursorRightMousePressed(cursor);
		listener->mIsRightMousePressed = true;
	}

	void EventSystem::ProcessRBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMouseStillDown(cursor);

		if (mRightButtonPressedListener)
			mRightButtonPressedListener->OnCursorRightMouseStayDown(cursor);
	}

	void EventSystem::ProcessRBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMouseReleased(cursor);

		if (mRightButtonPressedListener)
		{
			mRightButtonPressedListener->OnCursorRightMouseReleased(cursor);
			mRightButtonPressedListener->mIsRightMousePressed = false;
		}
	}

	void EventSystem::ProcessMBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMousePressed(cursor);

		if (!mUnderCursorListeners.ContainsKey(cursor.id))
			return;

		auto listener = mUnderCursorListeners[cursor.id];

		mMiddleButtonPressedListener = listener;

		listener->OnCursorMiddleMousePressed(cursor);
		listener->mIsMiddleMousePressed = true;
	}

	void EventSystem::ProcessMBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMouseStillDown(cursor);

		if (mMiddleButtonPressedListener)
			mMiddleButtonPressedListener->OnCursorMiddleMouseStayDown(cursor);
	}

	void EventSystem::ProcessMBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMouseReleased(cursor);

		if (mMiddleButtonPressedListener)
		{
			mMiddleButtonPressedListener->OnCursorMiddleMouseReleased(cursor);
			mMiddleButtonPressedListener->mIsMiddleMousePressed = false;
		}
	}

	void EventSystem::ProcessScrolling()
	{
		float scroll = o2Input.GetMouseWheelDelta();
		if (!Math::Equals(scroll, 0.0f))
		{
			for (auto kv : mUnderCursorListeners)
				kv.Value()->OnScrolled(scroll);

			for (auto listener : mCursorListeners)
				listener->OnScrolled(scroll);
		}
	}

	void EventSystem::ProcessKeyPressed(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
			listener->OnKeyPressed(key);
	}

	void EventSystem::ProcessKeyDown(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
			listener->OnKeyStayDown(key);
	}

	void EventSystem::ProcessKeyReleased(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
			listener->OnKeyReleased(key);
	}

	void EventSystem::DrawnCursorAreaListener(CursorAreaEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mAreaCursorListeners.Add(listener);
	}

	void EventSystem::UnregCursorAreaListener(CursorAreaEventsListener* listener)
	{
		mInstance->mAreaCursorListeners.Remove(listener);
		mInstance->mPressedListeners.RemoveAll([&](auto x) { return x.Value() == listener; });

		if (mInstance->mRightButtonPressedListener == listener)
			mInstance->mRightButtonPressedListener = nullptr;

		if (mInstance->mMiddleButtonPressedListener == listener)
			mInstance->mMiddleButtonPressedListener = nullptr;

		mInstance->mUnderCursorListeners.RemoveAll([&](auto x) { return x.Value() == listener; });
		mInstance->mLastUnderCursorListeners.RemoveAll([&](auto x) { return x.Value() == listener; });
	}

	void EventSystem::RegCursorListener(CursorEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mCursorListeners.Add(listener);
	}

	void EventSystem::UnregCursorListener(CursorEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mCursorListeners.Remove(listener);
	}

	void EventSystem::RegDragListener(DragableObject* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mDragListeners.Add(listener);
	}

	void EventSystem::UnregDragListener(DragableObject* listener)
	{
		mInstance->mDragListeners.Remove(listener);
	}

	void EventSystem::RegKeyboardListener(KeyboardEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mKeyboardListeners.Add(listener);
	}

	void EventSystem::UnregKeyboardListener(KeyboardEventsListener* listener)
	{
		mInstance->mKeyboardListeners.Remove(listener);
	}

	void EventSystem::RegApplicationListener(ApplicationEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		mInstance->mApplicationListeners.Add(listener);
	}

	void EventSystem::UnregApplicationListener(ApplicationEventsListener* listener)
	{
		mInstance->mApplicationListeners.Remove(listener);
	}
}