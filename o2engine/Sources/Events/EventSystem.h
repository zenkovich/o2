#pragma once

#include "Application/Input.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"

// Events system accessor macros
#define o2Events o2::EventSystem::Instance()

namespace o2
{
	class CursorEventsListener;
	class DragEventsListener;
	class KeyboardEventsListener;
	class ApplicationEventsListener;

	// -----------------------
	// Event processing system
	// -----------------------
	class EventSystem: public Singleton<EventSystem>
	{
	public:
		typedef Vector<Ptr<CursorEventsListener>>      CursEventsListenersVec;
		typedef Vector<Ptr<DragEventsListener>>        DragEventsListenersVec;
		typedef Vector<Ptr<KeyboardEventsListener>>    KeybEventsListenersVec;
		typedef Vector<Ptr<ApplicationEventsListener>> AppEventsListenersVec;

	public:
		// Returns drag event listener under cursor
		Ptr<DragEventsListener> GetListenerUnderCursor(CursorId cursorId) const;

		// Breaks cursor event. All pressed listeners will be unpressed with specific event OnPressBreak
		void BreakCursorEvent();

	protected:
		// Default constructor
		EventSystem();

		// Destructor
		~EventSystem();

		// Updates and processes events
		void Update(float dt);

		// Calls when application was started
		void OnApplicationStarted();

		// Calls when application closing
		void OnApplicationClosing();

		// Calls when application activated
		void OnApplicationActivated();

		// Calls when application deactivated
		void OnApplicationDeactivated();

		// Calls when application frame was sized
		void OnApplicationSized();

		// processes cursor tracing for cursor
		void ProcessCursorTracing(const Input::Cursor& cursor);

		// Processes cursor enter event
		void ProcessCursorEnter();

		// Processes cursor exit event
		void ProcessCursorExit();

		// Processes cursor pressed event
		void ProcessCursorPressed(const Input::Cursor& cursor);

		// Processes cursor down event
		void ProcessCursorDown(const Input::Cursor& cursor);

		// Processes cursor released event
		void ProcessCursorReleased(const Input::Cursor& cursor);

		// Processes mouse right button pressed event
		void ProcessRBPressed();

		// Processes mouse right button down event
		void ProcessRBDown();

		// Processes mouse right button released event
		void ProcessRBReleased();

		// Processes mouse middle button pressed event
		void ProcessMBPressed();

		// Processes mouse middle button down event
		void ProcessMBDown();

		// Processes mouse middle button released event
		void ProcessMBReleased();

		// Processes scrolling event
		void ProcessScrolling();

		// Processes key pressed event
		void ProcessKeyPressed(const Input::Key& key);

		// Processes key down event
		void ProcessKeyDown(const Input::Key& key);

		// Processes key released event
		void ProcessKeyReleased(const Input::Key& key);

		// Returns true if listener was clipped by scissors in render
		bool IsListenerClipped(float depth, const Vec2F& cursorPos) const;

	protected:
		CursEventsListenersVec                          mCursorListeners;             // All cursor listeners
		Dictionary<CursorId, Ptr<CursorEventsListener>> mPressedListeners;            // Pressed listeners for all pressed cursors
		Ptr<CursorEventsListener>                       mRightButtonPressedListener;  // Right mouse button pressed listener
		Ptr<CursorEventsListener>                       mMiddleButtonPressedListener; // Middle mouse button pressed listener
		Dictionary<CursorId, Ptr<CursorEventsListener>> mUnderCursorListeners;        // Under cursor listeners for each cursor
		Dictionary<CursorId, Ptr<CursorEventsListener>> mLastUnderCursorListeners;    // Under cursor listeners for each cursor on last frame
		DragEventsListenersVec                          mDragListeners;               // Drag events listeners
		KeybEventsListenersVec                          mKeyboardListeners;           // Keyboard events listeners
		AppEventsListenersVec                           mApplicationListeners;        // Application events listeners

	protected:
		// Registering cursor events listener
		static void RegCursorListener(CursorEventsListener* listener);

		// Unregistering cursor events listener
		static void UnregCursorListener(CursorEventsListener* listener);

		// Registering drag events listener
		static void RegDragListener(DragEventsListener* listener);

		// Unregistering drag events listener
		static void UnregDragListener(DragEventsListener* listener);

		// Registering keyboard events listener
		static void RegKeyboardListener(KeyboardEventsListener* listener);

		// Unregistering keyboard events listener
		static void UnregKeyboardListener(KeyboardEventsListener* listener);

		// Registering application events listener
		static void RegApplicationListener(ApplicationEventsListener* listener);

		// Unregistering application events listener
		static void UnregApplicationListener(ApplicationEventsListener* listener);

		friend class CursorEventsListener;
		friend class KeyboardEventsListener;
		friend class DragEventsListener;
		friend class ApplicationEventsListener;
		friend class IApplication;
		friend class ITemplPtr<EventSystem>;
		friend class BaseApplication;
	};
}
