#include "TestApplication.h"

#include "ft2build.h"
#include FT_FREETYPE_H

void TestApplication::OnStarted()
{
	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("FT Error: %i", error);
	}

	FT_Face face;
	error = FT_New_Face(library, "C:\\Windows\\Fonts\\Arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		printf("FT unknown format");
	}
	else if (error)
	{
		printf("FT error %i", error);
	}

	error = FT_Set_Char_Size(
		face,    /* handle to face object           */
		0,       /* char_width in 1/64th of points  */
		640*64,   /* char_height in 1/64th of points */
		300,     /* horizontal device resolution    */
		300);   /* vertical device resolution      */

	if (error)
	{
		printf("FT error %i", error);
	}

	error = FT_Set_Pixel_Sizes(
		face,   /* handle to face object */
		0,      /* pixel_width           */
		66);   /* pixel_height          */

	if (error)
	{
		printf("FT error %i", error);
	}

	auto glyph_index = FT_Get_Char_Index(face, '4');

	error = FT_Load_Glyph(
		face,          /* handle to face object */
		glyph_index,   /* glyph index           */
		FT_LOAD_DEFAULT);  /* load flags, see below */

	if (error)
	{
		printf("FT error %i", error);
	}

	error = FT_Render_Glyph(face->glyph,   /* glyph slot  */
							FT_RENDER_MODE_NORMAL); /* render mode */

	if (error)
	{
		printf("FT error %i", error);
	}

	Bitmap bt(Bitmap::Format::R8G8B8A8, Vec2F(100, 100));
	int ps = 4;
	for (int i = 0; i < face->glyph->bitmap.width; i++)
	{
		for (int j = 0; j < face->glyph->bitmap_top; j++)
		{
			Color4 c(255, 255, 255, face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + i]);
			auto dc = c.ABGR();
			memcpy(&bt.GetData()[((100-j-30)*bt.GetSize().x + i + 30)*ps], &dc, ps);
		}
	}

	bt.Save("glyph.png", Bitmap::ImageType::Png);

	mSprite3.LoadFromBitmap(&bt);

	FolderAsset folder("Images");

	ImageAsset image("Images/beam3.png");

	AtlasAsset atlas("BasicAtlas.atlas");

	mTexture = TextureRef(o2Render.GetResolution(), Texture::Format::R8G8B8A8, Texture::Usage::RenderTarget);

	mSprite2.texture = mTexture;
	mSprite2.textureSrcRect = RectI(Vec2I(), mTexture->size);
	mSprite2.NormalizeSize();
	mSprite2.pivot = Vec2F(0.0f, 1.0f);
	mSprite2.scale = Vec2F(0.2f, 0.2f);
	mSprite2.position = Vec2F(-o2Render.resolution->x*0.5f, o2Render.resolution->y*0.5f);

	mBasisTest = Basis::Build(Vec2F(), Vec2F(100, 100), 0.0f, Math::Cos(Math::Deg2rad(45.0f)));

	mFont.CreateFromBMFont(o2Assets.GetDataPath() + "myriad.xml");
	mText = mnew Text(&mFont);
	mText->ctext = "Heello' I'm text\nmulti line";

	mSprite.LoadFromImage("ui_skin/hint_bk.png");
	mSprite.mode = Sprite::Mode::Sliced;
	mSprite.sliceBorder = RectI(20, 20, 20, 20);
}

void TestApplication::OnUpdate(float dt)
{
	o2Application.windowCaption = String::Format("FPS %f", o2Time.GetFPS());

	if (o2Input.IsKeyPressed('Z'))
		mSprite.mode = Sprite::Mode::Default;

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
		o2Debug.Log("FILL %f", (float)mSprite.fill);
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
	o2Render.Clear(Color4::Gray());
	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.resolution->y), Color4::Blue());

	o2Render.camera = mCamera;
	o2Render.renderTexture = mTexture;
	o2Render.Clear(Color4::Gray());
	mSprite.Draw();
	mSprite3.Draw();
	mText->Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.resolution->y), Color4::Blue());
	o2Render.renderTexture = NoTexture();

	//o2Render.CurrentCamera = Camera::Default();
	mSprite2.Draw();
	mSprite3.Draw();

	o2Render.DrawBasis(mCamera.GetBasis());
	mText->Draw();
	o2Render.DrawBasis(mText->GetBasis());
	o2Render.DrawRectFrame(mText->AABB);

	o2Render.DrawLine(Vec2F(), Vec2F::Rotated(Math::Deg2rad(mSprite.fill*-360.0f + 90.0f))*100.0f);
}
