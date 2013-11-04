#include "engine_settings.h"
#ifdef PLATFORM_WIN

#include "application_win.h"
#include "other/device_info.h"
#include "render_system/render_system.h"
#include "util/log.h"
#include "util/math/math.h"
#include "util/timer.h"

OPEN_O2_NAMESPACE


cApplication::cApplication():
	cApplicationBaseInterface(), mHWnd(0), mWndStyle(0), mWindowed(true), mWindowedSize(800, 600), mWindowedPos(0, 0),
	mAutoAjustByScreen(false), mAutoAjustScale(1, 1), mWindowResizible(true), mActive(false), mTimer(NULL)
{
	initializeWindow();

	mTimer = new cTimer;
	mTimer->reset();
	mApplication = this;

	mRenderSystem = new grRenderSystem(this);

	cDeviceInfo::initializeSingleton();
	deviceInfo().initialize(this);
}

cApplication::~cApplication()
{	
	safe_release(mRenderSystem);

	cDeviceInfo::deinitializeSingleton();
}

void cApplication::initializeWindow()
{
	mLog->hout("Initializing window..");

	mWndStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	WNDCLASSEX wndClass;
	wndClass.cbSize         = sizeof(WNDCLASSEX);
	wndClass.style			= mWndStyle;
	wndClass.lpfnWndProc	= (WNDPROC)wndProc;
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
		mLog->out("ERROR: Can't regist class");
		return;
	}

	if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",             
						   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						   mWindowedPos.x, mWindowedPos.y, mWindowedSize.x, mWindowedSize.y,
						   NULL, NULL, NULL,  NULL))) 
	{
		
		mLog->out("ERROR: Can't create window (CreateWindowEx)");
		return;
	}

	mLog->hout("Window initialized!");
}

void cApplication::launch()
{
	ShowWindow(mHWnd, SW_SHOW);
	
	mLog->hout("Application launched!");

	processMessage(cApplacationMessage::ON_STARTED);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	mTimer->reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else
		{
			float dt = mTimer->getElapsedTime();

			onUpdate(dt);
			draw();
			mInputMessage.update(dt);

		}
	}

	processMessage(cApplacationMessage::ON_CLOSING);
}

