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
	Sprite mBackground;
	Sprite mSpr1;
	Sprite mSpr2;
	Sprite mSpr3;
	Sprite mSpr4;

protected:
	void RebuildStyle();
	void RebuildButtonStyle();
	void RebuildCloseButtonStyle();
	void RebuildArrowButtonStyle();
	void RebuildHorProgressBarStyle();
	void RebuildVerProgressBarStyle();
	void RebuildHorScrollBarStyle();
	void RebuildVerScrollBarStyle();
	void RebuildCheckboxStyle();
	void RebuildScrollAreaStyle();
	void RebuildScrollAreaStraightBarsStyle();
	void RebuildLabelStyle();
	void RebuildEditBoxStyle();
	void RebuildCustomListStyle();
	void RebuildListStyle();
	void RebuildCustomDropDownStyle();
	void RebuildDropDownStyle();
	void RebuildWindowStyle();
};

