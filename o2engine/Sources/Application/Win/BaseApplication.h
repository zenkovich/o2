#pragma once

#include <Windows.h>
#include "Application/IApplication.h"

namespace o2
{
	// ----------------------------------
	// Windows application implementation 
	// ---------------------------------- 
	class BaseApplication: public IApplication
	{
	public:
		// Default constructor. */
		BaseApplication();

		// Destructor
		~BaseApplication();

		// Launching application cycle
		void Launch();

		// Makes application fullscreen. On mobiles/tablets has no effect, just ignoring
		void SetFullscreen(bool fullscreen = true);

		// Return true, if application is fullscreen On mobiles/tables always true
		bool IsFullScreen() const;

		// Sets application window as resizible. On mobiles/tablets has no effect, just ignoring
		void SetResizible(bool resizible);

		// Returns true, if application is resizible. On mobiles/tablets always returns false
		bool IsResizible() const;

		// TODO: Doesn't work when application not yet launched
		// Sets application window size. On mobiles/tablets has no effect, just ignoring
		void SetWindowSize(const Vec2I& size); 

		// Returns application window size. On mobiles/tablets returns content size
		Vec2I GetWindowSize() const;

		// TODO: Doesn't work when application not yet launched
		// Sets application window position. On mobiles/tablets has no effect, just ignoring
		void SetWindowPosition(const Vec2I& position);

		// Returns application window position. On mobiles/tablets return zero vector
		Vec2I GetWindowPosition() const;

		// Sets application window caption. On mobiles/tablets has no effect, just ignoring
		void SetWindowCaption(const String& caption);

		// Returns application window caption. On mobiles/tablets returns empty string
		String GetWindowCaption() const;

		// TODO: Doesn't work when application not yet launched
		// Sets inside content size
		void SetContentSize(const Vec2I& size);

		// Returns inside content size
		Vec2I GetContentSize() const;

	protected:
		HWND   mHWnd;            // Window handle
		UInt   mWndStyle;        // Window styles data
		bool   mWindowed;        // True if app in windowed mode, false if in fullscreen mode
		bool   mWindowResizible; // True, if window can be sized by user
		Vec2I  mWindowedSize;    // Size of window
		Vec2I  mWindowedPos;     // Position of window
		String mWndCaption;      // Window caption
		bool   mActive;          // True, if window is active

		static BaseApplication* mApplication;   // Static pointer for Window proc function

	protected:
		// Called on updating
		void OnUpdate(float dt);

		// Called on drawing
		void OnDraw();

		// Initializing window
		void InitializeWindow();

		// Reset window by window parameters
		void ResetWnd();

		//void InitializeProperties();

		// Window proc function
		static LRESULT WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		friend class Render;
	};
}