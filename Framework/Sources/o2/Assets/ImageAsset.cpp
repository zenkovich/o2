#include "o2/stdafx.h"
#include "ImageAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/AtlasAsset.h"
#include "o2/Utils/Bitmap/Bitmap.h"
#include "o2/Utils/Debug/Log/LogStream.h"

//#undef LoadBitmap

namespace o2
{
	ImageAsset::ImageAsset():
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mMeta = mnew MetaInfo();
	}

	ImageAsset::ImageAsset(const String& path):
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		Load();
	}

	ImageAsset::ImageAsset(UID id):
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	ImageAsset::ImageAsset(const ImageAsset& asset):
		Asset(asset), bitmap(this), atlasId(this), atlas(this), sliceBorder(this), atlasPage(this),
		atlasRect(this), size(this), width(this), height(this), meta(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		if (asset.mBitmap)
			mBitmap = asset.mBitmap->Clone();
		else
			mBitmap = nullptr;

		mAtlasPage = asset.mAtlasPage;
		mAtlasRect = asset.mAtlasRect;
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

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool ImageAsset::operator==(const ImageAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool ImageAsset::operator!=(const ImageAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
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

	UID ImageAsset::GetAtlasId() const
	{
		return ((MetaInfo*)mMeta)->mAtlasId;
	}

	void ImageAsset::SetAtlasId(UID id)
	{
		AssetInfo atlasInfo = o2Assets.GetAssetInfo(id);
		if (atlasInfo.assetType != &TypeOf(AtlasAsset))
		{
			GetAssetsLogStream()->Error("Can' setup image atlas id (" + (String)id + "): wrong id");
			return;
		}

		((MetaInfo*)mMeta)->mAtlasId = id;
	}

	AtlasAssetRef ImageAsset::GetAtlas() const
	{
		return AtlasAssetRef(((MetaInfo*)mMeta)->mAtlasId);
	}

	void ImageAsset::SetAtlas(const AtlasAssetRef& atlas)
	{
		((MetaInfo*)mMeta)->mAtlasId = atlas->GetAssetId();
	}

	void ImageAsset::SetSliceBorder(const BorderI& border)
	{
		GetMeta()->mSliceBorder = border;
	}

	BorderI ImageAsset::GetSliceBorder() const
	{
		return GetMeta()->mSliceBorder;
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
		return AtlasAsset::GetPageTextureRef(GetAtlasId(), GetAtlasPage());
	}

	ImageAsset::MetaInfo* ImageAsset::GetMeta() const
	{
		return ((MetaInfo*)mMeta);
	}

	const char* ImageAsset::GetFileExtensions() const
	{
		return "png jpg bmp";
	}

	void ImageAsset::LoadData(const String& path)
	{
		DataNode data;
		data.LoadFromFile(path);

		if (auto node = data.GetNode("mAtlasPage"))
			mAtlasPage = *node;

		if (auto node = data.GetNode("mAtlasRect"))
			mAtlasRect = *node;
	}

	void ImageAsset::SaveData(const String& path)
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
		return mMaxSize == other.mMaxSize && mFormat == other.mFormat && mScale == other.mScale;
	}

	const Type* ImageAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(ImageAsset);
	}

	bool ImageAsset::MetaInfo::IsEqual(IMetaInfo* other) const
	{
		if (!IMetaInfo::IsEqual(other))
			return false;

		MetaInfo* otherMeta = (MetaInfo*)other;
		return mAtlasId == otherMeta->mAtlasId && mIOS == otherMeta->mIOS && mWindows == otherMeta->mWindows &&
			mAndroid == otherMeta->mAndroid && mMacOS == otherMeta->mMacOS && mSliceBorder == otherMeta->mSliceBorder &&
			mDefaultMode == otherMeta->mDefaultMode;
	}

	ImageAssetRef ImageAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<ImageAsset>();
	}

}

DECLARE_CLASS(o2::ImageAsset);

DECLARE_CLASS(o2::ImageAssetRef);

DECLARE_CLASS(o2::ImageAsset::PlatformMeta);

DECLARE_CLASS(o2::ImageAsset::MetaInfo);
