#pragma once

#include "O2.h"
#include "ITestScreen.h"

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
	TestScreensVec   mTestScreens;
	Ptr<ITestScreen> mCurrentScreen;
	Ptr<ITestScreen> mNextCurrentScreen;

	void CheckArialFontEffects();
};