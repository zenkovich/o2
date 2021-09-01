#include "o2/stdafx.h"

#ifdef PLATFORM_MAC

#include "o2/Application/Application.h"
#include "o2/Events/EventSystem.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	void Application::InitializePlatform()
	{}

	void Application::Shutdown()
	{}

	void Application::SetFullscreen(bool fullscreen /*= true*/)
	{}

	void Application::CheckCursorInfiniteMode()
	{}

	void Application::Launch()
	{
		mLog->Out("Application launched!");

		OnStarted();
		onStarted.Invoke();
		o2Events.OnApplicationStarted();
	}

    void Application::Update()
    {
        ProcessFrame();
    }

	bool Application::IsFullScreen() const
	{
		return true;
	}

	void Application::Maximize()
	{}

	bool Application::IsMaximized() const
	{
		return true;
	}

	void Application::SetResizible(bool resizible)
	{}

	bool Application::IsResizible() const
	{
		return false;
	}

	void Application::SetWindowSize(const Vec2I& size)
	{}

	Vec2I Application::GetWindowSize() const
	{
		return Vec2I();
	}

	void Application::SetWindowPosition(const Vec2I& position)
	{}

	Vec2I Application::GetWindowPosition() const
	{
		return Vec2I();
	}

	void Application::SetWindowCaption(const String& caption)
	{}

	String Application::GetWindowCaption() const
	{
		return "";
	}

	void Application::SetContentSize(const Vec2I& size)
	{}

	Vec2I Application::GetContentSize() const
	{
		return mResolution; 
	}

	Vec2I Application::GetScreenResolution() const
	{
		return mResolution;
	}

	void Application::SetCursor(CursorType type)
	{}

	void Application::SetCursorPosition(const Vec2F& position)
	{}

	String Application::GetBinPath() const
	{
		return "";
	}
}

#endif // PLATFORM_MAC
