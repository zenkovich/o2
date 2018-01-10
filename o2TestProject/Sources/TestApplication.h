#pragma once

#include "ITestScreen.h"
#include "Application/Application.h"

using namespace o2;

class TestApplication: public Application
{
protected:
	// Calls when application is starting
	void OnStarted();

	// Called on updating
	void OnUpdate(float dt);

	// Called on drawing
	void OnDraw();

public:
	// Turns on test screen with id
	void GoToScreen(const String& id);

protected:
	TestScreensVec mTestScreens;
	ITestScreen*   mCurrentScreen;
	ITestScreen*   mNextCurrentScreen;

	Vec2F          mRulerStartPoint;

	void CheckArialFontEffects();
};
