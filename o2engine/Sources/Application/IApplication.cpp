#include "IApplication.h"

#include <time.h>
#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Debug.h"
#include "Utils/Timer.h"
#include "Utils/Time.h"
#include "Input.h"

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
		mLog = new LogStream("Application");
 		Debug::GetLog()->BindStream(mLog);
// 
// 		//project config
// 		mProjectConfig = mnew ProjectConfig();
// 
// 		//assets
// 		mAssets = mnew Assets();

		//input message
		mInput = new Input();
// 
// 		//scheduler
// 		mScheduler = mnew Scheduler();
// 
		//timer
		mTimer = new Timer();
		mTimer->Reset();

		//timers
		mTime = new Time();
// 
// 		//ui
// 		mUIController = mnew UIController();

		mLog->Out("Initialized");
	}

	void IApplication::DeinitializeSystems()
	{
		delete mInput;
		delete mTime;
		delete mTimer;

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

	LogStream* IApplication::GetLog() const
	{
		return mLog;
	}

	Input* IApplication::GetInput()
	{
		return mInput;
	}

	Time* IApplication::GetTime() const
	{
		return mTime;
	}

}