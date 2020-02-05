#include "o2/stdafx.h"
#include "ImageAsset.h"

#include "o2/Assets/Types/AtlasAsset.h"
#include "o2/Assets/Assets.h"
#include "o2/Utils/Bitmap/Bitmap.h"
#include "o2/Utils/Debug/Log/LogStream.h"

//#undef LoadBitmap

namespace o2
{
	ImageAsset::ImageAsset():
		Asset(mnew Meta())
	{}

	ImageAsset::ImageAsset(const ImageAsset& other):
		Asset(other), mAtlasPage(other.mAtlasPage), mAtlasRect(other.mAtlasRect), bitmap(this), atlas(this), sliceBorder(this),
		atlasPage(this), atlasRect(this), size(this), width(this), height(this), meta(this)
	{
		if (other.mBitmap)
			mBitmap = other.mBitmap->Clone();
		else
			mBitmap = nullptr;

		mAtlasPage = other.mAtlasPage;
		mAtlasRect = other.mAtlasRect;
	}

	ImageAsset::~ImageAsset()
	{
		if (mBitmap)
			delete mBitmap;
	}

	ImageAsset& ImageAsset::operator=(const ImageAsset& asset)
	{
		Asset::operator=(asset);

		if (mBitmap)
			delete mBitmap;

		if (asset.mBitmap)
			SetBitmap(asset.mBitmap);

		mAtlasPage = asset.mAtlasPage;
		mAtlasRect = asset.mAtlasRect;

		return *this;
	}

	Bitmap* ImageAsset::GetBitmap()
	{
		if (!mBitmap)
			LoadBitmap();

		return mBitmap;
	}

	void ImageAsset::SetBitmap(Bitmap* bitmap)
	{
		if (mBitmap)
			delete mBitmap;

		mBitmap = bitmap;
	}

	UID ImageAsset::GetAtlas() const
	{
		return GetMeta()->atlasId;
	}

	void ImageAsset::SetAtlas(const UID& atlas)
	{
		ImageAssetRef thisRef(GetUID());
		if (auto prevAtlas = AtlasAssetRef(GetMeta()->atlasId)) 
			prevAtlas->mImages.Remove(thisRef);

		GetMeta()->atlasId = atlas;

		if (auto newAtlas = AtlasAssetRef(GetMeta()->atlasId))
			newAtlas->mImages.Add(thisRef);
	}

	void ImageAsset::SetSliceBorder(const BorderI& border)
	{
		GetMeta()->sliceBorder = border;
	}

	BorderI ImageAsset::GetSliceBorder() const
	{
		return GetMeta()->sliceBorder;
	}

	UInt ImageAsset::GetAtlasPage() const
	{
		return mAtlasPage;
	}

	RectI ImageAsset::GetAtlasRect() const
	{
		return mAtlasRect;
	}

	Vec2F ImageAsset::GetSize() const
	{
		return mAtlasRect.Size();
	}

	float ImageAsset::GetWidth() const
	{
		return (float)mAtlasRect.Width();
	}

	float ImageAsset::GetHeight() const
	{
		return (float)mAtlasRect.Height();
	}

	TextureRef ImageAsset::GetAtlasTextureRef() const
	{
		return AtlasAsset::GetPageTextureRef(GetAtlas(), GetAtlasPage());
	}

	ImageAsset::Meta* ImageAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	const char* ImageAsset::GetFileExtensions() const
	{
		return "png jpg bmp";
	}

	void ImageAsset::SaveData(const String& path) const
	{
		if (mBitmap)
			mBitmap->Save(GetFullPath(), Bitmap::ImageType::Png);
	}

	void ImageAsset::LoadBitmap()
	{
		String assetFullPath = GetFullPath();
		mBitmap->Load(assetFullPath);
	}

	bool ImageAsset::PlatformMeta::operator==(const PlatformMeta& other) const
	{
		return maxSize == other.maxSize && format == other.format && scale == other.scale;
	}

	bool ImageAsset::Meta::IsEqual(AssetMeta* other) const
	{
		if (!AssetMeta::IsEqual(other))
			return false;

		Meta* otherMeta = (Meta*)other;
		return atlasId == otherMeta->atlasId && ios == otherMeta->ios && windows == otherMeta->windows &&
			android == otherMeta->android && macOS == otherMeta->macOS && sliceBorder == otherMeta->sliceBorder &&
			defaultMode == otherMeta->defaultMode;
	}
}

DECLARE_CLASS_MANUAL(o2::DefaultAssetMeta<o2::ImageAsset>);

DECLARE_CLASS(o2::ImageAsset);

DECLARE_CLASS(o2::ImageAsset::PlatformMeta);

DECLARE_CLASS(o2::ImageAsset::Meta);
