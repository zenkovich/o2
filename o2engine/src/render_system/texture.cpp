#include "texture.h"

#include "render_system.h"
#include "util/image/bitmap.h"

OPEN_O2_NAMESPACE

TextureRef::TextureRef(Texture* object):
cObjectRef(object)
{
	if (object)
	{
		mSize = object->mSize;
		mFormat = object->mFormat;
		mUsage = object->mUsage;
		mFileName = object->mFileName;
	}
	else
	{
		mSize = Vec2F(1, 1);
		mFormat = TextureFormat::Default;
		mUsage = TextureUsage::Default;
	}
}

TextureRef::operator Texture*()
{
	return mObject;
}

TextureRef TextureRef::Create(const Vec2F& size, TextureFormat format /*= grTexFormat::DEFAULT*/,
							  TextureUsage usage /*= grTexUsage::DEFAULT*/)
{
	return TextureRef(AppRender()->CreateTexture(size, format, usage));
}

TextureRef TextureRef::CreateFromBitmap(Bitmap* image)
{
	return TextureRef(AppRender()->CreateTextureFromBitmap(image));
}

TextureRef TextureRef::CreateFromFile(const String& fileName)
{
	return TextureRef(AppRender()->GetTextureFromFile(fileName));
}

TextureRef TextureRef::CreateAsRenderTarget(const Vec2F& size, TextureFormat format /*= grTexFormat::DEFAULT*/)
{
	return TextureRef(AppRender()->CreateRenderTargetTexture(size, format));
}

CLOSE_O2_NAMESPACE