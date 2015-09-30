#include "TestApplication.h"

#include "ft2build.h"
#include FT_FREETYPE_H

IOBJECT_CPP(ReflectionTest);
IOBJECT_CPP(ReflectionTest2);

void TestApplication::OnStarted()
{
	Types::InitializeType<ReflectionTest>("ReflectionTest");
	Types::InitializeType<ReflectionTest2>("ReflectionTest2");

	auto refType = ReflectionTest::type;
	auto refType2 = ReflectionTest2::type;

	ReflectionTest2 rt;

	auto vp = rt.GetType().GetFieldPtr<char>(&rt, "refl/vec");
	auto vp2 = (char*)&rt.refl.vec;

	mSprite.LoadFromImage("ui_skin/hint_bk.png");

	mTestTween = Tween<Vec2F>::EaseInOut(Vec2F(), Vec2F(100, 0), 0.5f);
	mTestTween.bindProperty = &mSprite.position;

	mTestAnim.AddKey(0.0f, Vec2F(0.0f, 0.0f));
	mTestAnim.AddKey(2.0f, Vec2F(0.0f, 100.0f));
	mTestAnim.AddKey(1.0f, Vec2F(100.0f, 0.0f));
	mTestAnim.targetProperty = &mSprite.position;
}

void TestApplication::OnUpdate(float dt)
{
	mTestTween.Update(dt);
	mTestAnim.Update(dt);

	//o2Debug.Log("TWEEN %vf %b", mTestTween.GetValue(), mTestTween.GetState());

	o2Application.windowCaption = String::Format("FPS %f dt = %f", o2Time.GetFPS(), o2Time.GetDeltaTime());

	if (o2Input.IsKeyPressed('T'))
		mTestTween.state = !mTestTween.state;

	if (o2Input.IsKeyPressed('A'))
		mTestAnim.reversed ? mTestAnim.PlayForward() : mTestAnim.PlayBack();
}


void TestApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.camera = Camera::Default();

	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.resolution->y), Color4::Blue());

	for (auto key : mTestTween.transitionCurve.GetKeys())
	{
		Vector<Vec2F> kv;
		for (int i = 0; i < key.mApproxValuesCount; i++)
			kv += key.mApproxValues[i]*50.0f;

		o2Render.DrawLine(kv);
	}

	float tx = mTestTween.time/mTestTween.duration;
	o2Render.DrawCross(Vec2F(tx, mTestTween.transitionCurve.Evaluate(tx*mTestTween.transitionCurve.Length()))*50.0f);
}
