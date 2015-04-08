#include "render_system_base_interface.h"

#include "app/application.h"
#include "font_manager.h"
#include "texture.h"
#include "util/image/bitmap.h"
#include "util/log.h"
#include "util/log/file_log_stream.h"

OPEN_O2_NAMESPACE

RenderSystemBaseInterface::RenderSystemBaseInterface():
mCurrentCamera(NULL)
{
	mLog = mnew FileLogStream("Render", gLog->GetLevel(), "render_log.txt");
	gLog->BindStream(mLog);
	mFontManager = mnew FontManager();

	InitializeProperties();
}

RenderSystemBaseInterface::~RenderSystemBaseInterface()
{
	SafeRelease(mFontManager);
	gLog->UnbindStream(mLog);
}

void RenderSystemBaseInterface::BindCamera(Camera* camera)
{
	mCurrentCamera = camera;
	UpdateCameraTransforms();
}

Camera* RenderSystemBaseInterface::CurrentCamera() const
{
	return mCurrentCamera;
}

FontManager* RenderSystemBaseInterface::GetFontManager() const
{
	return mFontManager;
}

TextureRef RenderSystemBaseInterface::GetTextureFromFile(const String& fileName)
{
	foreach(mTextures, it)
	{
		if (fileName == (*it)->GetFileName())
		{
			return TextureRef(*it);
		}
	}

	Texture* newTexture = mnew Texture();
	newTexture->CreateFromFile(fileName);
	AddTexture(newTexture);

	mLog->HOut("Created texture '%s'", fileName.c_str());

	return TextureRef(newTexture);
}

TextureRef RenderSystemBaseInterface::CreateTexture(const Vec2F& size,
													TextureFormat format /*= grTexFormat::DEFAULT*/,
													TextureUsage usage /*= grTexUsage::DEFAULT*/)
{
	Texture* res = mnew Texture();
	res->Create(size, format, usage);
	AddTexture(res);

	mLog->HOut("Created texture %ix%i, format: %s, usage: %s", (int)size.x, (int)size.y, AsString(format),
			   AsString(format)); //TODO fix output strings!

	return TextureRef(res);
}

TextureRef RenderSystemBaseInterface::CreateTextureFromBitmap(Bitmap* image)
{
	Texture* res = mnew Texture();
	res->CreateFromBitmap(image);
	AddTexture(res);

	mLog->HOut("Created texture %ix%i, format: %s, usage: %s from image", (int)res->GetSize().x, (int)res->GetSize().y,
			   AsString(format), AsString(format)); //TODO fix output strings!

	return TextureRef(res);
}

TextureRef RenderSystemBaseInterface::CreateRenderTargetTexture(const Vec2F& size,
																TextureFormat format /*= grTexFormat::DEFAULT*/)
{
	Texture* res = mnew Texture();
	res->CreateAsRenderTarget(size, format);
	AddTexture(res);

	mLog->HOut("Created render texture %ix%i, format: %s", (int)size.x, (int)size.y, "asd"); //TODO

	return TextureRef(res);
}

Vec2I RenderSystemBaseInterface::GetResolution() const
{
	return mResolution;
}

void RenderSystemBaseInterface::DrawLine(const Vec2F& a, const Vec2F& b, const Color4 color /*= color4(255)*/)
{
	unsigned long dcolor = color.AsULong();
	Vertex2 v[] ={Vertex2(a.x, a.y, dcolor, 0, 0), Vertex2(b.x, b.y, dcolor, 0, 0)};
	DrawLines(v, 1);
}

void RenderSystemBaseInterface::DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4 color /*= color4(255)*/)
{
	unsigned long dcolor = color.AsULong();
	Vertex2 v[] ={
		Vertex2(minp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, minp.y, dcolor, 0, 0),
		Vertex2(maxp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, maxp.y, dcolor, 0, 0),
		Vertex2(maxp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, maxp.y, dcolor, 0, 0),
		Vertex2(minp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, minp.y, dcolor, 0, 0)
	};
	DrawLines(v, 4);
}

void RenderSystemBaseInterface::DrawCross(const Vec2F& pos, float size /*= 5*/, const Color4 color /*= color4(255)*/)
{
	unsigned long dcolor = color.AsULong();
	Vertex2 v[] ={
		Vertex2(pos.x - size, pos.y, dcolor, 0, 0), Vertex2(pos.x + size, pos.y, dcolor, 0, 0),
		Vertex2(pos.x, pos.y - size, dcolor, 0, 0), Vertex2(pos.x, pos.y + size, dcolor, 0, 0)};
	DrawLines(v, 2);
}

void RenderSystemBaseInterface::DrawCircle(const Vec2F& pos, float radius /*= 5*/, const Color4 color /*= color4::white()*/)
{
	const int segCount = 20;
	Vertex2 v[segCount*2];
	unsigned long dcolor = color.AsULong();

	float angleSeg = 2.0f*PI/(float)(segCount - 1);
	for (int i = 0; i < segCount; i++)
	{
		float a = (float)i*angleSeg;
		v[i*2]     = Vertex2(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
		v[i*2 + 1] = Vertex2(Vec2F::Rotated(a + angleSeg)*radius + pos, dcolor, 0, 0);
	}

	DrawLines(v, segCount);
}

Texture* RenderSystemBaseInterface::AddTexture(Texture* texture)
{
	mTextures.Add(texture);
	return texture;
}

void RenderSystemBaseInterface::RemoveTexture(Texture* texture)
{
	if (!texture/* || texture->getRefCount() > 0*/)
		return;

	mTextures.Remove(texture);

	SafeRelease(texture);
}

void RenderSystemBaseInterface::RemoveAllTextures()
{
	ReleaseArray(mTextures);
}

void RenderSystemBaseInterface::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(RenderSystemBaseInterface, camera, BindCamera, CurrentCamera);
}

CLOSE_O2_NAMESPACE