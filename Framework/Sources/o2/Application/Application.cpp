#include "o2/stdafx.h"
#include "o2/Application/Application.h"

#include <chrono>
#include <thread>
#include "o2/Application/Input.h"
#include "o2/Assets/Assets.h"
#include "o2/Config/ProjectConfig.h"
#include "o2/Events/EventSystem.h"
#include "o2/Render/Render.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/ConsoleLogStream.h"
#include "o2/Utils/Debug/Log/FileLogStream.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/Debug/StackTrace.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2/Utils/Tasks/TaskManager.h"

namespace o2
{
	DECLARE_SINGLETON(Application);

	Application::Application()
	{}

	Application::~Application()
	{
		DeinitializeSystems();
	}

	void Application::BasicInitialize()
	{
		InitalizeSystems();
		InitializePlatform();

		mRender = mnew Render();

		o2Debug.InitializeFont();
		o2UI.TryLoadStyle();

		mReady = true;
	}

	void Application::InitalizeSystems()
	{
		srand((UInt)time(NULL));

		mTime = mnew Time();

		mLog = mnew LogStream("Application");
		o2Debug.GetLog()->BindStream(mLog);

		mProjectConfig = mnew ProjectConfig();

		mAssets = mnew Assets();

		mInput = mnew Input();
		mTaskManager = mnew TaskManager();

		mTimer = mnew Timer();
		mTimer->Reset();

		mEventSystem = mnew EventSystem();

		mUIManager = mnew UIManager();

		mScene = mnew Scene();

		mPhysics = mnew PhysicsWorld();

		mLog->Out("Initialized");
	}

	void Application::DeinitializeSystems()
	{
		delete mUIManager;
		delete mPhysics;
		delete mScene;
		delete mRender;
		delete mInput;
		delete mTime;
		delete mTimer;
		delete mProjectConfig;
		delete mAssets;
		delete mEventSystem;
		delete mTaskManager;
	}

	void Application::ProcessFrame()
	{
		if (!mReady)
			return;

		if (mCursorInfiniteModeEnabled)
			CheckCursorInfiniteMode();

		float maxFPSDeltaTime = 1.0f/(float)maxFPS;

		float realdDt = mTimer->GetDeltaTime();

		if (realdDt < maxFPSDeltaTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)((maxFPSDeltaTime - realdDt)*1000.0f)));
			realdDt = maxFPSDeltaTime;
		}

		float dt = Math::Clamp(realdDt, 0.001f, 0.05f);

		mInput->PreUpdate();

		mTime->Update(realdDt);
		o2Debug.Update(dt);
		mTaskManager->Update(dt);
		mEventSystem->Update(dt);

		mRender->Begin();

		OnDraw();
		OnUpdate(dt);

		mScene->Update(dt);

		mAccumulatedDT += dt;
		float fixedDT = 1.0f/(float)fixedFPS;
		while (mAccumulatedDT > fixedDT)
		{
			OnFixedUpdate(fixedDT);
			mPhysics->Update(fixedDT);
			mAccumulatedDT -= fixedDT;
		}

		mEventSystem->PostUpdate();

		OnDraw();
		mScene->Draw();
		mUIManager->Draw();
		o2Debug.Draw();

		mRender->End();

		mInput->Update(dt);
	}

	LogStream* Application::GetLog() const
	{
		return mInstance->mLog;
	}

	Input* Application::GetInput() const
	{
		return mInstance->mInput;
	}

	ProjectConfig* Application::GetProjectConfig() const
	{
		return mInstance->mProjectConfig;
	}

	Time* Application::GetTime() const
	{
		return mInstance->mTime;
	}

	void Application::OnMoved()
	{}

	void Application::OnResizing()
	{}

	void Application::OnClosing()
	{}

	void Application::OnStarted()
	{}

	void Application::OnDeactivated()
	{}

	void Application::OnActivated()
	{}

	void Application::OnUpdate(float dt)
	{}

	void Application::OnFixedUpdate(float dt)
	{}

	void Application::OnDraw()
	{}

	bool Application::IsReady()
	{
		return IsSingletonInitialzed() && Application::Instance().mReady;
	}

	void Application::SetCursorInfiniteMode(bool enabled)
	{
		mCursorInfiniteModeEnabled = enabled;
	}

	bool Application::IsCursorInfiniteModeOn() const
	{
		return mCursorInfiniteModeEnabled;
	}

	bool Application::IsEditor() const
	{
		return IS_EDITOR;
	}
	MemoryManager* MemoryManager::mInstance = new MemoryManager();
	template<> Debug* Singleton<Debug>::mInstance = mnew Debug();
	template<> FileSystem* Singleton<FileSystem>::mInstance = mnew FileSystem();
}
