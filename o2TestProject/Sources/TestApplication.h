#pragma once

#include "O2.h"

using namespace o2;

class TestApplication: public BaseApplication
{
protected:
	// Calls when application is starting
	void OnStarted();

	// Called on updating
	void OnUpdate(float dt);

	// Called on drawing
	void OnDraw();

protected:
	Sprite     mSprite;
	Sprite     mSprite2;
	Sprite     mSprite3;
	TextureRef mTexture;
	Camera     mCamera;
	Basis      mBasisTest;
	Basis      mBasisTest2;
	Font       mFont;
	Ptr<Text>  mText;
};