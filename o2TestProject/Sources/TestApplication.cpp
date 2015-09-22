#include "TestApplication.h"

#include "ft2build.h"
#include FT_FREETYPE_H

void TestApplication::OnStarted()
{
	mTexture = TextureRef(o2Render.GetResolution(), Texture::Format::R8G8B8A8, Texture::Usage::RenderTarget);

	mSprite2.texture = mTexture;
	mSprite2.textureSrcRect = RectI(Vec2I(), mTexture->size);
	mSprite2.NormalizeSize();
	mSprite2.pivot = Vec2F(0.0f, 1.0f);
	mSprite2.scale = Vec2F(0.2f, 0.2f);
	mSprite2.position = Vec2F(-o2Render.resolution->x*0.5f, o2Render.resolution->y*0.5f);

	mBasisTest = Basis::Build(Vec2F(), Vec2F(100, 100), 0.0f, Math::Cos(Math::Deg2rad(45.0f)));

	mFont.Load(o2Assets.GetDataPath() + "myriad.xml");
	mVFont.Load("C:\\Windows\\Fonts\\Arial.ttf");
	mVFont.AddEffect(mnew FontGradientEffect());
	mVFont.AddEffect(mnew FontStrokeEffect());
	mVFont.AddEffect(mnew FontShadowEffect());

	mText = mnew Text(&mVFont);
	mText->ctext = "Heello' I'm text\nmulti line";

	mSprite.LoadFromImage("ui_skin/hint_bk.png");
	mSprite.mode = Sprite::Mode::Sliced;
	mSprite.sliceBorder = RectI(20, 20, 20, 20);

	Bitmap b(o2Assets.GetAssetsPath() + "ui_skin/hint_bk.png");
	b.Outline(5, Color4::Red());
	b.Save("blur.png", Bitmap::ImageType::Png);
}

void TestApplication::OnUpdate(float dt)
{
	o2Application.windowCaption = String::Format("FPS %f", o2Time.GetFPS());

	if (o2Input.IsKeyPressed('Z'))
	{
		mSprite.mode = Sprite::Mode::Default;
		mText->ctext += "ghghghg";
	}

	if (o2Input.IsKeyPressed('X'))
		mSprite.mode = Sprite::Mode::Sliced;

	if (o2Input.IsKeyPressed('C'))
		mSprite.mode = Sprite::Mode::FillLeftToRight;

	if (o2Input.IsKeyPressed('V'))
		mSprite.mode = Sprite::Mode::FillRightToLeft;

	if (o2Input.IsKeyPressed('B'))
		mSprite.mode = Sprite::Mode::FillUpToDown;

	if (o2Input.IsKeyPressed('N'))
		mSprite.mode = Sprite::Mode::FillDownToUp;

	if (o2Input.IsKeyPressed('M'))
		mSprite.mode = Sprite::Mode::Fill360CW;

	if (o2Input.IsKeyPressed('L'))
		mSprite.mode = Sprite::Mode::Fill360CCW;

	if (o2Input.IsKeyDown('K'))
	{
		mSprite.fill += o2Input.cursorDelta->x/100.0f;
	}

	Ptr<IRectDrawable> cd = &mSprite;

	if (o2Input.IsCursorDown())
		cd->size += o2Input.GetCursorDelta();

	if (o2Input.IsAltCursorDown())
		cd->angle += o2Input.GetCursorDelta().x*0.01f;

	if (o2Input.IsAlt2CursorDown())
		cd->position += o2Input.GetCursorDelta();

	if (o2Input.IsKeyPressed('Q'))
		mText->horAlign = Text::HorAlign::Left;

	if (o2Input.IsKeyPressed('W'))
		mText->horAlign = Text::HorAlign::Middle;

	if (o2Input.IsKeyPressed('E'))
		mText->horAlign = Text::HorAlign::Right;

	if (o2Input.IsKeyPressed('R'))
		mText->horAlign = Text::HorAlign::Both;

	if (o2Input.IsKeyPressed('A'))
		mText->verAlign = Text::VerAlign::Top;

	if (o2Input.IsKeyPressed('S'))
		mText->verAlign = Text::VerAlign::Middle;

	if (o2Input.IsKeyPressed('D'))
		mText->verAlign = Text::VerAlign::Bottom;

	if (o2Input.IsKeyPressed('F'))
		mText->verAlign = Text::VerAlign::Both;
}

void TestApplication::OnDraw()
{
	//mText->ctext = (String)o2Time.GetCurrentFrame();

	o2Render.Clear(Color4::Gray());
	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.resolution->y), Color4::Blue());

	o2Render.camera = mCamera;
	o2Render.renderTexture = mTexture;
	o2Render.Clear(Color4::Gray());
	mSprite3.Draw();
	mText->Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.resolution->y), Color4::Blue());
	o2Render.renderTexture = NoTexture();

	//o2Render.CurrentCamera = Camera::Default();
	mSprite2.Draw();
	mSprite3.Draw();
	mSprite.Draw();

	o2Render.DrawBasis(mCamera.GetBasis());
	mText->Draw();
	o2Render.DrawBasis(mText->GetBasis());
	o2Render.DrawRectFrame(mText->AABB);

	o2Render.DrawLine(Vec2F(), Vec2F::Rotated(Math::Deg2rad(mSprite.fill*-360.0f + 90.0f))*100.0f);
}
