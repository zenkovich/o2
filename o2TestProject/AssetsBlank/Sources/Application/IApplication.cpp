#include "IApplication.h"

#include <time.h>
#include "Application/Input.h"
#include "Assets/Assets.h"
#include "Config/ProjectConfig.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

namespace o2
{
	DECLARE_SINGLETON(IApplication);

	IApplication::IApplication():
		mLog(nullptr)
	{
		InitializeProperties();
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
 		Debug.GetLog()->BindStream(mLog);

		//project config
 		mProjectConfig = mnew ProjectConfigStuff();

		//assets
		mAssets = mnew Assets();

		//input message
		mInput = mnew InputStuff();
// 
// 		//scheduler
// 		mScheduler = mnew Scheduler();
// 
		//timer
		mTimer = mnew Timer();
		mTimer->Reset();

		//timers
		mTime = mnew TimeStuff();
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
		mAssets.Release();

		mLog->Out("Deinitialized");
	}

	void IApplication::ProcessFrame()
	{
		float realdDt = mTimer->GetDeltaTime();
		float dt = Math::Clamp(realdDt, 0.001f, 0.05f);

		mTime->Update(realdDt);

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
		return mInstance->mLog;
	}

	Ptr<InputStuff> IApplication::GetInput() const
	{
		return mInstance->mInput;
	}

	Ptr<ProjectConfigStuff> IApplication::GetProjectConfig() const
	{
		return mInstance->mProjectConfig;
	}

	Ptr<TimeStuff> IApplication::GetTime() const
	{
		return mInstance->mTime;
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

	Vec2I IApplication::GetContentSize() const
	{
		return Vec2I();
	}

	void IApplication::SetContentSize(const Vec2I& size)
	{
	}

	String IApplication::GetWindowCaption() const
	{
		return "";
	}

	void IApplication::SetWindowCaption(const String& caption)
	{
	}

	Vec2I IApplication::GetWindowPosition() const
	{
		return Vec2I();
	}

	void IApplication::SetWindowPosition(const Vec2I& position)
	{
	}

	Vec2I IApplication::GetWindowSize() const
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

	void IApplication::SetFullscreen(bool fullscreen /*= true*/)
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
	
	void IApplication::InitializeProperties()
	{
		INITIALIZE_PROPERTY(IApplication, Fullscreen, SetFullscreen, IsFullScreen);
		INITIALIZE_PROPERTY(IApplication, Resizible, SetResizible, IsResizible);
		INITIALIZE_PROPERTY(IApplication, WindowSize, SetWindowSize, GetWindowSize);
		INITIALIZE_PROPERTY(IApplication, WindowContentSize, SetContentSize, GetContentSize);
		INITIALIZE_PROPERTY(IApplication, WindowPosition, SetWindowPosition, GetWindowPosition);
		INITIALIZE_PROPERTY(IApplication, WindowCaption, SetWindowCaption, GetWindowCaption);
	}
}