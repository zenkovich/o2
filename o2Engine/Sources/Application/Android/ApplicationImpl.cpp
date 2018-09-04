#include "stdafx.h"

#ifdef PLATFORM_ANDROID

#include "Application/Application.h"
#include "Events/EventSystem.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/FileSystem/FileSystem.h"

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
	{}

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
		return Vec2I();
	}

	Vec2I Application::GetScreenResolution() const
	{
		return Vec2I(100, 100);
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

#endif // PLATFORM_ANDROID