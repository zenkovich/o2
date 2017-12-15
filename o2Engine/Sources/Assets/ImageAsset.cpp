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

	ImageAsset::ImageAsset(UID id):
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
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		if (asset.mBitmap)
			mBitmap = asset.mBitmap->Clone();
		else
			mBitmap = nullptr;

		mAtlasPage = asset.mAtlasPage;
		mAtlasRect = asset.mAtlasRect;

		InitializeProperties();
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
			GetAssetsLogStream()->Error("Can' setup image atlas id (%i): wrong id", id);
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

	void ImageAsset::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ImageAsset, bitmap, SetBitmap, GetBitmap);
		INITIALIZE_PROPERTY(ImageAsset, atlasId, SetAtlasId, GetAtlasId);
		INITIALIZE_PROPERTY(ImageAsset, atlas, SetAtlas, GetAtlas);
		INITIALIZE_PROPERTY(ImageAsset, sliceBorder, SetSliceBorder, GetSliceBorder);
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

CLASS_META(o2::ImageAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(bitmap);
	PUBLIC_FIELD(atlasId);
	PUBLIC_FIELD(atlas);
	PUBLIC_FIELD(sliceBorder);
	PUBLIC_FIELD(atlasPage);
	PUBLIC_FIELD(atlasRect);
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(width);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(meta);
	PROTECTED_FIELD(mBitmap);
	PROTECTED_FIELD(mAtlasPage);
	PROTECTED_FIELD(mAtlasRect);

	PUBLIC_FUNCTION(Bitmap*, GetBitmap);
	PUBLIC_FUNCTION(void, SetBitmap, Bitmap*);
	PUBLIC_FUNCTION(UID, GetAtlasId);
	PUBLIC_FUNCTION(void, SetAtlasId, UID);
	PUBLIC_FUNCTION(AtlasAssetRef, GetAtlas);
	PUBLIC_FUNCTION(void, SetAtlas, const AtlasAssetRef&);
	PUBLIC_FUNCTION(void, SetSliceBorder, const BorderI&);
	PUBLIC_FUNCTION(BorderI, GetSliceBorder);
	PUBLIC_FUNCTION(UInt, GetAtlasPage);
	PUBLIC_FUNCTION(RectI, GetAtlasRect);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(float, GetWidth);
	PUBLIC_FUNCTION(float, GetHeight);
	PUBLIC_FUNCTION(TextureRef, GetAtlasTextureRef);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, LoadBitmap);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::ImageAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::ImageAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mMaxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mScale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFormat).SERIALIZABLE_ATTRIBUTE();
}
END_META;

CLASS_META(o2::ImageAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);

	PUBLIC_FIELD(mAtlasId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mIOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAndroid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMacOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSliceBorder).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDefaultMode).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
}
END_META;
