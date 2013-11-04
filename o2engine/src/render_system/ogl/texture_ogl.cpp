#include "public.h"
#ifdef RENDER_OGL

#include "texture_ogl.h"

#include "util/image/image.h"
#include "../render_system.h"

OPEN_O2_NAMESPACE

grTexture::grTexture( grRenderSystem* renderSystem, const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
                      grTexUsage::type usage /*= grTexUsage::DEFAULT*/ ):
	grTextureBaseInterface(renderSystem), mHandle(0)
{
	create(renderSystem, size, format, usage);
}

grTexture::grTexture( grRenderSystem* renderSystem, const std::string& fileName ):
	grTextureBaseInterface(renderSystem)
{
	createFromFile(renderSystem, fileName);
}

grTexture::~grTexture()
{	
	glDeleteTextures(1, &mHandle);
}

void grTexture::create( grRenderSystem* renderSystem, const vec2f& size, 
	                    grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
						grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	mFormat = format;
	mUsage = usage;
	mSize = size;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (format == grTexFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (format == grTexFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, (GLsizei)size.x, (GLsizei)size.y, 0, texFormat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	mReady = true;
}

void grTexture::createFromImage( grRenderSystem* renderSystem, cImage* image )
{
	mRenderSystem = renderSystem;

	cImage::Format imageFormat = image->getFormat();
	if (imageFormat == cImage::FMT_NONE)
		mFormat = grTexFormat::DEFAULT;
	else if (imageFormat == cImage::FMT_R8G8B8A8)
		mFormat = grTexFormat::R8G8B8A8;

	mUsage = grTexUsage::DEFAULT;
	mSize = image->getSize().castTo<float>();
	mFileName = image->getFilename();

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (mFormat == grTexFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (mFormat == grTexFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, image->getSize().x, image->getSize().y, 0, texFormat, GL_UNSIGNED_BYTE, 
		         image->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	mReady = true;
}

void grTexture::createFromFile( grRenderSystem* renderSystem, const std::string& fileName )
{
	cImage* image = new cImage;
	if (image->load(fileName, cImage::IT_AUTO, renderSystem->mLog))
		createFromImage(renderSystem, image);
}

void grTexture::createAsRenderTarget( grRenderSystem* renderSystem, const vec2f& size, 
	                                  grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	create(renderSystem, size, format, grTexUsage::RENDER_TARGET);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL