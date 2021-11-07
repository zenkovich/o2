#pragma once

#include "o2/Application/Input.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"

// Events system accessor macros
#define o2Events o2::EventSystem::Instance()

namespace o2
{
	class ApplicationEventsListener;
	class CursorAreaEventsListener;
	class CursorEventsListener;
	class DragableObject;
	class KeyboardEventsListener;
	class ShortcutKeysListenersManager;

	// -----------------------
	// Event processing system
	// -----------------------
	class EventSystem: public Singleton<EventSystem>
	{
	public:
		static bool eventsListenersEnabledByDefault; // Then it is true, new events listeners will be enabled on initialization

	public:
		// Returns all cursor listeners under cursor arranged by depth
		Vector<CursorAreaEventsListener*> GetAllCursorListenersUnderCursor(CursorId cursorId) const;

		// Breaks cursor event. All pressed listeners will be unpressed with specific event OnPressBreak
		void BreakCursorEvent();

		// Updates and processes events
		void Update();

		// Post update events
		void PostUpdate();

	protected:
		// Default constructor
		EventSystem();

		// Destructor
		~EventSystem();

		// Returns time between clicks for double click reaction
		float GetDoubleClickTime() const;

		// It is called when application was started
		void OnApplicationStarted();

		// It is called when application closing
		void OnApplicationClosing();

		// It is called when application activated
		void OnApplicationActivated();

		// It is called when application deactivated
		void OnApplicationDeactivated();

		// It is called when application frame was sized
		void OnApplicationSized();

		// Processes key pressed event
		void ProcessKeyPressed(const Input::Key& key);

		// Processes key down event
		void ProcessKeyDown(const Input::Key& key);

		// Processes key released event
		void ProcessKeyReleased(const Input::Key& key);

	protected:
		float mDblClickTime = 0.3f; // Time between clicks for double click reaction

		Vector<CursorEventsListener*> mCursorListeners; // All cursor non area listeners

		Vector<CursorAreaEventListenersLayer*> mCursorAreaEventsListenersLayers; // Drawn cursor area events listeners layers
		
		CursorAreaEventListenersLayer          mCursorAreaListenersBasicLayer; // Basic cursor area events listeners layer, for main screen
		CursorAreaEventListenersLayer*         mCurrentCursorAreaEventsLayer;  // Current list of area listeners
		Vector<CursorAreaEventListenersLayer*> mLayersStack;                   // Input layers stack, in order they are pushed

		Vector<KeyboardEventsListener*>    mKeyboardListeners;    // Keyboard events listeners
		Vector<ApplicationEventsListener*> mApplicationListeners; // Application events listeners

		ShortcutKeysListenersManager* mShortcutEventsManager; // Shortcut events manager

	protected:
		// Push current cursor area events listeners layer in layers stack
		static void PushCursorAreaEventsListenersLayer(CursorAreaEventListenersLayer* layer);

		// Pops current cursor area events listeners layer
		static void PopCursorAreaEventsListenersLayer();

		// Unregisters layer
		static void RemoveCursorAreaEventsListenersLayer(CursorAreaEventListenersLayer* layer);

		// Registering cursor area events listener
		static void DrawnCursorAreaListener(CursorAreaEventsListener* listener);

		// Unregistering cursor area events listener
		static void UnregCursorAreaListener(CursorAreaEventsListener* listener);

		// Registering cursor events listener
		static void RegCursorListener(CursorEventsListener* listener);

		// Unregistering cursor events listener
		static void UnregCursorListener(CursorEventsListener* listener);

		// Registering drag events listener
		static void RegDragListener(DragableObject* listener);

		// Unregistering drag events listener
		static void UnregDragListener(DragableObject* listener);

		// Registering keyboard events listener
		static void RegKeyboardListener(KeyboardEventsListener* listener);

		// Unregistering keyboard events listener
		static void UnregKeyboardListener(KeyboardEventsListener* listener);

		// Registering application events listener
		static void RegApplicationListener(ApplicationEventsListener* listener);

		// Unregistering application events listener
		static void UnregApplicationListener(ApplicationEventsListener* listener);

		friend class Application;
		friend class ApplicationEventsListener;
		friend class CursorAreaEventListenersLayer;
		friend class CursorAreaEventsListener;
		friend class CursorEventsListener;
		friend class DragableObject;
		friend class KeyboardEventsListener;
		friend class WndProcFunc;
	};
}
