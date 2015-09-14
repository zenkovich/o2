#include "TestApplication.h"

void TestApplication::OnStarted()
{
	FolderAsset folder("Images");

	ImageAsset image("Images/beam3.png");

	AtlasAsset atlas("BasicAtlas.atlas");

	mSprite.LoadFromImage("Images/pic.png");

	mTexture = TextureRef(o2Render.GetResolution(), Texture::Format::R8G8B8A8, Texture::Usage::RenderTarget);

	mSprite2.TexturePtr = mTexture;
	mSprite2.TexSrcRect = RectI(Vec2I(), mTexture->Size);
	mSprite2.NormalizeSize();
	mSprite2.Pivot = Vec2F(0.0f, 1.0f);
	mSprite2.Scale = Vec2F(0.2f, 0.2f);
	mSprite2.Position = Vec2F(-o2Render.Resolution->x*0.5f, o2Render.Resolution->y*0.5f);

	mBasisTest = Basis::Build(Vec2F(), Vec2F(100, 100), 0.0f, Math::Cos(Math::Deg2rad(45.0f)));
}

void TestApplication::OnUpdate(float dt)
{
	o2Application.WindowCaption = String::Format("FPS %f", o2Time.GetFPS());

	if (o2Input.IsCursorDown())
		mCamera.Position += o2Input.GetCursorDelta();

	if (o2Input.IsAltCursorDown())
		mCamera.Angle += o2Input.GetCursorDelta().x*0.01f;

	mCamera.Scale = mCamera.Scale.Get()*(1.0f + o2Input.GetMouseWheelDelta()/100.0f);

// 	if (o2Input.IsCursorDown())
// 	{
// 		RectF rt = mBasisTest.AABB();
// 		RectF crt(rt.left, rt.bottom, rt.right + o2Input.GetCursorDelta().x, rt.top + o2Input.GetCursorDelta().y);
// 		Basis brt(rt.LeftBottom(), Vec2F::Right()*rt.Width(), Vec2F::Up()*rt.Height());
// 		Basis bcrt(crt.LeftBottom(), Vec2F::Right()*crt.Width(), Vec2F::Up()*crt.Height());
// 		Basis t = bcrt*brt.Inverted();
// 		mBasisTest = mBasisTest*brt.Inverted()*bcrt;
// 	}
// 
// 	if (o2Input.IsAltCursorDown())
// 	{
// 		mBasisTest = mBasisTest*Basis::Rotated(o2Input.GetCursorDelta().x*0.01f);
// 	}
// 
// 	if (o2Input.IsAlt2CursorDown())
// 	{
// 		mBasisTest.Translate(o2Input.GetCursorDelta());
// 	}
// 
// 	Vec2F off, scale;
// 	float angle, shift;
// 	mBasisTest.Decompose(&off, &angle, &scale, &shift);
// 
// 	mBasisTest2 = Basis::Build(off, scale, angle, shift);
// 	
// 	if (o2Input.IsCursorDown())
// 	{
// 		RectF rt = mSprite.GetAxisAlignedRect();
// 		rt.right += o2Input.GetCursorDelta().x;
// 		rt.top += o2Input.GetCursorDelta().y;
// 		mSprite.SetAxisAlignedRect(rt);
// 	}
// 
// 	if (o2Input.IsAltCursorDown())
// 		mSprite.Angle += o2Input.GetCursorDelta().x*0.01f;
// 
// 	if (o2Input.IsAlt2CursorPressed())
// 		mSprite.WorldPivot = o2Input.GetCursorPos();
}

void TestApplication::OnDraw()
{
	o2Render.Clear(Color4::Gray());
	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.Resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.Resolution->y), Color4::Blue());

	o2Render.CurrentCamera = mCamera;
	o2Render.RenderTexture = mTexture;
	o2Render.Clear(Color4::Red());
	mSprite.Draw();
	o2Render.DrawLine(Vec2F(), Vec2F((float)o2Render.Resolution->x, 0.0f), Color4::Red());
	o2Render.DrawLine(Vec2F(), Vec2F(0.0f, (float)o2Render.Resolution->y), Color4::Blue());
	o2Render.RenderTexture = NoTexture();

	o2Render.CurrentCamera = Camera::Default();
	mSprite2.Draw();

	o2Render.DrawBasis(mCamera.GetBasis());
}
