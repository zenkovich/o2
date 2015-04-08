#include "public.h"

#ifdef RENDER_OGL

#include "render_system/render_system.h"
#include "texture_ogl.h"
#include "util/image/bitmap.h"

OPEN_O2_NAMESPACE

Texture::Texture():
TextureBaseInterface(), mHandle(0)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &mHandle);
}

void Texture::Create(const Vec2F& size, TextureFormat format /*= grTexFormat::DEFAULT*/,
					 TextureUsage usage /*= grTexUsage::DEFAULT*/)
{
	mFormat = format;
	mUsage = usage;
	mSize = size;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (format == TextureFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (format == TextureFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, (GLsizei)size.x, (GLsizei)size.y, 0, texFormat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::CreateFromBitmap(Bitmap* image)
{
	Bitmap::Format imageFormat = image->GetFormat();
	if (imageFormat == Bitmap::Format::Default)
		mFormat = TextureFormat::Default;
	else if (imageFormat == Bitmap::Format::R8G8B8A8)
		mFormat = TextureFormat::R8G8B8A8;

	mUsage = TextureUsage::Default;
	mSize = image->GetSize();
	mFileName = image->GetFilename();

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (mFormat == TextureFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (mFormat == TextureFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, image->GetSize().x, image->GetSize().y, 0, texFormat, GL_UNSIGNED_BYTE,
				 image->GetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::CreateFromFile(const String& fileName)
{
	Bitmap* image = mnew Bitmap;
	if (image->Load(fileName, Bitmap::ImageType::Auto, AppRender()->mLog))
		CreateFromBitmap(image);
	SafeRelease(image);
}

void Texture::CreateAsRenderTarget(const Vec2F& size, TextureFormat format /*= grTexFormat::DEFAULT*/)
{
	Create(size, format, TextureUsage::RenderTarget);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL