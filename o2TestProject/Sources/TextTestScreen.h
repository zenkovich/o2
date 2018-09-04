#pragma once

#include "ITestScreen.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Utils/Editor/DragHandle.h"

class TextTestScreen: public ITestScreen
{
public:
	TextTestScreen(TestApplication* application);
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
