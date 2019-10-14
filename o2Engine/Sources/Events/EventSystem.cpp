#include "stdafx.h"
#include "EventSystem.h"

#include "Events/ApplicationEventsListener.h"
#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "Events/ShortcutKeysListener.h"
#include "Render/Render.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Editor/DragAndDrop.h"
#include "Utils/System/Time/Time.h"

namespace o2
{
#undef DrawText

	DECLARE_SINGLETON(EventSystem);

	EventSystem::EventSystem()
	{
		mShortcutEventsManager = mnew ShortcutKeysListenersManager();
	}

	EventSystem::~EventSystem()
	{
		delete mShortcutEventsManager;
	}

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

				if (auto widget = dynamic_cast<Widget*>(listener))
				{
					String path;
					auto parent = widget;
					while (parent)
					{
						path = parent->GetName() + "/" + path;
						parent = parent->GetParentWidget();
					}

					name = path + " : " + widget->GetType().GetName();
					o2Debug.DrawRect(widget->layout->GetWorldRect(), Color4::Red());
				}

				o2Debug.DrawText(Vec2F(-o2Render.GetResolution().x*0.5f, o2Render.GetResolution().y*0.5f - (float)line), name);

				line += 20;
			}
		}
	}

	void EventSystem::PostUpdate()
	{
		mAreaCursorListeners.Clear();
		mDragListeners.Clear();
	}

	void EventSystem::OnApplicationStarted()
	{
		for (auto listener : mApplicationListeners)
		{
			if (listener->IsListeningEvents())
				listener->OnApplicationStarted();
		}
	}

	void EventSystem::OnApplicationClosing()
	{
		for (auto listener : mApplicationListeners)
		{
			if (listener->IsListeningEvents())
				listener->OnApplicationClosing();
		}
	}

	void EventSystem::OnApplicationActivated()
	{
		for (auto listener : mApplicationListeners)
		{
			if (listener->IsListeningEvents())
				listener->OnApplicationActivated();
		}
	}

	void EventSystem::OnApplicationDeactivated()
	{
		for (auto listener : mApplicationListeners)
		{
			if (listener->IsListeningEvents())
				listener->OnApplicationDeactivated();
		}
	}

	void EventSystem::OnApplicationSized()
	{
		for (auto listener : mApplicationListeners)
		{
			if (listener->IsListeningEvents())
				listener->OnApplicationSized();
		}
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

			if (!mUnderCursorListeners.ContainsKey(cursor.id))
				mUnderCursorListeners.Add(cursor.id, {});

			mUnderCursorListeners[cursor.id].Add(listener);

			if (!listener->IsInputTransparent())
				return;
		}
	}

	void EventSystem::ProcessCursorEnter()
	{
		for (auto underCursorListeners : mUnderCursorListeners)
		{
			bool lastListenersHasSameCursor = mLastUnderCursorListeners.ContainsKey(underCursorListeners.Key());
			for (auto listener : underCursorListeners.Value())
			{
				if (!lastListenersHasSameCursor || !mLastUnderCursorListeners[underCursorListeners.Key()].Contains(listener))
					listener->OnCursorEnter(*o2Input.GetCursor(underCursorListeners.Key()));
			}
		}
	}

	void EventSystem::ProcessCursorExit()
	{
		for (auto lastUnderCursorListeners : mLastUnderCursorListeners)
		{
			bool listenersHasSameCursor = mUnderCursorListeners.ContainsKey(lastUnderCursorListeners.Key());
			for (auto listener : lastUnderCursorListeners.Value())
			{
				if (!listenersHasSameCursor || !mUnderCursorListeners[lastUnderCursorListeners.Key()].Contains(listener))
					listener->OnCursorExit(*o2Input.GetCursor(lastUnderCursorListeners.Key()));
			}
		}
	}

	bool EventSystem::eventsListenersEnabledByDefault = true;

	CursorAreaEventsListener* EventSystem::GetCursorListenerUnderCursor(CursorId cursorId) const
	{
		if (mUnderCursorListeners.ContainsKey(cursorId))
			return mUnderCursorListeners[cursorId].First();

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
			for (auto listener : kv.Value())
			{
				listener->OnCursorPressBreak(*o2Input.GetCursor(kv.Key()));
				listener->mIsPressed = false;
			}
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

		for (auto listener : mUnderCursorListeners[cursor.id])
		{
			float time = o2Time.GetApplicationTime();
			if (time - listener->mLastPressedTime < mDblClickTime)
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

	void EventSystem::ProcessCursorDown(const Input::Cursor& cursor)
	{
		for (auto listener : mCursorListeners)
			listener->OnCursorStillDown(cursor);

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

	void EventSystem::ProcessCursorReleased(const Input::Cursor& cursor)
	{
		for (auto listener : mCursorListeners)
			listener->OnCursorReleased(cursor);

		for (auto listener : mAreaCursorListeners)
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

	void EventSystem::ProcessRBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMousePressed(cursor);

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

	void EventSystem::ProcessRBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMouseStillDown(cursor);

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

	void EventSystem::ProcessRBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorRightMouseReleased(cursor);

		for (auto listener : mRightButtonPressedListeners)
		{
			listener->OnCursorRightMouseReleased(cursor);
			listener->mIsRightMousePressed = false;
		}
	}

	void EventSystem::ProcessMBPressed()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMousePressed(cursor);

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

	void EventSystem::ProcessMBDown()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMouseStillDown(cursor);

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

	void EventSystem::ProcessMBReleased()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (auto listener : mCursorListeners)
			listener->OnCursorMiddleMouseReleased(cursor);

		for (auto listener : mMiddleButtonPressedListeners)
		{
			listener->OnCursorRightMouseReleased(cursor);
			listener->mIsMiddleMousePressed = false;
		}
	}

	void EventSystem::ProcessScrolling()
	{
		float scroll = o2Input.GetMouseWheelDelta();
		if (!Math::Equals(scroll, 0.0f))
		{
			for (auto kv : mUnderCursorListeners)
			{
				for (auto listener : kv.Value())
				{
					listener->OnScrolled(scroll);

					if (!listener->IsInputTransparent())
						break;
				}
			}

			for (auto listener : mCursorListeners)
				listener->OnScrolled(scroll);
		}
	}

	void EventSystem::ProcessKeyPressed(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
		{
			if (listener->IsListeningEvents())
				listener->OnKeyPressed(key);
		}
	}

	void EventSystem::ProcessKeyDown(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
		{
			if (listener->IsListeningEvents())
				listener->OnKeyStayDown(key);
		}
	}

	void EventSystem::ProcessKeyReleased(const Input::Key& key)
	{
		auto listeners = mKeyboardListeners;
		for (auto listener : listeners)
		{
			if (listener->IsListeningEvents())
				listener->OnKeyReleased(key);
		}
	}

	void EventSystem::DrawnCursorAreaListener(CursorAreaEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (!listener->IsListeningEvents())
			return;

		mInstance->mAreaCursorListeners.Add(listener);
	}

	void EventSystem::UnregCursorAreaListener(CursorAreaEventsListener* listener)
	{
		mInstance->mAreaCursorListeners.Remove(listener);
		mInstance->mRightButtonPressedListeners.Remove(listener);
		mInstance->mMiddleButtonPressedListeners.Remove(listener);

		for (auto kv : mInstance->mPressedListeners)
			kv.Value().Remove(listener);

		for (auto kv : mInstance->mUnderCursorListeners)
			kv.Value().Remove(listener);

		for (auto kv : mInstance->mLastUnderCursorListeners)
			kv.Value().Remove(listener);
	}

	void EventSystem::RegCursorListener(CursorEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance)
			mInstance->mCursorListeners.Add(listener);
	}

	void EventSystem::UnregCursorListener(CursorEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance)
			mInstance->mCursorListeners.Remove(listener);
	}

	void EventSystem::RegDragListener(DragableObject* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance)
			mInstance->mDragListeners.Add(listener);
	}

	void EventSystem::UnregDragListener(DragableObject* listener)
	{
		if (mInstance)
			mInstance->mDragListeners.Remove(listener);
	}

	void EventSystem::RegKeyboardListener(KeyboardEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance)
			mInstance->mKeyboardListeners.Add(listener);
	}

	void EventSystem::UnregKeyboardListener(KeyboardEventsListener* listener)
	{
		if (mInstance)
			mInstance->mKeyboardListeners.Remove(listener);
	}

	void EventSystem::RegApplicationListener(ApplicationEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance)
			mInstance->mApplicationListeners.Add(listener);
	}

	void EventSystem::UnregApplicationListener(ApplicationEventsListener* listener)
	{
		if (mInstance)
			mInstance->mApplicationListeners.Remove(listener);
	}
}
