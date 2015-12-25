#pragma once

#include "public.h"
#include "util/callback.h"
#include "util/input/input_message.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class RenderSystem;
class LogStream;
class FileSystem;
class Scheduler;
class TimeUtil;
class Timer;
class UIController;
class ProjectConfig;
class Assets;

/* Basic application class. Not implementing frame data.
  * Containing input message and systems:
  * Project configs
  * Application log stream,
  * Render system,
  * File system,
  * Scheduler,
  * Time utils,
  * UI controller
  */
class ApplicationBaseInterface: public Singleton<ApplicationBaseInterface>
{
protected:
	ProjectConfig* mProjectConfig; /* Project config. */
	InputMessage*  mInputMessage;  /* While application user input message. */
	LogStream*     mLog;           /* Log stream with id "app", using only for application messages. */
	RenderSystem*  mRenderSystem;  /* Render system. */
	FileSystem*    mFileSystem;    /* File system. */
	Scheduler*     mScheduler;     /* Scheduler. */
	TimeUtil*      mTimeUtils;     /* Time utilities. */
	UIController*  mUIController;  /* User interface controller host. */
	Assets*        mAssets;        /* Assets. */
	Timer*         mTimer;         /* Timer for detecting delta time for update. */

public:
	CallbackChain onActivatedEvent;   /* On Activated event callbacks. */
	CallbackChain onDeactivatedEvent; /* On deactivated event callbacks. */
	CallbackChain onStartedEvent;     /* On started event callbacks. */
	CallbackChain onClosingEvent;     /* On closing event callbacks. */
	CallbackChain onResizingEvent;    /* On resized app window callbacks. Ignoring on mobiles/tables. */
	CallbackChain onMovingEvent;      /* On moving app window callbacks. Ignoring on mobiles/tables. */

	/* ctor. */
	ApplicationBaseInterface();

	/* dtor. */
	virtual ~ApplicationBaseInterface();

	/* Returns pointer to input message object. */
	InputMessage* GetInputMessage();

	/* Returns pointer to render system object. */
	RenderSystem* GetRenderSystem() const;

	/* Returns pointer to log object. */
	LogStream* GetLog() const;

	/* Returns pointer to project config object. */
	ProjectConfig* GetProjectConfig() const;

	/* Returns pointer to file system object. */
	FileSystem* GetFileSystem() const;

	/* Returns pointer to scheduler object. */
	Scheduler* GetScheduler() const;

	/* Returns pointer to time utils object. */
	TimeUtil* GetTimeUtils() const;

	/* Returns pointer to ui controller object. */
	UIController* GetUIController() const;

	/* Returns pointer to assets object. */
	Assets* GetAssets() const;

	/* Launching application cycle. */
	virtual void Launch() {}

	/* Called on updating. */
	virtual void OnUpdate(float dt) {}

	/* Called on drawing. */
	virtual void OnDraw() {}

	/* Makes application windowed. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetWindowed() {}

	/* Makes application fullscreen. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetFullscreen() {}

	/* Return true, if application is fullscreen On mobiles/tables always true. */
	virtual bool IsFullScreen() const { return true; }

	/* Sets application window as resizible. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetResizible(bool resizible) {}

	/* Returns true, if application is resizible. On mobiles/tablets always returns false. */
	virtual bool IsResizible() const { return false; }

	/* Sets application window size. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetWindowSize(const Vec2I& size) {}

	/* Returns application window size. On mobiles/tablets returns content size. */
	virtual Vec2I GetWindowSize() const { return GetContentSize(); }

	/* Sets application window position. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetWindowPosition(const Vec2I& position) {}

	/* Returns application window position. On mobiles/tablets return zero vector. */
	virtual Vec2I GetWindowPosition() const { return Vec2I(); }

	/* Sets application window caption. On mobiles/tablets has no effect, just ignoring. */
	virtual void SetWindowCaption(const String& caption) {}

	/* Returns application window caption. On mobiles/tablets returns empty string. */
	virtual String GetWindowCaption() const { return ""; }

	/* Sets inside content size. */
	virtual void SetContentSize(const Vec2I& size) {}

	/* Returns inside content size. */
	virtual Vec2I GetContentSize() const { return Vec2I(); }

protected:
	/* Initializing all systems and log. Call it when creating applications*/
	void InitalizeSystems();

	/* Deinitializing systems. */
	void DeinitializeSystems();

	/* Processing frame update, drawing and input messages. */
	void ProcessFrame();

	/* Calls when application activated. */
	virtual void OnActivated() {}

	/* Calls when application deactivated. */
	virtual void OnDeactivated() {}

	/* Calls when application is starting. */
	virtual void OnStarted() {}

	/* Calls when application is closing. */
	virtual void OnClosing() {}

	/* Calls when application window resized. Ignoring on mobiles/tablets. */
	virtual void OnResizing() {}

	/* Calls when application window moved. Ignoring on mobiles/tablets. */
	virtual void OnMoved() {}
};

inline Assets*        AppAssets()     { return ApplicationBaseInterface::InstancePtr()->GetAssets(); }
inline FileSystem*    AppFileSystem() { return ApplicationBaseInterface::InstancePtr()->GetFileSystem(); }
inline InputMessage*  AppInput()      { return ApplicationBaseInterface::InstancePtr()->GetInputMessage(); }
inline ProjectConfig* AppConfig()     { return ApplicationBaseInterface::InstancePtr()->GetProjectConfig(); }
inline RenderSystem*  AppRender()     { return ApplicationBaseInterface::InstancePtr()->GetRenderSystem(); }
inline Scheduler*     AppScheduler()  { return ApplicationBaseInterface::InstancePtr()->GetScheduler(); }
inline TimeUtil*      AppTimer()      { return ApplicationBaseInterface::InstancePtr()->GetTimeUtils(); }
inline UIController*  AppUI()         { return ApplicationBaseInterface::InstancePtr()->GetUIController(); }

CLOSE_O2_NAMESPACE