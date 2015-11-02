#pragma once

#include "ITestScreen.h"

class UITestScreen: public ITestScreen
{
public:
	UITestScreen(Ptr<TestApplication> application);
	~UITestScreen();

	void Load();
	void Unload();

	void Update(float dt);
	void Draw();
	String GetId() const;

protected:
	Sprite      mBackground;
	Sprite      mFakeWindow;
	Ptr<Button> mButton;
	Ptr<HorizontalLayout> mHorLayout;
	Ptr<HorizontalLayout>   mHorLayout2;

protected:
	void CreateNewBtn();
};

