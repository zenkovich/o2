#include "public.h"

#ifdef RENDER_D3D9C

#include "texture_d3d9c.h"

#include "render_system/render_system.h"
#include "util/image/bitmap.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

Texture::Texture():
TextureBaseInterface(), mTexturePtr(0)
{
}

Texture::~Texture()
{
	if (mTexturePtr)
		mTexturePtr->Release();
}

void Texture::Create(const Vec2F& size, TextureFormat::type format /*= grTexFormat::DEFAULT*/,
					 TextureUsage::type usage /*= grTexUsage::DEFAULT*/)
{
	mFormat = format;
	mUsage = usage;
	mSize = size;

	D3DSURFACE_DESC desc;

	AppRender()->mBackBufferSurface->GetDesc(&desc);
	AppRender()->mBackBufferSurface->Release();

	D3DFORMAT texFormat = desc.Format;
	if (mFormat == TextureFormat::R8G8B8A8)
		texFormat = D3DFMT_A8R8G8B8;
	else if (mFormat == TextureFormat::R8G8B8)
		texFormat = D3DFMT_R8G8B8;

	DWORD dusage = 0;
	D3DPOOL dpool = D3DPOOL_MANAGED;
	if (usage == TextureUsage::RenderTarget)
	{
		dusage = D3DUSAGE_RENDERTARGET;
		dpool = D3DPOOL_DEFAULT;
	}

	if (FAILED(AppRender()->mDirect3DDevice->CreateTexture((unsigned int)mSize.x, (unsigned int)mSize.y,
		1, dusage, texFormat, dpool, &mTexturePtr, NULL)))
	{
		AppRender()->mLog->Error("Failed to create texture!");
		return;
	}
}

void Texture::CreateFromBitmap(Bitmap* image)
{

	Bitmap::Format imageFormat = image->GetFormat();
	if (imageFormat == Bitmap::Default)
		mFormat = TextureFormat::Default;
	else if (imageFormat == Bitmap::R8G8B8A8)
		mFormat = TextureFormat::R8G8B8A8;

	mUsage = TextureUsage::Default;
	mSize = image->GetSize().castTo<float>();
	mFileName = image->GetFilename();

	D3DFORMAT texFormat = D3DFMT_R8G8B8;
	if (mFormat == TextureFormat::R8G8B8A8)
		texFormat = D3DFMT_A8R8G8B8;
	else if (mFormat == TextureFormat::R8G8B8)
		texFormat = D3DFMT_R8G8B8;

	if (FAILED(AppRender()->mDirect3DDevice->CreateTexture((unsigned int)mSize.x, (unsigned int)mSize.y,
		1, 0, texFormat, D3DPOOL_MANAGED, &mTexturePtr, NULL)))
	{
		AppRender()->mLog->Error("Failed to create texture! Image:%s", image->GetFilename());
		return;
	}

	D3DLOCKED_RECT lockedRect;
	if (FAILED(mTexturePtr->LockRect(0, &lockedRect, 0, D3DLOCK_DISCARD)))
	{
		AppRender()->mLog->Error("Failed to lock texture rect");
		return;
	}

	unsigned char* psrc = (unsigned char*)image->GetData();
	for (unsigned int y = 0; y < (unsigned int)mSize.y; ++y)
	{
		unsigned char* ptr = (unsigned char*)lockedRect.pBits + lockedRect.Pitch*((unsigned int)mSize.y - y - 1);
		for (unsigned int i = 0; i < (unsigned int)mSize.x; ++i)
		{
			ptr[0] = psrc[2]; ptr[1] = psrc[1];
			ptr[2] = psrc[0]; ptr[3] = psrc[3];
			ptr+=4; psrc+=4;
		}
	}

	mTexturePtr->UnlockRect(0);
}

void Texture::CreateFromFile(const String& fileName)
{
	Bitmap* image = mnew Bitmap;
	if (image->Load(fileName, Bitmap::Auto, AppRender()->mLog))
		CreateFromBitmap(image);
	SafeRelease(image);
}

void Texture::CreateAsRenderTarget(const Vec2F& size, TextureFormat::type format /*= grTexFormat::DEFAULT*/)
{
	Create(size, format, TextureUsage::RenderTarget);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C