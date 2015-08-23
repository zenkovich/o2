#include "IApplication.h"

#include <time.h>
#include "Input.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"
#include "Config/ProjectConfig.h"

namespace o2
{
	DECLARE_SINGLETON(IApplication);

	IApplication::IApplication():
		mLog(nullptr)
	{
		InitalizeSystems();
	}

	IApplication::~IApplication()
	{
		DeinitializeSystems();
	}

	void IApplication::InitalizeSystems()
	{
		srand((UInt)time(NULL));

		//log
		mLog = mnew LogStream("Application");
 		Debug::GetLog()->BindStream(mLog);

		//project config
 		mProjectConfig = mnew ProjectConfig();
// 
// 		//assets
// 		mAssets = mnew Assets();

		//input message
		mInput = mnew Input();
// 
// 		//scheduler
// 		mScheduler = mnew Scheduler();
// 
		//timer
		mTimer = mnew Timer();
		mTimer->Reset();

		//timers
		mTime = mnew Time();
// 
// 		//ui
// 		mUIController = mnew UIController();

		mLog->Out("Initialized");
	}

	void IApplication::DeinitializeSystems()
	{
		mInput.Release();
		mTime.Release();
		mTimer.Release();
		mProjectConfig.Release();

		mLog->Out("Deinitialized");
	}

	void IApplication::ProcessFrame()
	{
		float dt = Math::Clamp(mTimer->GetElapsedTime(), 0.001f, 0.05f);

		mTime->Update(dt);

		//mScheduler->ProcessBeforeFrame(dt);

		OnUpdate(dt);
		//mUIController->Update(dt);

		//mRenderSystem->BeginRender();
		OnDraw();
		//mUIController->Draw();
		//mRenderSystem->EndRender();

		mInput->update(dt);

		//mScheduler->ProcessAfterFrame(dt);
	}

	Ptr<LogStream> IApplication::GetLog() const
	{
		return mLog;
	}

	Ptr<Input> IApplication::GetInput() const
	{
		return mInput;
	}

	Ptr<ProjectConfig> IApplication::GetProjectConfig() const
	{
		return mProjectConfig;
	}

	Ptr<Time> IApplication::GetTime() const
	{
		return mTime;
	}

	void IApplication::OnMoved()
	{
	}

	void IApplication::OnResizing()
	{
	}

	void IApplication::OnClosing()
	{
	}

	void IApplication::OnStarted()
	{
	}

	void IApplication::OnDeactivated()
	{
	}

	void IApplication::OnActivated()
	{
	}

	o2::Vec2I IApplication::GetContentSize() const
	{
		return Vec2I();
	}

	void IApplication::SetContentSize(const Vec2I& size)
	{
	}

	o2::String IApplication::GetWindowCaption() const
	{
		return "";
	}

	void IApplication::SetWindowCaption(const String& caption)
	{
	}

	o2::Vec2I IApplication::GetWindowPosition() const
	{
		return Vec2I();
	}

	void IApplication::SetWindowPosition(const Vec2I& position)
	{
	}

	o2::Vec2I IApplication::GetWindowSize() const
	{
		return GetContentSize();
	}

	void IApplication::SetWindowSize(const Vec2I& size)
	{
	}

	bool IApplication::IsResizible() const
	{
		return false;
	}

	void IApplication::SetResizible(bool resizible)
	{
	}

	bool IApplication::IsFullScreen() const
	{
		return true;
	}

	void IApplication::SetFullscreen()
	{
	}

	void IApplication::SetWindowed()
	{
	}

	void IApplication::OnDraw()
	{
	}

	void IApplication::OnUpdate(float dt)
	{
	}

	void IApplication::Launch()
	{
	}
}