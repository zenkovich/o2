#pragma once

#include "Animation/Animatable.h"
#include "Application/Application.h"
#include "ApplicationConfig.h"
#include "Render/Sprite.h"

using namespace o2;

class WindowsManager;
class ApplicationConfig;

class Test: public Animatable
{
public:
	Sprite sp1;
	Sprite sp2;

	SERIALIZABLE(Test);
};

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

	Test test;

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
