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
	Sprite                     mBackground;
	Sprite                     mFakeWindow;

protected:
	void CheckStyle();
	bool CheckButtonStyle();
	bool CheckHorProgressBarStyle();
	bool CheckVerProgressBarStyle();
	bool CheckHorScrollBarStyle();
	bool CheckVerScrollBarStyle();
	bool CheckCheckboxStyle();
	bool CheckScrollArea();
	bool CheckScrollAreaStraightBars();
	bool CheckLabel();
	bool CheckEditBox();
	bool CheckCustomList();
	bool CheckList();
	bool CheckCustomDropDown();
};

