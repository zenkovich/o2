#include "application_base_interface.h"

#include <time.h>
#include "assets/assets.h"
#include "config/project_config.h"
#include "render_system/render_system.h"
#include "ui/ui_controller.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/log/console_log_stream.h"
#include "util/log/file_log_stream.h"
#include "util/scheduler.h"
#include "util/time_utils.h"
#include "util/timer.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(ApplicationBaseInterface);

ApplicationBaseInterface::ApplicationBaseInterface():
mLog(NULL), mRenderSystem(NULL), mFileSystem(NULL)
{
	InitalizeSystems();
}

ApplicationBaseInterface::~ApplicationBaseInterface()
{
	DeinitializeSystems();
}

void ApplicationBaseInterface::InitalizeSystems()
{
	srand(time(NULL));

	//log
	FileLogStream* fileLog = mnew FileLogStream(BASIC_LOG_LEVEL, GLOBAL_LOG_FILENAME);
	gLog = mnew ConsoleLogStream(BASIC_LOG_LEVEL);
	fileLog->BindStream(gLog);
	mLog = mnew FileLogStream("App", 2, "app_log.txt");
	gLog->BindStream(mLog);

	//file system
	mFileSystem = mnew FileSystem();

	//project config
	mProjectConfig = mnew ProjectConfig();

	//assets
	mAssets = mnew Assets();

	//input message
	mInputMessage = mnew InputMessage();

	//scheduler
	mScheduler = mnew Scheduler();

	//timer
	mTimer = mnew Timer();
	mTimer->Reset();

	//timers
	mTimeUtils = mnew TimeUtil();

	//ui
	mUIController = mnew UIController();

	mLog->Out("All Systems initialized");
}

void ApplicationBaseInterface::DeinitializeSystems()
{
	SafeRelease(mFileSystem);
	SafeRelease(mScheduler);
	SafeRelease(mTimeUtils);
	SafeRelease(mUIController);
	SafeRelease(mProjectConfig);
	SafeRelease(mAssets);

	mLog->Out("All systems deinitialized");

	//safe_release(gLog->getParentStream());
}

void ApplicationBaseInterface::ProcessFrame()
{
	float dt = Clamp(mTimer->GetElapsedTime(), 0.001f, 0.05f);

	mTimeUtils->Update(dt);

	mScheduler->ProcessBeforeFrame(dt);

	OnUpdate(dt);
	mUIController->Update(dt);

	mRenderSystem->BeginRender();
	OnDraw();
	mUIController->Draw();
	mRenderSystem->EndRender();

	mInputMessage->update(dt);

	mScheduler->ProcessAfterFrame(dt);
}

InputMessage* ApplicationBaseInterface::GetInputMessage()
{
	return mInputMessage;
}

RenderSystem* ApplicationBaseInterface::GetRenderSystem() const
{
	return mRenderSystem;
}

LogStream* ApplicationBaseInterface::GetLog() const
{
	return mLog;
}

ProjectConfig* ApplicationBaseInterface::GetProjectConfig() const
{
	return mProjectConfig;
}

FileSystem* ApplicationBaseInterface::GetFileSystem() const
{
	return mFileSystem;
}

Scheduler* ApplicationBaseInterface::GetScheduler() const
{
	return mScheduler;
}

TimeUtil* ApplicationBaseInterface::GetTimeUtils() const
{
	return mTimeUtils;
}

UIController* ApplicationBaseInterface::GetUIController() const
{
	return mUIController;
}

Assets* ApplicationBaseInterface::GetAssets() const
{
	return mAssets;
}

CLOSE_O2_NAMESPACE