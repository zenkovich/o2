#pragma once

#include "ITestScreen.h"

class TextTestScreen: public ITestScreen
{
public:
	TextTestScreen(Ptr<TestApplication> application);
	~TextTestScreen();

	void Load();
	void Unload();

	void Update(float dt);
	void Draw();
	String GetId() const;

protected:
	Sprite mBackground;
	Sprite mFakeWindow;
	DragHandle mHandleMin;
	DragHandle mHandleMax;
	Text mText;

	void OnHandleMoved(const Vec2F& pos);
};