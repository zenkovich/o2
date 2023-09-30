#pragma once

#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/String.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"

#if defined PLATFORM_WINDOWS
#include "o2/Application/Windows/ApplicationBase.h"
#elif defined PLATFORM_ANDROID
#include "o2/Application/Android/ApplicationBase.h"
#include <jni.h>
#include <android/asset_manager.h>
#elif defined PLATFORM_MAC
#include "o2/Application/Mac/ApplicationBase.h"
#elif defined PLATFORM_IOS
#include "o2/Application/iOS/ApplicationBase.h"
#elif defined PLATFORM_LINUX
#include "o2/Application/Linux/ApplicationBase.h"
#endif

// Application access macros
#define o2Application o2::Application::Instance()

namespace o2
{
	class Assets;
	class EventSystem;
	class FileSystem;
	class Input;
	class LogStream;
	class PhysicsWorld;
	class ProjectConfig;
	class Render;
	class Scene;
	class TaskManager;
	class Time;
	class Timer;
	class UIManager;

#if IS_SCRIPTING_SUPPORTED
	class ScriptEngine;
#endif

	// -----------
	// Application
	// -----------
	class Application: public Singleton<Application>, public ApplicationBase
	{
	public:
		PROPERTIES(Application);
		PROPERTY(bool, fullscreen, SetFullscreen, IsFullScreen);               // Full screen/window changing property
		PROPERTY(bool, resizible, SetResizible, IsResizible);                  // Resizible window property
		PROPERTY(Vec2I, windowSize, SetWindowSize, GetWindowSize);             // Window frame size property
		PROPERTY(Vec2I, windowContentSize, SetContentSize, GetContentSize);    // Window content frame size property
		PROPERTY(Vec2I, windowPosition, SetWindowPosition, GetWindowPosition); // Window position on screen property
		PROPERTY(String, windowCaption, SetWindowCaption, GetWindowCaption);   // Window caption property

	public:
		Function<void()> onActivated;   // On Activated event callbacks
		Function<void()> onDeactivated; // On deactivated event callbacks
		Function<void()> onStarted;     // On started event callbacks
		Function<void()> onClosing;     // On closing event callbacks
		Function<void()> onResizing;    // On resized app window callbacks. Ignoring on mobiles/tables
		Function<void()> onMoving;      // On moving app window callbacks. Ignoring on mobiles/tables

	public:
		int maxFPS = 600;   // Maximum frames per second
		int fixedFPS = 60; // Fixed frames per second

	public:
		// Default constructor
		Application();

		// Destructor 
		virtual ~Application();
		
		// Platform-specific initializations
		void InitializePlatform();

		// Returns pointer to log object
		virtual LogStream* GetLog() const;

		// Returns pointer to input message object
		virtual Input* GetInput() const;

		// Returns pointer to project config
		virtual ProjectConfig* GetProjectConfig() const;

		// Returns pointer to time utilities object
		virtual Time* GetTime() const;

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
		
		// Returns graphics scale
		virtual float GetGraphicsScale() const;

		// Returns is application ready to use
		static bool IsReady();

#if defined PLATFORM_WINDOWS
		// Initializes engine application
		virtual void Initialize();

		// Launching application cycle
		virtual void Launch();

#elif defined PLATFORM_ANDROID
		// Launching application
		virtual void Initialize(JNIEnv* env, jobject activity, AAssetManager* assetManager, String dataPath,
								const Vec2I& resolution);

		// Launching application cycle
		virtual void Launch();

		// Updates frame
		void Update();
		
#elif defined PLATFORM_MAC
		// Initializes engine application
		virtual void Initialize();
		
		// Launching application cycle
		virtual void Launch();
		
		// Updates frame
		void Update();
		
#elif defined PLATFORM_IOS
        // Initializes engine and runs it
        virtual void Run(int argc, char * argv[]);
		
        // Updates frame
        void Update();
		
        // Launching application
        virtual void Launch();

#elif defined PLATFORM_LINUX
        // Initializes engine application
        virtual void Initialize();

        // Launching application cycle
        virtual void Launch();

#endif

	protected:
		bool mReady = false; // Is all systems is ready

		Assets*        mAssets = nullptr;        // Assets
		EventSystem*   mEventSystem = nullptr;   // Events processing system
		FileSystem*    mFileSystem = nullptr;    // File system
		Input*         mInput = nullptr;         // While application user input message
		LogStream*     mLog = nullptr;           // Log stream with id "app", using only for application messages
		PhysicsWorld*  mPhysics = nullptr;       // Physics
		ProjectConfig* mProjectConfig = nullptr; // Project config
		Render*        mRender = nullptr;        // Graphics render
		Scene*         mScene = nullptr;         // Scene
		TaskManager*   mTaskManager = nullptr;   // Tasks manager
		Time*          mTime = nullptr;          // Time utilities
		Timer*         mTimer = nullptr;         // Timer for detecting delta time for update
		UIManager*     mUIManager = nullptr;     // UI manager

#if IS_SCRIPTING_SUPPORTED
		ScriptEngine*  mScriptingEngine = nullptr; // Scripting engine
#endif

		bool  mCursorInfiniteModeEnabled = false; // Is cursor infinite mode enabled
		Vec2F mCursorCorrectionDelta;             // Cursor corrections delta - result of infinite cursors offset

		float mAccumulatedDT = 0.0f; // Accumulated delta time for fixed FPS update
		
		CursorAreaEventListenersLayer mMainListenersLayer; // Main listeners layer, required for processing default scaled camera
		
		float mGraphicsScale = 1.0f; // Application graphics scale. Used in mac for retina displays

	protected:
		// Basic initialization for all platforms
		virtual void BasicInitialize();

		// It is called when application frame resized
		virtual void OnResized(const Vec2I& size);

		// Updates scene
		virtual void UpdateScene(float dt);

		// Updates scene with fixed delta time
		virtual void FixedUpdateScene(float dt);

		// Before update physics
		virtual void PreUpdatePhysics();

		// Updates physics
		virtual void UpdatePhysics(float dt);

		// After update physics
		virtual void PostUpdatePhysics();

		// Draws scene
		virtual void DrawScene();

		// Updates event system
		virtual void UpdateEventSystem();

		// Post updates event system
		virtual void PostUpdateEventSystem();

		// Draws UI manager
		virtual void DrawUIManager();

		// Draws debug
		virtual void DrawDebug();

		// Updates debug
		virtual void UpdateDebug(float dt);

		// Calling on updating
		virtual void OnUpdate(float dt);

		// Calling on updating by fixed FPS
		virtual void OnFixedUpdate(float dt);
		
		// Setups default camera with scaled graphics
		virtual void SetupGraphicsScaledCamera();

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
		virtual void InitalizeSystems();

		// Deinitializing systems
		virtual void DeinitializeSystems();

		// Processing frame update, drawing and input messages
		virtual void ProcessFrame();

		// Checks that cursor is near border and moves to opposite border if needs
		void CheckCursorInfiniteMode();

		friend class WndProcFunc;
	};
}
