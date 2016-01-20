#include "Render/Texture.h"

#include "Render/Render.h"
#include "Utils/Bitmap.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	Texture::Texture():
		mReady(false)
	{
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(const Vec2I& size, Format format /*= Format::Default*/, Usage usage /*= Usage::Default*/)
	{
		Create(size, format, usage);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(const String& fileName)
	{
		Create(fileName);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::Texture(Bitmap* bitmap)
	{
		Create(bitmap);
		o2Render.mTextures.Add(this);
		InitializeProperties();
	}

	Texture::~Texture()
	{
		if (!mReady)
			return;

		glDeleteTextures(1, &mHandle);

		if (mUsage == Usage::RenderTarget)
			glDeleteBuffers(1, &mFrameBuffer);

		o2Render.mTextures.Remove(this);
	}

	void Texture::Create(const Vec2I& size, Format format /*= Format::Default*/, Usage usage /*= Usage::Default*/)
	{
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

				o2Render.mLog->Error("Failed to create GL frame buffer object! GL Error %x %s", glError,
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
	}

	void Texture::Create(Bitmap* bitmap)
	{
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

	void Texture::InitializeProperties()
	{
		INITIALIZE_GETTER(Texture, size, GetSize);
		INITIALIZE_GETTER(Texture, format, GetFormat);
		INITIALIZE_GETTER(Texture, usage, GetUsage);
		INITIALIZE_GETTER(Texture, fileName, GetFileName);
	}
}