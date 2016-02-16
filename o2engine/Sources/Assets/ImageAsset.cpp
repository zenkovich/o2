#include "ImageAsset.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Utils/Bitmap.h"
#include "Utils/Log/LogStream.h"

//#undef LoadBitmap

namespace o2
{
	ImageAsset::ImageAsset():
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	ImageAsset::ImageAsset(const String& path):
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	ImageAsset::ImageAsset(AssetId id):
		Asset(), mBitmap(nullptr), mAtlasPage(0)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	ImageAsset::ImageAsset(const ImageAsset& asset):
		Asset(asset)
	{
		if (asset.mBitmap)
			mBitmap = asset.mBitmap->Clone();
		else
			mBitmap = nullptr;

		mAtlasPage = asset.mAtlasPage;
		mAtlasRect = asset.mAtlasRect;

		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	ImageAsset::~ImageAsset()
	{
		if (mBitmap)
			delete mBitmap;
	}

	ImageAsset& ImageAsset::operator=(ImageAsset& asset)
	{
		SetBitmap(asset.GetBitmap());
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

	AssetId ImageAsset::GetAtlasId() const
	{
		return ((MetaInfo*)mMeta)->mAtlasId;
	}

	void ImageAsset::SetAtlasId(AssetId id)
	{
		AssetInfo atlasInfp = o2Assets.GetAssetInfo(id);
		if (atlasInfp.mType != AtlasAsset::type.ID())
		{
			GetAssetsLogStream()->Error("Can' setup image atlas id (%i): wrong id", id);
			return;
		}

		((MetaInfo*)mMeta)->mAtlasId = id;
	}

	AtlasAsset* ImageAsset::GetAtlas() const
	{
		return mnew AtlasAsset(((MetaInfo*)mMeta)->mAtlasId);
	}

	void ImageAsset::SetAtlas(AtlasAsset* atlas)
	{
		((MetaInfo*)mMeta)->mAtlasId = atlas->GetAssetId();
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
		Deserialize(data);
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

	void ImageAsset::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ImageAsset, bitmap, SetBitmap, GetBitmap);
		INITIALIZE_PROPERTY(ImageAsset, atlasId, SetAtlasId, GetAtlasId);
		INITIALIZE_PROPERTY(ImageAsset, atlas, SetAtlas, GetAtlas);
		INITIALIZE_GETTER(ImageAsset, atlasPage, GetAtlasPage);
		INITIALIZE_GETTER(ImageAsset, atlasRect, GetAtlasRect);
		INITIALIZE_GETTER(ImageAsset, meta, GetMeta);
		INITIALIZE_GETTER(ImageAsset, size, GetSize);
		INITIALIZE_GETTER(ImageAsset, width, GetWidth);
		INITIALIZE_GETTER(ImageAsset, height, GetHeight);
	}

	bool ImageAsset::PlatformMeta::operator==(const PlatformMeta& other) const
	{
		return mMaxSize == other.mMaxSize && mFormat == other.mFormat && mScale == other.mScale;
	}

	Type::Id ImageAsset::MetaInfo::GetAssetType() const
	{
		return ImageAsset::type.ID();
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
}