#pragma once

#include "ITestScreen.h"
#include "Render\Sprite.h"

class UITestScreen: public ITestScreen
{
public:
	UITestScreen(TestApplication* application);
	~UITestScreen();

	void Load();
	void Unload();

	void Update(float dt);
	void Draw();
	String GetId() const;

protected:
	Sprite mBackground;
};
