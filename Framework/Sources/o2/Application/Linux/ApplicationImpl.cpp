#include "o2/stdafx.h"

#ifdef PLATFORM_LINUX

#include "o2/Application/Application.h"
#include "o2/Events/EventSystem.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/FileSystem/FileSystem.h"

#include <GL/glx.h>

namespace o2
{

	void Application::Initialize()
	{
		BasicInitialize();
	}

	void Application::InitializePlatform()
	{
		mWindowed = true;
		mWindowedSize = Vec2I(800, 600);
		mWindowedPos = Vec2I(0, 0);
		mWindowResizible = true;
		mActive = false;

#if !defined(O2_DISABLE_PLATFORM)
		if (mNeedPlatformInitialization)
		{
			mLog->Out("Initializing window..");

            mDisplay = XOpenDisplay(NULL);
            if (!mDisplay)
            {
                mLog->Error("Can't open display");
                return;
            }

            int screen = DefaultScreen(mDisplay);
            Window rootWindow = RootWindow(mDisplay, screen);

            GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER};
            mVisualInfo = glXChooseVisual(mDisplay, 0, att);

            Colormap cmap = XCreateColormap(mDisplay, rootWindow, mVisualInfo->visual, AllocNone);

            XSetWindowAttributes winAttrs;
            winAttrs.colormap = cmap;
            winAttrs.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
                                  ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask |
                                  Button1MotionMask | VisibilityChangeMask | FocusChangeMask;

            mWindow = XCreateWindow(mDisplay, rootWindow, mWindowedPos.x, mWindowedPos.y,
                mWindowedSize.x, mWindowedSize.y, 0, mVisualInfo->depth, InputOutput, mVisualInfo->visual, CWColormap | CWEventMask, &winAttrs);

            XStoreName(mDisplay, mWindow, "o2 application");
            XMapWindow(mDisplay, mWindow);

			mLog->Out("Window initialized!");
        }
#endif
	}

	void Application::Shutdown()
	{
		DestroyWindow(mHWnd);
	}

	void Application::SetFullscreen(bool fullscreen /*= true*/)
	{
//		if (fullscreen)
//		{
//			//mRenderSystem->FrameResized();
//			mLog->Out("Setting full screen");
//		}
//		else
//		{
//			mLog->Out("Setting windowed..");
//
//			mWindowed = true;
//
//			RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
//			AdjustWindowRect(&rt, mWndStyle, false);
//			SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y,
//						 mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);
//
//			//mRenderSystem->FrameResized();
//			mLog->Out("Complete");
//		}
	}

	void Application::CheckCursorInfiniteMode()
	{
//		int threshold = 10;
//		POINT p, lp;
//		GetCursorPos(&p);
//		lp = p;
//
//		Vec2I resolution = GetScreenResolution();
//
//		if (p.x > resolution.x - threshold)
//			p.x = threshold;
//		else if (p.x < threshold)
//			p.x = resolution.x - threshold;
//
//		if (p.y > resolution.y - threshold)
//			p.y = threshold;
//		else if (p.y < threshold)
//			p.y = resolution.y - threshold;
//
//		SetCursorPos(p.x, p.y);
//
//		if (p.x != lp.x || p.y != lp.y)
//			mCursorCorrectionDelta = Vec2F((float)(lp.x - p.x), (float)(lp.y - p.y));
	}

	void Application::Launch()
	{
		mLog->Out("Application launched!");

		OnStarted();
		onStarted.Invoke();
		o2Events.OnApplicationStarted();

        //mTimer->Reset();
        Vec2I cursorPos;

#if !defined(O2_DISABLE_PLATFORM)
        Atom wmDeleteMessage = XInternAtom(mDisplay, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(mDisplay, mWindow, &wmDeleteMessage, 1);

        while (1)
        {
            bool breakCycle = false;
            while (XPending(mDisplay) > 0)
            {
                XEvent xev;
                XNextEvent( mDisplay, &xev );

                //mLog->Out("event type " + (String)xev.type);

                if(xev.type == ClientMessage && xev.xclient.data.l[0] == wmDeleteMessage)
                {
                    // Exit the main loop if the close button is clicked
                    breakCycle = true;
                }

                if(xev.type == KeyPress)
                {
                    int key = XLookupKeysym( &xev.xkey, 0 );
                    mInput->OnKeyPressed( key );
                }

                if (xev.type == KeyRelease)
                {
                    int key = XLookupKeysym( &xev.xkey, 0 );
                    mInput->OnKeyReleased( key );
                }

                if (xev.type == ButtonPress)
                {
                    int button = xev.xbutton.button;
                    if(button == Button1)
                        mInput->OnCursorPressed( cursorPos );
                    else if(button == Button2)
                        mInput->OnAlt2CursorPressed( cursorPos );
                    else if(button == Button3)
                        mInput->OnAltCursorPressed( cursorPos );
                    if (xev.xbutton.button == Button4)
                        mInput->OnMouseWheel(120);
                    else if (xev.xbutton.button == Button5)
                        mInput->OnMouseWheel(-120);
                }

                if (xev.type == ButtonRelease)
                {
                    int button = xev.xbutton.button;
                    if(button == Button1)
                        mInput->OnCursorReleased();
                    else if(button == Button2)
                        mInput->OnAlt2CursorReleased();
                    else if(button == Button3)
                        mInput->OnAltCursorReleased();
                }

                if (xev.type == MotionNotify)
                {
                    Vec2I pos( xev.xmotion.x, xev.xmotion.y );

                    cursorPos = o2::Vec2I(pos.x - mWindowedSize.x / 2, mWindowedSize.y / 2 - pos.y);
                    mInput->OnCursorMoved( cursorPos );
                }

                if(xev.type == ResizeRequest)
                {
                    OnResized( Vec2I( xev.xresizerequest.width, xev.xresizerequest.height ) );
                }
            }

            XWindowAttributes gwa;
            XGetWindowAttributes(mDisplay, mWindow, &gwa);
            Vec2I windowedSize(gwa.width, gwa.height);
            if (windowedSize != mWindowedSize)
                OnResized(windowedSize);

            ProcessFrame();

            if (breakCycle)
                break;
        }
#endif

		o2Events.OnApplicationClosing();
		OnClosing();
		onClosing.Invoke();
	}

	bool Application::IsFullScreen() const
	{
		return !mWindowed;
	}

	void Application::Maximize()
	{
		ShowWindow(mHWnd, SW_MAXIMIZE);
	}

	bool Application::IsMaximized() const
	{
		WINDOWPLACEMENT pl;
		GetWindowPlacement(mHWnd, &pl);
		return pl.showCmd == SW_MAXIMIZE;
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

		mLog->Out("Set resizible: " + ((mWindowResizible ? (String)"true" : (String)"false")));

		SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
	}

	bool Application::IsResizible() const
	{
		return mWindowResizible;
	}

	void Application::SetWindowSize(const Vec2I& size)
	{
		mWindowedSize = size;
		mLog->Out("Set sindow size: %ix%i", mWindowedSize.x, mWindowedSize.y);
		SetFullscreen(!mWindowed);
	}

	Vec2I Application::GetWindowSize() const
	{
		return mWindowedSize;
	}

	void Application::SetWindowPosition(const Vec2I& position)
	{
		mWindowedPos = position;
		mLog->Out("Set window position: %i, %i", mWindowedPos.x, mWindowedPos.y);
		SetFullscreen(!mWindowed);
	}

	Vec2I Application::GetWindowPosition() const
	{
		return mWindowedPos;
	}

	void Application::SetWindowCaption(const String& caption)
	{
		mWndCaption = caption;
#if !defined(O2_DISABLE_PLATFORM)
        XStoreName(mDisplay, mWindow, caption.Data());
#endif
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

		mRender->OnFrameResized();
		onResizing();
	}

	Vec2I Application::GetContentSize() const
	{
		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		return Vec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	Vec2I Application::GetScreenResolution() const
	{

#if !defined(O2_DISABLE_PLATFORM)
        int screen = DefaultScreen(mDisplay);
        return Vec2I(DisplayWidth(mDisplay, screen), DisplayHeight(mDisplay, screen));
#else
        return Vec2I(800, 600);
#endif
	}

	void Application::SetCursor(CursorType type)
	{
		LPSTR cursorsIds[] = { IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO,
			IDC_SIZEALL, IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT };

		mCurrentCursor = LoadCursor(NULL, cursorsIds[(int)type]);
		::SetCursor(mCurrentCursor);
		SetClassLongPtr(mHWnd, (int)GCLP_HCURSOR, (LONG_PTR)mCurrentCursor);
	}

	void Application::SetCursorPosition(const Vec2F& position)
	{
		SetCursorPos((int)position.x, (int)position.y);
	}

	String Application::GetBinPath() const
	{
		TCHAR szFileName[MAX_PATH];
		GetModuleFileName(NULL, szFileName, MAX_PATH);
		return o2FileSystem.CanonicalizePath(o2FileSystem.GetParentPath((String)szFileName));
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
			cursorPos -= Vec2F(Math::Round(app->mRender->mResolution.x*0.5f),
							   Math::Round(-app->mRender->mResolution.y*0.5f));

		float wheelDelta;

		if (app->IsReady())
		{
			switch (uMsg)
			{
			case WM_LBUTTONDOWN:
			case WM_TOUCH:
			case WM_POINTERDOWN:
			SetCapture(app->mHWnd);
			app->mInput->OnCursorPressed(cursorPos);
			break;

			case WM_LBUTTONUP:
			case WM_POINTERUP:
			app->mInput->OnCursorReleased();
			ReleaseCapture();
			break;

			case WM_RBUTTONDOWN:
			SetCapture(app->mHWnd);
			app->mInput->OnAltCursorPressed(cursorPos);
			break;

			case WM_RBUTTONUP:
			app->mInput->OnAltCursorReleased();
			ReleaseCapture();
			break;

			case WM_MBUTTONDOWN:
			SetCapture(app->mHWnd);
			app->mInput->OnAlt2CursorPressed(cursorPos);
			break;

			case WM_MBUTTONUP:
			app->mInput->OnAlt2CursorReleased();
			ReleaseCapture();
			break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			key = (int)wParam;
			app->mInput->OnKeyPressed(key);
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
			app->mInput->OnKeyReleased((int)wParam);
			break;

			case WM_MOUSEMOVE:
			app->mInput->OnCursorMoved(cursorPos, 0);
			app->mInput->GetCursor()->delta -= app->mCursorCorrectionDelta;
			app->mCursorCorrectionDelta = Vec2F();
			break;

			case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			app->mInput->OnMouseWheel(wheelDelta);
			break;

			case WM_ACTIVATEAPP:
			case WM_ENABLE:
			if (wParam == TRUE)
			{
				app->mActive = true;
				app->OnActivated();
				app->onActivated.Invoke();
				o2Events.OnApplicationActivated();
			}
			else
			{
				app->mActive = false;
				app->OnDeactivated();
				app->onDeactivated.Invoke();
				o2Events.OnApplicationDeactivated();
			}
			break;

			case WM_SIZE:
			GetWindowRect(app->mHWnd, &rt);
			size.x = rt.right - rt.left; size.y = rt.bottom - rt.top;

			if (size.x > 0 && size.y > 0 && size != app->mWindowedSize)
				app->OnResized(size);

			app->ProcessFrame();

			break;

			case WM_MOVE:
			GetWindowRect(app->mHWnd, &rt);
			pos.x = rt.left; pos.y = rt.top;

			if (pos.x < 10000 && pos.y < 10000 && pos != app->mWindowedPos)
			{
				app->mWindowedPos = pos;
				app->OnMoved();
				app->onMoving.Invoke();
			}
			break;

			case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
			}
		}

		return DefWindowProc(wnd, uMsg, wParam, lParam);
	}
}

#endif // PLATFORM_LINUX
