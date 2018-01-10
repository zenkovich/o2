#include "TextTestScreen.h"

#include "TestApplication.h"
#include "Render\Render.h"

TextTestScreen::TextTestScreen(TestApplication* application):
	ITestScreen(application)
{
}

TextTestScreen::~TextTestScreen()
{
}

void TextTestScreen::Load()
{
	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution;

	mFakeWindow.LoadFromImage("ui/UI_window_frame_regular.png");
	mFakeWindow.size = Vec2F(400, 400);

	mText.font = FontRef("stdFont.ttf");
	//mText.font = FontRef("myriad.xml");
	mText.text = "Hello, I'm text\nmulti line\nand many symbols\nha h a h ah\n1 2 3 4 5 6 7 8 9 0";
	//mText.ctext = "abcdefghklmnopqrstuvxyz12345";

	mHandleMin.SetRegularSprite(mnew Sprite("ui/UI_Radio_bk.png"));
	mHandleMin.SetHoverSprite(mnew Sprite("ui/UI_Radio_bk_select.png"));
	mHandleMin.SetPressedSprite(mnew Sprite("ui/UI_Radio_bk_pressed.png"));
	mHandleMin.onChangedPos += Function<void(const Vec2F&)>(this, &TextTestScreen::OnHandleMoved);

	mHandleMax.SetRegularSprite(mnew Sprite("ui/UI_Radio_bk.png"));
	mHandleMax.SetHoverSprite(mnew Sprite("ui/UI_Radio_bk_select.png"));
	mHandleMax.SetPressedSprite(mnew Sprite("ui/UI_Radio_bk_pressed.png"));
	mHandleMax.onChangedPos += Function<void(const Vec2F&)>(this, &TextTestScreen::OnHandleMoved);

	mHandleMin.position = Vec2F(-100, -100);
	mHandleMax.position = Vec2F(100, 100);
}

void TextTestScreen::Unload()
{
}

void TextTestScreen::Update(float dt)
{
	if (o2Input.IsKeyPressed('Q'))
		mText.horAlign = HorAlign::Left;

	if (o2Input.IsKeyPressed('W'))
		mText.horAlign = HorAlign::Middle;

	if (o2Input.IsKeyPressed('E'))
		mText.horAlign = HorAlign::Right;

	if (o2Input.IsKeyPressed('R'))
		mText.horAlign = HorAlign::Both;

	if (o2Input.IsKeyPressed('A'))
		mText.verAlign = VerAlign::Top;

	if (o2Input.IsKeyPressed('S'))
		mText.verAlign = VerAlign::Middle;

	if (o2Input.IsKeyPressed('D'))
		mText.verAlign = VerAlign::Bottom;

	if (o2Input.IsKeyPressed('F'))
		mText.verAlign = VerAlign::Both;

	if (o2Input.IsKeyPressed('G'))
		mText.wordWrap = !mText.wordWrap;

	if (o2Input.IsKeyPressed(VK_ESCAPE))
		mApplication->GoToScreen("MainTestScreen");
}

void TextTestScreen::Draw()
{
	mBackground.Draw();
	mFakeWindow.Draw();
	mText.Draw();
	mHandleMin.Draw();
	mHandleMax.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F(1000, 0));
	o2Render.DrawRectFrame(mHandleMin.position, mHandleMax.position);
}

String TextTestScreen::GetId() const
{
	return "TextTestScreen";
}

void TextTestScreen::OnHandleMoved(const Vec2F& pos)
{
	mText.SetRect(RectF(mHandleMin.position, mHandleMax.position));
}
