#include "stdafx.h"
#include "Render/Texture.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Render/Render.h"
#include "Utils/Bitmap.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	Texture::Texture():
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(const Vec2I& size, Format format /*= Format::Default*/, Usage usage /*= Usage::Default*/):
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(size, format, usage);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(const String& fileName):
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(fileName);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(Bitmap* bitmap):
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(bitmap);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(UID atlasAssetId, int page):
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(atlasAssetId, page);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(const String& atlasAssetName, int page):
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(atlasAssetName, page);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::~Texture()
	{
		o2Render.mTextures.Remove(this);

		if (!mReady)
			return;

		if (mUsage == Usage::RenderTarget)
			glDeleteFramebuffersEXT(1, &mFrameBuffer);

		glDeleteTextures(1, &mHandle);
	}

	void Texture::Create(const Vec2I& size, Format format /*= Format::Default*/, Usage usage /*= Usage::Default*/)
	{
		if (mReady)
		{
			if (mUsage == Usage::RenderTarget)
				glDeleteFramebuffersEXT(1, &mFrameBuffer);

			glDeleteTextures(1, &mHandle);
		}

		mFormat = format;
		mUsage = usage;
		mSize = size;

		glGenTextures(1, &mHandle);
		glBindTexture(GL_TEXTURE_2D, mHandle);

		GLint texFormat = GL_RGB;
		if (format == Format::R8G8B8A8)
			texFormat = GL_RGBA;
		else if (format == Format::R8G8B8)
			texFormat = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, (GLsizei)size.x, (GLsizei)size.y, 0, texFormat, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (mUsage == Usage::RenderTarget)
		{
			glGenFramebuffersEXT(1, &mFrameBuffer);
			glBindFramebufferEXT(GL_FRAMEBUFFER, mFrameBuffer);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mHandle, 0);

			GLenum DrawBuffers[2] ={GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1, DrawBuffers);

			if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				GLenum glError = glGetError();

				o2Render.mLog->Error("Failed to create GL frame buffer object! GL Error %i %cs", glError,
									 GetGLErrorDesc(glError));

				mReady = false;
				return;
			}

			mReady = true;

			glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		}

		mReady = true;
	}

	void Texture::Create(const String& fileName)
	{
		Bitmap* image = mnew Bitmap();
		if (image->Load(fileName, Bitmap::ImageType::Auto))
		{
			mFileName = fileName;
			Create(image);
		}

		delete image;
		mReady = true;
	}

	void Texture::Create(Bitmap* bitmap)
	{
		if (mReady)
		{
			if (mUsage == Usage::RenderTarget)
				glDeleteFramebuffersEXT(1, &mFrameBuffer);

			glDeleteTextures(1, &mHandle);
		}

		Bitmap::Format imageFormat = bitmap->GetFormat();

		if (imageFormat == Bitmap::Format::Default)
			mFormat = Format::Default;
		else if (imageFormat == Bitmap::Format::R8G8B8A8)
			mFormat = Format::R8G8B8A8;

		mUsage = Usage::Default;
		mSize = bitmap->GetSize();
		mFileName = bitmap->GetFilename();

		glGenTextures(1, &mHandle);
		glBindTexture(GL_TEXTURE_2D, mHandle);

		GLint texFormat = GL_RGB;
		if (mFormat == Format::R8G8B8A8)
			texFormat = GL_RGBA;
		else if (mFormat == Format::R8G8B8)
			texFormat = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, bitmap->GetSize().x, bitmap->GetSize().y, 0, texFormat, GL_UNSIGNED_BYTE,
					 bitmap->GetData());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		mReady = true;
	}

	void Texture::SetData(Bitmap* bitmap)
	{
		glBindTexture(GL_TEXTURE_2D, mHandle);

		GLint texFormat = GL_RGB;
		if (mFormat == Format::R8G8B8A8)
			texFormat = GL_RGBA;
		else if (mFormat == Format::R8G8B8)
			texFormat = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, bitmap->GetSize().x, bitmap->GetSize().y, 0, texFormat, GL_UNSIGNED_BYTE,
					 bitmap->GetData());

		GL_CHECK_ERROR(o2Render.mLog);
	}

	void Texture::Create(UID atlasAssetId, int page)
	{
		if (o2Assets.IsAssetExist(atlasAssetId))
		{
			mAtlasAssetId = atlasAssetId;
			mAtlasPage = page;
			String textureFileName = AtlasAsset::GetPageTextureFileName(atlasAssetId, page);
			Create(textureFileName);

			mReady = true;
		}
		else o2Render.mLog->Error("Failed to load atlas texture with id %i and page %i", atlasAssetId, page);
	}

	void Texture::Create(const String& atlasAssetName, int page)
	{
		if (o2Assets.IsAssetExist(atlasAssetName))
		{
			mAtlasAssetId = o2Assets.GetAssetId(atlasAssetName);
			mAtlasPage = page;
			String textureFileName = AtlasAsset::GetPageTextureFileName(atlasAssetName, page);
			Create(textureFileName);

			mReady = true;
		}
		else o2Render.mLog->Error("Failed to load atlas texture with %s and page %i", atlasAssetName, page);
	}

	void Texture::Reload()
	{
		if (!mFileName.IsEmpty())
			Create(mFileName);
	}

	Vec2I Texture::GetSize() const
	{
		return mSize;
	}

	Texture::Format Texture::GetFormat() const
	{
		return mFormat;
	}

	Texture::Usage Texture::GetUsage() const
	{
		return mUsage;
	}

	String Texture::GetFileName() const
	{
		return mFileName;
	}

	bool Texture::IsReady() const
	{
		return mReady;
	}

	bool Texture::IsAtlasPage() const
	{
		return mAtlasAssetId != 0;
	}

	UID Texture::GetAtlasAssetId() const
	{
		return mAtlasAssetId;
	}

	int Texture::GetAtlasPage() const
	{
		return mAtlasPage;
	}

	void Texture::InitializeProperties()
	{
		INITIALIZE_GETTER(Texture, size, GetSize);
		INITIALIZE_GETTER(Texture, format, GetFormat);
		INITIALIZE_GETTER(Texture, usage, GetUsage);
		INITIALIZE_GETTER(Texture, fileName, GetFileName);
	}
}