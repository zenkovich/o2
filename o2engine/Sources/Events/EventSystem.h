#pragma once

#include "Application/Input.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"

#define o2Events o2::EventSystem::Instance()

namespace o2
{
	class CursorEventsListener;
	class DragEventsListener;
	class KeyboardEventsListener;
	class ApplicationEventsListener;

	class EventSystem: public Singleton<EventSystem>
	{
	public:
		typedef Vector<Ptr<CursorEventsListener>>      CursEventsListenersVec;
		typedef Vector<Ptr<DragEventsListener>>        DragEventsListenersVec;
		typedef Vector<Ptr<KeyboardEventsListener>>    KeybEventsListenersVec;
		typedef Vector<Ptr<ApplicationEventsListener>> AppEventsListenersVec;

	public:
		Ptr<DragEventsListener> GetListenerUnderCursor(CursorId cursorId) const;

	protected:
		EventSystem();
		~EventSystem();

		void Update(float dt);

		void OnApplicationStarted();
		void OnApplicationClosing();
		void OnApplicationActivated();
		void OnApplicationDeactivated();
		void OnApplicationSized();

		void ProcessCursorTracing(const Input::Cursor& cursor);

		void ProcessCursorEnter();
		void ProcessCursorExit();

		void ProcessCursorPressed(const Input::Cursor& cursor);
		void ProcessCursorDown(const Input::Cursor& cursor);
		void ProcessCursorReleased(const Input::Cursor& cursor);

		void ProcessRBPressed();
		void ProcessRBDown();
		void ProcessRBReleased();

		void ProcessMBPressed();
		void ProcessMBDown();
		void ProcessMBReleased();

		void ProcessScrolling();

		void ProcessKeyPressed(const Input::Key& key);
		void ProcessKeyDown(const Input::Key& key);
		void ProcessKeyReleased(const Input::Key& key);

	protected:
		CursEventsListenersVec                          mCursorListeners;
		Dictionary<CursorId, Ptr<CursorEventsListener>> mPressedListeners;
		Ptr<CursorEventsListener>                       mRightButtonPressedListener;
		Ptr<CursorEventsListener>                       mMiddleButtonPressedListener;
		Dictionary<CursorId, Ptr<CursorEventsListener>> mUnderCursorListeners;
		Dictionary<CursorId, Ptr<CursorEventsListener>> mLastUnderCursorListeners;
		DragEventsListenersVec                          mDragListeners;
		KeybEventsListenersVec                          mKeyboardListeners;
		AppEventsListenersVec                           mApplicationListeners;

		friend class CursorEventsListener;
		friend class KeyboardEventsListener;
		friend class DragEventsListener;
		friend class ApplicationEventsListener;
		friend class IApplication;
		friend class ITemplPtr<EventSystem>;
		friend class BaseApplication;
	};
}
