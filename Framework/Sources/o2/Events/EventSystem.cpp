#include "o2/stdafx.h"
#include "EventSystem.h"

#include "o2/Events/ApplicationEventsListener.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Events/ShortcutKeysListener.h"
#include "o2/Render/Render.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/System/Time/Time.h"

namespace o2
{
#undef DrawText

	DECLARE_SINGLETON(EventSystem);

	EventSystem::EventSystem()
	{
		PushEditorScopeOnStack scope;

		mShortcutEventsManager = mnew ShortcutKeysListenersManager();
		mCurrentCursorAreaEventsLayer = &mCursorAreaListenersBasicLayer;
		mCursorAreaListenersBasicLayer.mEnabled = true;
	}

	EventSystem::~EventSystem()
	{
		delete mShortcutEventsManager;
	}

	float EventSystem::GetDoubleClickTime() const
	{
		return mDblClickTime;
	}

	void EventSystem::Update()
	{
		for (auto layer : mCursorAreaEventsListenersLayers)
			layer->Update();

		for (const Input::Cursor& cursor : o2Input.GetCursors())
		{
			if (cursor.pressedTime < FLT_EPSILON && cursor.isPressed)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorPressed(cursor);
			}
			else
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorStillDown(cursor);
			}
		}

		for (const Input::Cursor& cursor : o2Input.GetReleasedCursors())
		{
			for (auto listener : mCursorListeners)
				listener->OnCursorReleased(cursor);
		}

		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		for (const Input::Key& key : o2Input.GetPressedKeys())
		{
			if (key.keyCode == -1)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorRightMousePressed(cursor);
			}
			else if (key.keyCode == -2)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorMiddleMousePressed(cursor);
			}
			else
				ProcessKeyPressed(key);
		}

		for (const Input::Key& key : o2Input.GetDownKeys())
		{
			if (key.keyCode == -1)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorRightMouseStillDown(cursor);
			}
			else if (key.keyCode == -2)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorMiddleMouseStillDown(cursor);
			}
			else
				ProcessKeyDown(key);
		}

		for (const Input::Key& key : o2Input.GetReleasedKeys())
		{
			if (key.keyCode == -1)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorRightMouseReleased(cursor);
			}
			else if (key.keyCode == -2)
			{
				for (auto listener : mCursorListeners)
					listener->OnCursorMiddleMouseReleased(cursor);
			}
			else
				ProcessKeyReleased(key);
		}
		
		float scroll = o2Input.GetMouseWheelDelta();
		if (!Math::Equals(scroll, 0.0f))
		{
			for (auto listener : mCursorListeners)
				listener->OnScrolled(scroll);
		}

		if (o2Input.IsKeyDown(VK_F1))
		{
			int line = 0;
			auto allUnderCursor = o2Input.IsKeyDown(VK_CONTROL) ? 
				GetAllCursorListenersUnderCursor(0) : 
				mCursorAreaListenersBasicLayer.mUnderCursorListeners[0];

			for (auto listener : allUnderCursor)
			{
				String name = typeid(listener).name();

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
		for (auto layer : mCursorAreaEventsListenersLayers)
			layer->PostUpdate();

		mCursorAreaEventsListenersLayers.Clear();
		mCursorAreaEventsListenersLayers.Add(&mCursorAreaListenersBasicLayer);
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

	bool EventSystem::eventsListenersEnabledByDefault = true;

	Vector<CursorAreaEventsListener*> EventSystem::GetAllCursorListenersUnderCursor(CursorId cursorId) const
	{
		Vector<CursorAreaEventsListener*> res;
		Vec2F cursorPos = o2Input.GetCursorPos(cursorId);
		for (auto listener : mCursorAreaListenersBasicLayer.cursorEventAreaListeners)
		{
			if (!listener->IsUnderPoint(cursorPos) || !listener->mScissorRect.IsInside(cursorPos) || !listener->mInteractable)
				continue;

			res.Add(listener);
		}

		return res;
	}

	void EventSystem::BreakCursorEvent()
	{
		for (auto layer : mCursorAreaEventsListenersLayers)
			layer->BreakCursorEvent();
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

	void EventSystem::SetCursorAreaEventsListenersLayer(CursorAreaEventListenersLayer* layer)
	{
		if (layer)
		{
			mInstance->mCurrentCursorAreaEventsLayer = layer;
			mInstance->mCursorAreaEventsListenersLayers.Add(layer);
		}
		else 
			mInstance->mCurrentCursorAreaEventsLayer = &mInstance->mCursorAreaListenersBasicLayer;
	}

	void EventSystem::DrawnCursorAreaListener(CursorAreaEventsListener* listener)
	{
		if (!IsSingletonInitialzed())
			return;

		if (!listener->IsListeningEvents())
			return;

		mInstance->mCurrentCursorAreaEventsLayer->cursorEventAreaListeners.Add(listener);
	}

	void EventSystem::UnregCursorAreaListener(CursorAreaEventsListener* listener)
	{
		for (auto layer : mInstance->mCursorAreaEventsListenersLayers)
			layer->UnregCursorAreaListener(listener);
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
			mInstance->mCurrentCursorAreaEventsLayer->mDragListeners.Add(listener);
	}

	void EventSystem::UnregDragListener(DragableObject* listener)
	{
		if (mInstance)
		{
			for (auto layer : mInstance->mCursorAreaEventsListenersLayers)
				layer->UnregDragListener(listener);
		}
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
