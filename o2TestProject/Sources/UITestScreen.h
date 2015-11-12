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
	Ptr<UIButton>              mButton;
	Ptr<UIHorizontalLayout>    mHorLayout;
	Ptr<UIHorizontalLayout>    mHorLayout2;
	Ptr<UIHorizontalProgress>  mBar;
	Ptr<UIHorizontalScrollBar> mScroll;

protected:
	void CreateNewBtn();

	void CheckStyle();
	bool CheckButtonStyle();
	bool CheckHorProgressBarStyle();
	bool CheckVerProgressBarStyle();
	bool CheckHorScrollBarStyle();
	bool CheckVerScrollBarStyle();
	bool CheckCheckboxStyle();
	bool CheckScrollArea();
	bool CheckLabel();
	bool CheckEditBox();
};

