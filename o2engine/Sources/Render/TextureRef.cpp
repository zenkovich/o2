#include "TextureRef.h"

#include "Render/Render.h"

namespace o2
{
	TextureRef::TextureRef():
		mTexture(nullptr)
	{
	}

	TextureRef::TextureRef(const Vec2I& size, Texture::Format format /*= Texture::Format::Default*/,
						   Texture::Usage usage /*= Texture::Usage::Default*/)
	{
		mTexture = mnew Texture(size, format, usage);
		mTexture->mRefs.Add(this);
	}

	TextureRef::TextureRef(const String& fileName)
	{
		mTexture = o2Render.mTextures.FindMatch([&](const Ptr<Texture>& tex) { return tex->GetFileName() == fileName; });

		if (!mTexture)
			mTexture = mnew Texture(fileName);

		mTexture->mRefs.Add(this);
	}

	TextureRef::TextureRef(Ptr<Bitmap> bitmap)
	{
		mTexture = mnew Texture(bitmap);
		mTexture->mRefs.Add(this);
	}

	TextureRef::TextureRef(const TextureRef& other):
		mTexture(other.mTexture)
	{
		if (mTexture)
			mTexture->mRefs.Add(this);
	}

	TextureRef::TextureRef(Ptr<Texture> texture):
		mTexture(texture)
	{
		if (mTexture)
			mTexture->mRefs.Add(this);
	}

	TextureRef::~TextureRef()
	{
		if (mTexture)
			mTexture->mRefs.Remove(this);
	}

	TextureRef& TextureRef::operator=(const TextureRef& other)
	{
		if (mTexture)
			mTexture->mRefs.Remove(this);

		mTexture = other.mTexture;

		if (mTexture)
			mTexture->mRefs.Add(this);

		return *this;
	}

	Texture* TextureRef::operator->()
	{
		return mTexture;
	}

	const Texture* TextureRef::operator->() const
	{
		return mTexture;
	}

	bool TextureRef::IsValid() const
	{
		return mTexture.IsValid();
	}

	TextureRef::operator bool() const
	{
		return mTexture.IsValid();
	}

	TextureRef TextureRef::Null()
	{
		return TextureRef();
	}

	bool TextureRef::operator!=(const TextureRef& other) const
	{
		return mTexture != other.mTexture;
	}

	bool TextureRef::operator==(const TextureRef& other) const
	{
		return mTexture == other.mTexture;
	}

	TextureRef NoTexture()
	{
		return TextureRef();
	}

}