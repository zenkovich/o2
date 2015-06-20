#pragma once

#include "Utils/Delegates.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"

namespace o2
{
	class LogStream;
	class FileSystem;
	class Time;
	class Timer;
	class Input;

	class IApplication: public Singleton<IApplication>
	{
	protected:
		Ptr<Input>      mInput;         /**< While application user input message. */
		Ptr<LogStream>  mLog;           /**< Log stream with id "app", using only for application messages. */
		Ptr<FileSystem> mFileSystem;    /**< File system. */
		Ptr<Time>       mTime;          /**< Time utilities. */
		Ptr<Timer>      mTimer;         /**< Timer for detecting delta time for update. */

		//RenderSystem* mRenderSystem;  /**< Render system. */
		//Scheduler*    mScheduler;     /**< Scheduler. */
		//UIController* mUIController;  /**< User interface controller host. */
		//Assets*       mAssets;        /**< Assets. */

	public:
		Function<void()> onActivatedEvent;   /**< On Activated event callbacks. */
		Function<void()> onDeactivatedEvent; /**< On deactivated event callbacks. */
		Function<void()> onStartedEvent;     /**< On started event callbacks. */
		Function<void()> onClosingEvent;     /**< On closing event callbacks. */
		Function<void()> onResizingEvent;    /**< On resized app window callbacks. Ignoring on mobiles/tables. */
		Function<void()> onMovingEvent;      /**< On moving app window callbacks. Ignoring on mobiles/tables. */

		/** ctor. */
		IApplication();

		/** dtor. */
		virtual ~IApplication();

		/** Returns pointer to log object. */
		Ptr<LogStream> GetLog() const;

		/** Returns pointer to input message object. */
		Ptr<Input> GetInput();

		/** Returns pointer to render system object. */
		//Ptr<RenderSystem> GetRenderSystem() const;

		/** Returns pointer to project config object. */
		//Ptr<ProjectConfig> GetProjectConfig() const;

		/** Returns pointer to scheduler object. */
		//Ptr<Scheduler> GetScheduler() const;

		/** Returns pointer to time utils object. */
		Ptr<Time> GetTime() const;

		/** Returns pointer to ui controller object. */
		//Ptr<UIController> GetUIController() const;

		/** Returns pointer to assets object. */
		//Ptr<Assets> GetAssets() const;

		/** Launching application cycle. */
		virtual void Launch();

		/** Makes application windowed. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetWindowed();

		/** Makes application fullscreen. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetFullscreen();

		/** Return true, if application is fullscreen On mobiles/tables always true. */
		virtual bool IsFullScreen() const;

		/** Sets application window as resizible. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetResizible(bool resizible);

		/** Returns true, if application is resizible. On mobiles/tablets always returns false. */
		virtual bool IsResizible() const;

		/** Sets application window size. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetWindowSize(const Vec2I& size);

		/** Returns application window size. On mobiles/tablets returns content size. */
		virtual Vec2I GetWindowSize() const;

		/** Sets application window position. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetWindowPosition(const Vec2I& position);

		/** Returns application window position. On mobiles/tablets return zero vector. */
		virtual Vec2I GetWindowPosition() const;

		/** Sets application window caption. On mobiles/tablets has no effect, just ignoring. */
		virtual void SetWindowCaption(const String& caption);

		/** Returns application window caption. On mobiles/tablets returns empty string. */
		virtual String GetWindowCaption() const;

		/** Sets inside content size. */
		virtual void SetContentSize(const Vec2I& size);

		/** Returns inside content size. */
		virtual Vec2I GetContentSize() const;

	protected:
		/** Called on updating. */
		virtual void OnUpdate(float dt);

		/** Called on drawing. */
		virtual void OnDraw();

		/** Initializing all systems and log. Call it when creating applications*/
		void InitalizeSystems();

		/** Deinitializing systems. */
		void DeinitializeSystems();

		/** Processing frame update, drawing and input messages. */
		void ProcessFrame();

		/** Calls when application activated. */
		virtual void OnActivated();

		/** Calls when application deactivated. */
		virtual void OnDeactivated();

		/** Calls when application is starting. */
		virtual void OnStarted();

		/** Calls when application is closing. */
		virtual void OnClosing();

		/** Calls when application window resized. Ignoring on mobiles/tablets. */
		virtual void OnResizing();

		/** Calls when application window moved. Ignoring on mobiles/tablets. */
		virtual void OnMoved();
	};
}