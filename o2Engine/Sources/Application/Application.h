#pragma once

#include "Utils/Delegates.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"
#include "Application/Windows/ApplicationBase.h"

// Application access macros
#define o2Application Application::Instance()

namespace o2
{
	class Assets;
	class EventSystem;
	class FileSystem;
	class Input;
	class LogStream;
	class ProjectConfig;
	class Render;
	class Scene;
	class TaskManager;
	class Time;
	class Timer;
	class UIManager;

	// -----------
	// Application
	// -----------
	class Application: public Singleton<Application>, public ApplicationBase
	{
	public:
		Function<void()> onActivatedEvent;   // On Activated event callbacks
		Function<void()> onDeactivatedEvent; // On deactivated event callbacks
		Function<void()> onStartedEvent;     // On started event callbacks
		Function<void()> onClosingEvent;     // On closing event callbacks
		Function<void()> onResizingEvent;    // On resized app window callbacks. Ignoring on mobiles/tables
		Function<void()> onMovingEvent;      // On moving app window callbacks. Ignoring on mobiles/tables

		Property<bool>   fullscreen;         // Full screen/window changing property
		Property<bool>   resizible;          // Resizible window property
		Property<Vec2I>  windowSize;         // Window frame size property
		Property<Vec2I>  windowContentSize;  // Window content frame size property
		Property<Vec2I>  windowPosition;     // Window position on screen property
		Property<String> windowCaption;      // Window caption property

		// Default constructor
		Application();

		// Destructor 
		virtual ~Application();

		// Returns pointer to log object
		virtual LogStream* GetLog() const;

		// Returns pointer to input message object
		virtual Input* GetInput() const;

		// Returns pointer to project config
		virtual ProjectConfig* GetProjectConfig() const;

		// Returns pointer to time utilities object
		virtual Time* GetTime() const;

		// Launching application cycle
		virtual void Launch();

		// Shutting down application
		virtual void Shutdown();

		// Makes application fullscreen. On mobiles/tablets has no effect, just ignoring
		virtual void SetFullscreen(bool fullscreen = true);

		// Return true, if application is fullscreen On mobiles/tables always true
		virtual bool IsFullScreen() const;

		// Maximize application frame. Available only on PC
		virtual void Maximize();

		// Returns is frame maximized. Available only on PC
		virtual bool IsMaximized() const;

		// Sets application window as resizible. On mobiles/tablets has no effect, just ignoring
		virtual void SetResizible(bool resizible);

		// Returns true, if application is resizible. On mobiles/tablets always returns false
		virtual bool IsResizible() const;

		// Sets application window size. On mobiles/tablets has no effect, just ignoring
		virtual void SetWindowSize(const Vec2I& size);

		// Returns application window size. On mobiles/tablets returns content size
		virtual Vec2I GetWindowSize() const;

		// Sets application window position. On mobiles/tablets has no effect, just ignoring
		virtual void SetWindowPosition(const Vec2I& position);

		// Returns application window position. On mobiles/tablets return zero vector
		virtual Vec2I GetWindowPosition() const;

		// Sets application window caption. On mobiles/tablets has no effect, just ignoring
		virtual void SetWindowCaption(const String& caption);

		// Returns application window caption. On mobiles/tablets returns empty string
		virtual String GetWindowCaption() const;

		// Sets inside content size
		virtual void SetContentSize(const Vec2I& size);

		// Returns inside content size
		virtual Vec2I GetContentSize() const;

		// Returns device screen resolution
		virtual Vec2I GetScreenResolution() const;

		// Sets cursor type
		virtual void SetCursor(CursorType type);

		// Sets cursor position
		virtual void SetCursorPosition(const Vec2F& position);

		// Sets cursor infinite moves mode
		virtual void SetCursorInfiniteMode(bool enabled);

		// Returns is cursor infinite mode enabled
		virtual bool IsCursorInfiniteModeOn() const;

		// Is application run in editor
		virtual bool IsEditor() const;

		// Returns application's path
		virtual String GetBinPath() const;

		// Returns is application ready to use
		static bool IsReady();

	protected:
		Assets*        mAssets;        // Assets
		EventSystem*   mEventSystem;   // Events processing system
		FileSystem*    mFileSystem;    // File system
		Input*         mInput;         // While application user input message
		LogStream*     mLog;           // Log stream with id "app", using only for application messages
		ProjectConfig* mProjectConfig; // Project config
		Render*        mRender;        // Graphics render
		Scene*         mScene;         // Scene
		TaskManager*   mTaskManager;   // Tasks manager
		Time*          mTime;          // Time utilities
		Timer*         mTimer;         // Timer for detecting delta time for update
		UIManager*     mUIManager;     // UI manager

		bool           mReady;         // Is all systems is ready

		bool           mCursorInfiniteModeEnabled; // Is cursor infinite mode enabled
		Vec2F          mCursorCorrectionDelta;     // Cursor corrections delta - result of infinite cursors offset

	protected:
		// Calling on updating
		virtual void OnUpdate(float dt);

		// Calling on drawing
		virtual void OnDraw();

		// Calling when application activated
		virtual void OnActivated();

		// Calling when application deactivated
		virtual void OnDeactivated();

		// Calling when application is starting
		virtual void OnStarted();

		// Calling when application is closing
		virtual void OnClosing();

		// Calling when application window resized. Ignoring on mobiles/tablets
		virtual void OnResizing();

		// Calling when application window moved. Ignoring on mobiles/tablets
		virtual void OnMoved();

		// Initializing all systems and log. Call it when creating applications
		void InitalizeSystems();

		// Deinitializing systems
		void DeinitializeSystems();

		// Processing frame update, drawing and input messages
		virtual void ProcessFrame();

		// Checks that cursor is near border and moves to opposite border if needs
		void CheckCursorInfiniteMode();

		// Initializes properties
		void InitializeProperties();

		friend class WndProcFunc;
	};
}