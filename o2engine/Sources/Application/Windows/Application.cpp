#include "Application/Application.h"

#include "Application/Input.h"
#include "Assets/Assets.h"
#include "Config/ProjectConfig.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "UI/UIManager.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"
#include <time.h>

namespace o2
{
	DECLARE_SINGLETON(Application);

	class WndProcFunc
	{
	public:
		static LRESULT WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	Application::Application():
		mLog(nullptr)
	{
		InitializeProperties();
		InitalizeSystems();

		mWindowed = true;
		mWindowedSize = Vec2I(800, 600);
		mWindowedPos = Vec2I(0, 0);
		mWindowResizible = true;
		mActive = false;
		mLog->Out("Initializing window..");

		mWndStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		WNDCLASSEX wndClass;
		wndClass.cbSize         = sizeof(WNDCLASSEX);
		wndClass.style			= mWndStyle;
		wndClass.lpfnWndProc	= (WNDPROC)WndProcFunc::WndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= NULL;
		wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
		wndClass.lpszMenuName	= NULL;
		wndClass.lpszClassName	= "o2App";
		wndClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wndClass))
		{
			mLog->Error("Can't register class");
			return;
		}

		if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",
									 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
									 mWindowedPos.x, mWindowedPos.y, mWindowedSize.x, mWindowedSize.y,
									 NULL, NULL, NULL, NULL)))
		{

			mLog->Error("Can't create window (CreateWindowEx)");
			return;
		}

		mLog->Out("Window initialized!");

		mRender = mnew Render();
		o2Debug.InitializeFont();
		o2UI.UpdateRootSize();
	}

	Application::~Application()
	{
		mRender.Release();
		DeinitializeSystems();
	}
	
	void Application::InitalizeSystems()
	{
		srand((UInt)time(NULL));

		mLog = mnew LogStream("Application");
		o2Debug.GetLog()->BindStream(mLog);

		mProjectConfig = mnew ProjectConfig();

		mAssets = mnew Assets();

		mInput = mnew Input();
// 
// 		mScheduler = mnew Scheduler();
// 
		mTimer = mnew Timer();
		mTimer->Reset();

		mTime = mnew Time();

		mEventSystem = mnew EventSystem();

		mUIManager = mnew UIManager();

		mLog->Out("Initialized");
	}

	void Application::DeinitializeSystems()
	{
		mUIManager.Release();
		mInput.Release();
		mTime.Release();
		mTimer.Release();
		mProjectConfig.Release();
		mAssets.Release();
		mEventSystem.Release();

		mLog->Out("Deinitialized");
	}

	void Application::ProcessFrame()
	{
		float realdDt = mTimer->GetDeltaTime();
		float dt = Math::Clamp(realdDt, 0.001f, 0.05f);

		mTime->Update(realdDt);
		o2Debug.Update(dt);

		//mScheduler->ProcessBeforeFrame(dt);

		mEventSystem->Update(dt);
		OnUpdate(dt);
		mUIManager->Update(dt);

		mRender->Begin();
		OnDraw();
		o2Debug.Draw();
		mUIManager->Draw();
		mRender->End();

		mInput->Update(dt);

		//mScheduler->ProcessAfterFrame(dt);
	}

	Ptr<LogStream> Application::GetLog() const
	{
		return mInstance->mLog;
	}

	Ptr<Input> Application::GetInput() const
	{
		return mInstance->mInput;
	}

	Ptr<ProjectConfig> Application::GetProjectConfig() const
	{
		return mInstance->mProjectConfig;
	}

	Ptr<Time> Application::GetTime() const
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

	void Application::Launch()
	{
		ShowWindow(mHWnd, SW_SHOW);

		mLog->Out("Application launched!");

		OnStarted();
		onStartedEvent.Invoke();
		o2Events.OnApplicationStarted();

		MSG msg;
		memset(&msg, 0, sizeof(msg));

		//mTimer->Reset();

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				ProcessFrame();
			}
		}

		o2Events.OnApplicationClosing();
		OnClosing();
		onClosingEvent.Invoke();
	}

	LRESULT WndProcFunc::WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto app = Application::InstancePtr();

		POINT pt;
		RECT rt;
		int key = 0;
		Vec2I size, pos;
		GetCursorPos(&pt);
		ScreenToClient(wnd, &pt);
		Vec2F cursorPos = Vec2F((float)pt.x, (float)-pt.y);

		if (app->mRender)
			cursorPos -= Vec2F(app->mRender->mResolution.x*0.5f, -app->mRender->mResolution.y*0.5f);

		float wheelDelta;

		switch (uMsg)
		{
			case WM_LBUTTONDOWN:
			app->mInput->CursorPressed(cursorPos);
			break;

			case WM_LBUTTONUP:
			app->mInput->CursorReleased();
			break;

			case WM_RBUTTONDOWN:
			app->mInput->AltCursorPressed(cursorPos);
			break;

			case WM_RBUTTONUP:
			app->mInput->AltCursorReleased();
			break;

			case WM_MBUTTONDOWN:
			app->mInput->Alt2CursorPressed(cursorPos);
			break;

			case WM_MBUTTONUP:
			app->mInput->Alt2CursorReleased();
			break;

			case WM_KEYDOWN:
			key = (int)wParam;
			app->mInput->KeyPressed(key);
			break;

			case WM_KEYUP:
			app->mInput->KeyReleased((int)wParam);
			break;

			case WM_MOUSEMOVE:
			app->mInput->SetCursorPos(cursorPos);
			break;

			case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			app->mInput->SetMouseWheelDelta(wheelDelta);
			break;

			case WM_ACTIVATE:
			if ((HWND)lParam == app->mHWnd || true)
			{
				if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
				{
					app->mActive = true;
					app->OnActivated();
					app->onActivatedEvent.Invoke();
					o2Events.OnApplicationActivated();
				}
				else
				{
					app->mActive = false;
					app->OnDeactivated();
					app->onDeactivatedEvent.Invoke();
					o2Events.OnApplicationDeactivated();
				}
			}
			break;

			case WM_SIZE:
			GetWindowRect(app->mHWnd, &rt);
			size.x = rt.right - rt.left; size.y = rt.bottom - rt.top;

			if (size.x > 0 && size.y > 0 && size != app->mWindowedSize)
			{
				app->mWindowedSize = size;
				app->OnResizing();
				app->mRender->mResolution = size;
				app->onResizingEvent.Invoke();
				o2Events.OnApplicationSized();
			}
			//mApplication->ProcessFrame();

			break;

			case WM_MOVE:
			GetWindowRect(app->mHWnd, &rt);
			pos.x = rt.left; pos.y = rt.top;

			if (pos.x < 10000 && pos.y < 10000 && pos != app->mWindowedPos)
			{
				app->mWindowedPos = pos;
				app->OnMoved();
				app->onMovingEvent.Invoke();
			}
			break;

			case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		}
		return DefWindowProc(wnd, uMsg, wParam, lParam);
	}

	void Application::SetFullscreen(bool fullscreen /*= true*/)
	{
		if (fullscreen)
		{
			//mRenderSystem->FrameResized();
			mLog->Out("Setting full screen");
		}
		else
		{
			mLog->Out("Setting windowed..");

			mWindowed = true;

			RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
			AdjustWindowRect(&rt, mWndStyle, false);
			SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y,
						 mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);

			//mRenderSystem->FrameResized();
			mLog->Out("Complete");
		}
	}

	void Application::OnUpdate(float dt)
	{}

	void Application::OnDraw()
	{}

	bool Application::IsFullScreen() const
	{
		return !mWindowed;
	}

	void Application::SetResizible(bool resizible)
	{
		if (resizible == mWindowResizible)
			return;

		mWindowResizible = resizible;

		if (mWindowResizible)
			mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		else
			mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;

		mLog->Out("set resizible: %s ", (mWindowResizible ? "true":"false"));

		SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
	}

	bool Application::IsResizible() const
	{
		return mWindowResizible;
	}

	void Application::SetWindowSize(const Vec2I& size)
	{
		mWindowedSize = size;
		mLog->Out("setWindowSize: %ix%i", mWindowedSize.x, mWindowedSize.y);
		SetFullscreen(!mWindowed);
	}

	Vec2I Application::GetWindowSize() const
	{
		return mWindowedSize;
	}

	void Application::SetWindowPosition(const Vec2I& position)
	{
		mWindowedPos = position;
		mLog->Out("set Window Position: %i, %i", mWindowedPos.x, mWindowedPos.y);
		SetFullscreen(!mWindowed);
	}

	Vec2I Application::GetWindowPosition() const
	{
		return mWindowedPos;
	}

	void Application::SetWindowCaption(const String& caption)
	{
		mWndCaption = caption;
		SetWindowText(mHWnd, mWndCaption.Data());
	}

	String Application::GetWindowCaption() const
	{
		return mWndCaption;
	}

	void Application::SetContentSize(const Vec2I& size)
	{
		Vec2I clientRectSize = size;

		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		clientRect.right = clientRect.left + size.x;
		clientRect.bottom = clientRect.top + size.y;

		AdjustWindowRect(&clientRect, mWndStyle, false);

		mWindowedPos = Vec2I(clientRect.left, clientRect.top);
		mWindowedSize = Vec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

		mLog->Out("Set Content Size: %ix%i", size.x, size.y);

		SetFullscreen(!mWindowed);
	}

	Vec2I Application::GetContentSize() const
	{
		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		return Vec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	Vec2I Application::GetScreenResolution() const
	{
		int hor = GetSystemMetrics(SM_CXSCREEN), ver = GetSystemMetrics(SM_CYSCREEN);
		return Vec2I(hor, ver);
	}

	void Application::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Application, fullscreen, SetFullscreen, IsFullScreen);
		INITIALIZE_PROPERTY(Application, resizible, SetResizible, IsResizible);
		INITIALIZE_PROPERTY(Application, windowSize, SetWindowSize, GetWindowSize);
		INITIALIZE_PROPERTY(Application, windowContentSize, SetContentSize, GetContentSize);
		INITIALIZE_PROPERTY(Application, windowPosition, SetWindowPosition, GetWindowPosition);
		INITIALIZE_PROPERTY(Application, windowCaption, SetWindowCaption, GetWindowCaption);
	}
}