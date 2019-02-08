#include "stdafx.h"
#include "Application/Application.h"

#include <chrono>
#include <thread>
#include "Application/Input.h"
#include "Assets/Assets.h"
#include "Config/ProjectConfig.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/Scene.h"
#include "Scene/UI/UIManager.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/ConsoleLogStream.h"
#include "Utils/Debug/Log/FileLogStream.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/Debug/StackTrace.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/System/Time/Time.h"
#include "Utils/System/Time/Timer.h"
#include "Utils/Tasks/TaskManager.h"

namespace o2
{
	DECLARE_SINGLETON(Application);

	Application::Application() :
		mLog(nullptr), mReady(false), mAssets(nullptr), mEventSystem(nullptr), mFileSystem(nullptr), mInput(nullptr),
		mProjectConfig(nullptr), mRender(nullptr), mScene(nullptr), mTaskManager(nullptr), mTime(nullptr), mTimer(nullptr),
		mUIManager(nullptr), mCursorInfiniteModeEnabled(false)
	{}

	Application::~Application()
	{
		DeinitializeSystems();
	}

	void Application::BasicInitialize()
	{
		DataNode::RegBasicConverters();

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

		mLog->Out("Initialized");
	}

	void Application::DeinitializeSystems()
	{
		delete mUIManager;
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

		float maxFPS = 60.0f;
		float maxFPSDeltaTime = 1.0f/maxFPS;

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