#pragma once

#include "O2.h"

using namespace o2;

class ReflectionTest: public IObject
{
public:
	Vec2F vec;
	float flt;

	IOBJECT(ReflectionTest)
	{
		FIELD(vec);
		FIELD(flt);
	}
};

class ReflectionTest2: public IObject
{
public:
	Vec2F vec;
	float flt;

	ReflectionTest refl;
	Ptr<ReflectionTest> prefl;

	IOBJECT(ReflectionTest2)
	{
		FIELD(vec);
		FIELD(flt);
		FIELD(refl);
		FIELD(prefl);
	}
};

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
	Sprite       mSprite;
	Tween<Vec2F> mTestTween;
	AnimatedValue<Vec2F> mTestAnim;

protected:
};