LRESULT cApplication::wndProc( HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	POINT pt;
	RECT rt;
	int key = 0;
	vec2i size, pos;

	switch(uMsg)
	{	
	case WM_LBUTTONDOWN:
		mApplication->mInputMessage.keyPressed(VK_LBUTTON);
		break;

	case WM_LBUTTONUP:
		mApplication->mInputMessage.keyReleased(VK_LBUTTON);
		break;
	
	case WM_RBUTTONDOWN:
		mApplication->mInputMessage.keyPressed(VK_RBUTTON);
		break;

	case WM_RBUTTONUP:
		mApplication->mInputMessage.keyReleased(VK_RBUTTON);
		break;

	case WM_KEYDOWN:
		key = (int)wParam;
		mApplication->mInputMessage.keyPressed(key);
		break;

	case WM_KEYUP:
		mApplication->mInputMessage.keyReleased((int)wParam);
		break;

	case WM_MOUSEMOVE:
		GetCursorPos(&pt);
		ScreenToClient(wnd, &pt);

		mApplication->mInputMessage.setCursorPos(vec2f((float)pt.x, (float)pt.y));
		break;

	case WM_ACTIVATE:
		if ((HWND)lParam == mApplication->mHWnd || true)
		{
			//hlog("LOWORD(wParam) = %i %i %i", LOWORD(wParam), mApplication->mHWnd, lParam);

			if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
			{
				mApplication->mActive = true;
				mApplication->processMessage(cApplacationMessage::ON_ACTIVATED);
			}
			else
			{
				mApplication->mActive = false;
				mApplication->processMessage(cApplacationMessage::ON_DEACTIVATED);
			}
		}
		break;

	case WM_SIZE:
		GetWindowRect(mApplication->mHWnd, &rt);
		size.x = rt.right - rt.left; size.y = rt.bottom - rt.top;

		if (size.x > 0 && size.y > 0 && size != mApplication->mWindowedSize)
		{
			mApplication->mWindowedSize = size;
			mApplication->processMessage(cApplacationMessage::ON_SIZING);
		}
		break;

	case WM_MOVE:
		GetWindowRect(mApplication->mHWnd, &rt);
		pos.x = rt.left; pos.y = rt.top;

		if (pos.x < 10000 && pos.y < 10000 && pos != mApplication->mWindowedPos)
		{
			mApplication->mWindowedPos = pos;
			//hlog("WND POS %i %i", pos.x, pos.y);
			mApplication->processMessage(cApplacationMessage::ON_MOVING);
		}
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(wnd, uMsg, wParam, lParam);
}

void cApplication::setOption( cApplicationOption::type option, ... )
{
	va_list vlist;
	va_start(vlist, option);

	if (option == cApplicationOption::WINDOWED)
	{
		mWindowed = true;

		mLog->hout("cApplication::setOptions( WINDOWED )");

		resetWnd();
	}
	else if (option == cApplicationOption::FULLSCREEN)
	{
		mWindowed = false;

		mLog->hout("cApplication::setOptions( FULLSCREEN )");

		resetWnd();
	}
	else if (option == cApplicationOption::RESIZIBLE)
	{
		bool isResizible = va_arg(vlist, bool);
		if (isResizible != mWindowResizible)
		{
			mWindowResizible = isResizible;

			if (mWindowResizible)
				mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
			else
				mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;

			mLog->hout("cApplication::setOptions( RESIZIBLE, %s )", ( mWindowResizible ? "true":"false" ));

			SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
		}
	}
	else if (option == cApplicationOption::AUTO_AJUST_BY_SCREEN_SPACE)
	{
		mAutoAjustScale = va_arg(vlist, vec2f);
		mAutoAjustByScreen = true;
		mWindowed = true;		

		mLog->hout("cApplication::setOptions( AUTO_AJUST_BY_SCREEN_SPACE, v2(%.3f %.3f), %s )", 
			mAutoAjustScale.x, mAutoAjustScale.y, ( mAutoAjustByScreen ? "true":"false" ));

		resetWnd();
	}
	else if (option == cApplicationOption::WND_SIZE)
	{
		mWindowedSize = va_arg(vlist, vec2i);

		mLog->hout("cApplication::setOptions( WND_SIZE, v2((%i %i) )", 
			mWindowedSize.x, mWindowedSize.y);

		resetWnd();
	}
	else if (option == cApplicationOption::WND_POSITION)
	{
		mWindowedPos = va_arg(vlist, vec2i);

		mLog->hout("cApplication::setOptions( WND_POSITION, v2(%i %i) )", 
			mWindowedPos.x, mWindowedPos.y);

		resetWnd();
	}
	else if (option == cApplicationOption::CLIENT_RECT)
	{
		vec2i clientRectSize = va_arg(vlist, vec2i);

		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		clientRect.right = clientRect.left + clientRectSize.x;
		clientRect.bottom = clientRect.top + clientRectSize.y;

		AdjustWindowRect(&clientRect, mWndStyle, false);

		mWindowedPos = vec2i(clientRect.left, clientRect.top);
		mWindowedSize = vec2i(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

		mLog->hout("cApplication::setOptions( CLIENT_RECT, v2(%i %i) )", 
			clientRectSize.x, clientRectSize.y);

		resetWnd();
	}
	else if (option == cApplicationOption::WND_CAPTION)
	{
		mWndCaption = va_arg(vlist, std::string);

		mLog->hout("cApplication::setOptions( WND_CAPTION, %s )", mWndCaption.c_str());

		SetWindowText(mHWnd, mWndCaption.c_str());
	}

	va_end(vlist);
}

void cApplication::getOption( cApplicationOption::type option, ... )
{	
	va_list vlist;
	va_start(vlist, option);

	if (option == cApplicationOption::WINDOWED)
	{
		bool* res = va_arg(vlist, bool*);
		*res = mWindowed;
	}
	else if (option == cApplicationOption::FULLSCREEN)
	{
		bool* res = va_arg(vlist, bool*);
		*res = !mWindowed;
	}
	else if (option == cApplicationOption::RESIZIBLE)
	{		
		bool* res = va_arg(vlist, bool*);
		*res = mWindowResizible && mWindowed;
	}
	else if (option == cApplicationOption::AUTO_AJUST_BY_SCREEN_SPACE)
	{
		bool* res = va_arg(vlist, bool*);
		*res = mAutoAjustByScreen && mWindowed;
	}
	else if (option == cApplicationOption::WND_SIZE)
	{
		vec2i* res = va_arg(vlist, vec2i*);
		*res = mWindowedSize;
	}
	else if (option == cApplicationOption::WND_POSITION)
	{		
		vec2i* res = va_arg(vlist, vec2i*);
		*res = mWindowedPos;
	}
	else if (option == cApplicationOption::CLIENT_RECT)
	{		
		vec2i* res = va_arg(vlist, vec2i*);

		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);

		*res = vec2i(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}
	else if (option == cApplicationOption::WND_CAPTION)
	{
		std::string* res = va_arg(vlist, std::string*);
		*res = mWndCaption;
	}

	va_end(vlist);
}

void cApplication::processMessage( cApplacationMessage::type message )
{
	if (message == cApplacationMessage::ON_SIZING)
	{
		mRenderSystem->frameResized();
	}
}

void cApplication::resetWnd()
{
	if (mWindowed)
	{
		setWindowed();
	}
	else
	{
		setFullscreen();
	}
}

void cApplication::setWindowed()
{
	mLog->hout("Setting windowed..");

	mWindowed = true;

	RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
	AdjustWindowRect(&rt, mWndStyle, false);
	SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y, 
		         mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);

	if (mAutoAjustByScreen)
	{
		autoAjustByScreenSpace();
	}
	
	mLog->hout("Complete");
}

void cApplication::autoAjustByScreenSpace()
{
	mLog->hout("Setting autoAjustByScreenSpace");
}

void cApplication::setFullscreen()
{
	mLog->hout("Setting fullscreen");
}

void cApplication::onUpdate( float dt )
{
}

void cApplication::onDraw()
{

}

void cApplication::draw()
{
	mRenderSystem->beginRender();

	onDraw();

	mRenderSystem->endRender();
}

cApplication* cApplication::mApplication = NULL;

CLOSE_O2_NAMESPACE

#endif //PLATFORM_WIN