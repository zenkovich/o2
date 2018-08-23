#include "stdafx.h"
#include "Render/Texture.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Render/Render.h"
#include "Utils/Bitmap/Bitmap.h"
#include "Utils/Debug/Log/LogStream.h"

namespace o2
{
	Texture::Texture() :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(const Vec2I& size, Format format /*= Format::Default*/, Usage usage /*= Usage::Default*/) :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(size, format, usage);
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(const String& fileName) :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(fileName);
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(Bitmap* bitmap) :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(bitmap);
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(UID atlasAssetId, int page) :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(atlasAssetId, page);
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(const String& atlasAssetName, int page) :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		Create(atlasAssetName, page);
		o2Render.mTextures.Add(this);
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
}

ENUM_META_(o2::Texture::Format, Format)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(R8G8B8);
	ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;

ENUM_META_(o2::Texture::Usage, Usage)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(RenderTarget);
}
END_ENUM_META;
