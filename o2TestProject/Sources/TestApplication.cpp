#include "TestApplication.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#undef DrawText

void TestApplication::OnStarted()
{
	VectorFontAsset arialFont("arial.ttf");
	if (!arialFont.GetEffects().ContainsPred([](auto eff) { return eff->GetTypeId() == FontGradientEffect::type.ID(); }))
		arialFont.AddEffect<FontGradientEffect>();

	if (!arialFont.GetEffects().ContainsPred([](auto eff) { return eff->GetTypeId() == FontColorEffect::type.ID(); }))
		arialFont.AddEffect<FontColorEffect>(Color4(100, 100, 100, 255));

	if (!arialFont.GetEffects().ContainsPred([](auto eff) { return eff->GetTypeId() == FontShadowEffect::type.ID(); }))
		arialFont.AddEffect<FontShadowEffect>();
	arialFont.Save();

	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution;

	mWidget.AddSpriteLayer("regular", "ui/UI_button_regular.png", Layout::Both(-10, -10, -11, -11), 0.0f);
	auto selectLayer = mWidget.AddSpriteLayer("select", "ui/UI_button_select.png", Layout::Both(-10, -10, -11, -11), 1.0f);
	auto pressedLayer = mWidget.AddSpriteLayer("pressed", "ui/UI_button_pressed.png", Layout::Both(-10, -10, -11, -11), 2.0f);
 	mWidget.AddTextLayer("caption", "Button", "arial.ttf", Text::HorAlign::Middle, Text::VerAlign::Middle,
 						 Layout::Both(), 3.0f);
	mWidget.size = Vec2F(100, 30);

	Animation selectStateAnim;
	selectStateAnim.SetTarget(&mWidget);
	*selectStateAnim.AddAnimationValue<float>(&selectLayer->drawable->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);

	auto selectState = mWidget.AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(&mWidget);
	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->drawable->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);

	auto pressedState = mWidget.AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 0.5f;

	mWidget.onClick += []() { o2Debug.Log("Clicked!"); };

	mFakeWindow.LoadFromImage("ui/UI_window_frame_regular.png");
	mFakeWindow.size = Vec2F(400, 400);

	DataNode widgetData;
	widgetData = mWidget;
	widgetData.SaveToFile("widget.xml");

	//mText.font = FontRef("arial.ttf");
	mText.font = FontRef("myriad.xml");
	mText.ctext = "Hello, I'm text\nmulti line\nand many symbols\nha h a h ah\n1 2 3 4 5 6 7 8 9 0";
	//mText.ctext = "abcdefghklmnopqrstuvxyz12345";

	mHandleMin.regularSprite = mnew Sprite("ui/UI_Radio_bk.png");
	mHandleMin.hoverSprite = mnew Sprite("ui/UI_Radio_bk_select.png");
	mHandleMin.pressedSprite = mnew Sprite("ui/UI_Radio_bk_pressed.png");
	mHandleMin.onChangedPos += Function<void(const Vec2F&)>(this, &TestApplication::OnHandleMoved);

	mHandleMax.regularSprite = mnew Sprite("ui/UI_Radio_bk.png");
	mHandleMax.hoverSprite = mnew Sprite("ui/UI_Radio_bk_select.png");
	mHandleMax.pressedSprite = mnew Sprite("ui/UI_Radio_bk_pressed.png");
	mHandleMax.onChangedPos += Function<void(const Vec2F&)>(this, &TestApplication::OnHandleMoved);

	mHandleMin.position = Vec2F(-100, -100);
	mHandleMax.position = Vec2F(100, 100);
}

void TestApplication::OnUpdate(float dt)
{
	mWidget.Update(dt);

	o2Application.windowCaption = String::Format("FPS %f dt = %f", o2Time.GetFPS(), o2Time.GetDeltaTime());

	if (o2Input.IsKeyPressed('Z'))
	{
		o2Debug.Log("dbg");
		o2Memory.CollectGarbage();
	}

	if (o2Input.IsKeyPressed('Q'))
		mText.horAlign = Text::HorAlign::Left;

	if (o2Input.IsKeyPressed('W'))
		mText.horAlign = Text::HorAlign::Middle;

	if (o2Input.IsKeyPressed('E'))
		mText.horAlign = Text::HorAlign::Right;

	if (o2Input.IsKeyPressed('R'))
		mText.horAlign = Text::HorAlign::Both;

	if (o2Input.IsKeyPressed('A'))
		mText.verAlign = Text::VerAlign::Top;

	if (o2Input.IsKeyPressed('S'))
		mText.verAlign = Text::VerAlign::Middle;

	if (o2Input.IsKeyPressed('D'))
		mText.verAlign = Text::VerAlign::Bottom;

	if (o2Input.IsKeyPressed('F'))
		mText.verAlign = Text::VerAlign::Both;

	if (o2Input.IsKeyPressed('G'))
		mText.wordWrap = !mText.wordWrap;
}

void TestApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.camera = Camera::Default();

	mBackground.Draw();
	mFakeWindow.Draw();
	mWidget.Draw();
	mText.Draw();
	mHandleMin.Draw();
	mHandleMax.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F(1000, 0));
	o2Render.DrawRectFrame(mHandleMin.position, mHandleMax.position);
}

void TestApplication::OnHandleMoved(const Vec2F& pos)
{
	mText.SetRect(RectF(mHandleMin.position, mHandleMax.position));
}

