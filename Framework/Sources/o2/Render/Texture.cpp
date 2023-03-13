#include "o2/stdafx.h"
#include "o2/Render/Texture.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/AtlasAsset.h"
#include "o2/Render/Render.h"
#include "o2/Utils/Bitmap/Bitmap.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	Texture::Texture() :
		mReady(false), mAtlasAssetId(0), mAtlasPage(-1)
	{
		o2Render.mTextures.Add(this);
	}

	Texture::Texture(const Vec2I& size, PixelFormat format /*= Format::R8G8B8A8*/, Usage usage /*= Usage::Default*/) :
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
		auto& info = o2Assets.GetAssetInfo(atlasAssetId);
		if (info.IsValid())
		{
			mAtlasAssetId = atlasAssetId;
			mAtlasPage = page;
			String textureFileName = AtlasAsset::GetPageTextureFileName(info, page);
			Create(textureFileName);

			mReady = true;
		}
		else o2Render.mLog->Error("Failed to load atlas texture with id " + (String)atlasAssetId + " and page " + (String)page);
	}

	void Texture::Create(const String& atlasAssetName, int page)
	{
		auto& info = o2Assets.GetAssetInfo(atlasAssetName);
		if (info.IsValid())
		{
			mAtlasAssetId = o2Assets.GetAssetId(atlasAssetName);
			mAtlasPage = page;
			String textureFileName = AtlasAsset::GetPageTextureFileName(info, page);
			Create(textureFileName);

			mReady = true;
		}
		else 
			o2Render.mLog->Error("Failed to load atlas texture with " + atlasAssetName + " and page " + (String)page);
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

	PixelFormat Texture::GetFormat() const
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

ENUM_META(o2::Texture::Usage)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(RenderTarget);
}
END_ENUM_META;

ENUM_META(o2::Texture::Filter)
{
	ENUM_ENTRY(Linear);
	ENUM_ENTRY(Nearest);
}
END_ENUM_META;
