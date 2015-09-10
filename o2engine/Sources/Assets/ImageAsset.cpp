#include "ImageAsset.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Utils/Bitmap.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	IOBJECT_CPP(ImageAsset);
	IOBJECT_CPP(ImageAsset::MetaInfo);
	IOBJECT_CPP(ImageAsset::PlatformMeta);

	Type::Id ImageAsset::MetaInfo::GetAssetType() const
	{
		return ImageAsset::type.ID();
	}

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
			mBitmap.Release();
	}

	ImageAsset& ImageAsset::operator=(ImageAsset& asset)
	{
		SetBitmap(asset.GetBitmap());
		return *this;
	}

	Ptr<Bitmap> ImageAsset::GetBitmap()
	{
		if (!mBitmap)
			LoadBitmap();

		return mBitmap;
	}

	void ImageAsset::SetBitmap(Ptr<Bitmap> bitmap)
	{
		if (mBitmap)
			mBitmap.Release();

		mBitmap = bitmap;
	}

	AssetId ImageAsset::GetAtlasId() const
	{
		return mMeta.Cast<MetaInfo>()->mAtlasId;
	}

	void ImageAsset::SetAtlasId(AssetId id)
	{
		AssetInfo atlasInfp = o2Assets.GetAssetInfo(id);
		if (atlasInfp.mType != AtlasAsset::type.ID())
		{
			GetAssetsLogStream()->Error("Can' setup image atlas id (%i): wrong id", id);
			return;
		}

		mMeta.Cast<MetaInfo>()->mAtlasId = id;
	}

	Ptr<AtlasAsset> ImageAsset::GetAtlas() const
	{
		return mnew AtlasAsset(mMeta.Cast<MetaInfo>()->mAtlasId);
	}

	void ImageAsset::SetAtlas(Ptr<AtlasAsset> atlas)
	{
		mMeta.Cast<MetaInfo>()->mAtlasId = atlas->GetAssetId();
	}

	UInt ImageAsset::GetAtlasPage() const
	{
		return mAtlasPage;
	}

	RectI ImageAsset::GetAtlasRect() const
	{
		return mAtlasRect;
	}

	Ptr<ImageAsset::MetaInfo> ImageAsset::GetMeta() const
	{
		return mMeta.Cast<MetaInfo>();
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
		INITIALIZE_PROPERTY(ImageAsset, Data, SetBitmap, GetBitmap);
		INITIALIZE_PROPERTY(ImageAsset, AtlasId, SetAtlasId, GetAtlasId);
		INITIALIZE_PROPERTY(ImageAsset, Atlas, SetAtlas, GetAtlas);
		INITIALIZE_GETTER(ImageAsset, AtlasPage, GetAtlasPage);
		INITIALIZE_GETTER(ImageAsset, AtlasRect, GetAtlasRect);
		INITIALIZE_GETTER(ImageAsset, Meta, GetMeta);
	}

}