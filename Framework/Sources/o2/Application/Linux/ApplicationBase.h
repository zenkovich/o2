#pragma once

#ifdef PLATFORM_LINUX

#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/String.h"

#include <X11/Xlib.h>

namespace o2
{
	class Application;

	// -------------------------------
	// Windows application base fields
	// -------------------------------
	class ApplicationBase
	{
	protected:
        ::Window mWindow;         // X Window
        Display* mDisplay;        // Display
        XVisualInfo* mVisualInfo; // Window visual info

		bool   mWindowed;        // True if app in windowed mode, false if in fullscreen mode
		bool   mWindowResizible; // True, if window can be sized by user
		Vec2I  mWindowedSize;    // Size of window
		Vec2I  mWindowedPos;     // Position of window
		String mWndCaption;      // Window caption
		bool   mActive;          // True, if window is active

		//HCURSOR mCurrentCursor;  // Current cursor type

		bool mNeedPlatformInitialization = true; // True, if need to initialize platform: window and render

	public:
		friend class Render;
		friend class FileSystem;
	};
}

#endif // PLATFORM_LINUX
