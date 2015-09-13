#include "TestApplication.h"

void TestApplication::OnStarted()
{
	FolderAsset folder("Images");

	ImageAsset image("Images/beam3.png");

	AtlasAsset atlas("BasicAtlas.atlas");

	mSprite.LoadFromImage("Images/hero_cat.png");
	mTexture = TextureRef(Vec2F(256, 256), Texture::Format::R8G8B8A8, Texture::Usage::RenderTarget);
	mSprite2.TexturePtr = mTexture;
	mSprite2.TexSrcRect = RectI(Vec2I(), mTexture->Size);
	mSprite2.NormalizeSize();
}

void TestApplication::OnUpdate(float dt)
{
	mSprite.Position += Vec2F(10, 10)*dt;
	o2Application.WindowCaption = String::Format("FPS %f", o2Time.GetFPS());
}

void TestApplication::OnDraw()
{
	o2Render.Clear(Color4::Gray());
	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F(o2Render.Resolution->x, 0), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, o2Render.Resolution->y), Color4::Blue());
}
