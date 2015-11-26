#pragma once

#include "O2.h"
using namespace o2;

#include "ApplicationConfig.h"

class WindowsManager;
class ApplicationConfig;

// ------------------
// Editor application
// ------------------
class EditorApplication: public Application
{
public:
	// Default constructor. Initializes all editor components
	EditorApplication();

	// Destructor
	~EditorApplication();

protected:
	Ptr<Sprite>            mBackground;     // Background sprite
	Ptr<Sprite>            mBackSign;       // Background o2 signature
	Ptr<WindowsManager>    mWindowsManager; // Windows manager
	Ptr<ApplicationConfig> mConfig;         // Application configuration

protected:
	// Calling on updating
	void OnUpdate(float dt);

	// Calling on drawing
	void OnDraw();

	// Calling when application activated
	void OnActivated();

	// Calling when application deactivated
	void OnDeactivated();

	// Calling when application is starting
	void OnStarted();

	// Calling when application is closing
	void OnClosing();

	// Calling when application window resized. Ignoring on mobiles/tablets
	void OnResizing();
};
