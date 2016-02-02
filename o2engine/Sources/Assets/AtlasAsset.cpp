#include "AtlasAsset.h"

#include "Assets/Assets.h"
#include "Assets/ImageAsset.h"

namespace o2
{
	Type::Id AtlasAsset::MetaInfo::GetAssetType() const
	{
		return AtlasAsset::type.ID();
	}

	bool AtlasAsset::MetaInfo::IsEqual(IMetaInfo* other) const
	{
		if (!IMetaInfo::IsEqual(other))
			return false;

		MetaInfo* otherMeta = (MetaInfo*)other;
		return mIOS == otherMeta->mIOS && mAndroid == otherMeta->mAndroid && mMacOS == otherMeta->mMacOS &&
			mWindows == otherMeta->mWindows && Math::Equals(mBorder, otherMeta->mBorder);
	}

	UInt AtlasAsset::Page::ID() const
	{
		return mId;
	}

	Vec2I AtlasAsset::Page::Size() const
	{
		return mSize;
	}

	TextureRef AtlasAsset::Page::GetTextureRef() const
	{
		return AtlasAsset::GetPageTextureRef(mOwner->GetAssetId(), mId);
	}

	String AtlasAsset::Page::GetTextureFileName() const
	{
		return AtlasAsset::GetPageTextureFileName(mOwner->GetAssetId(), mId);
	}

	const Dictionary<AssetId, RectI>& AtlasAsset::Page::ImagesRects() const
	{
		return mImagesRects;
	}

	bool AtlasAsset::Page::operator==(const Page& other) const
	{
		return mId == other.mId;
	}

	AtlasAsset::AtlasAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	AtlasAsset::AtlasAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	AtlasAsset::AtlasAsset(AssetId id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	AtlasAsset::AtlasAsset(const AtlasAsset& asset):
		Asset(asset)
	{
		mImagesAssetsInfos = asset.mImagesAssetsInfos;
		mPages = asset.mPages;
		for (auto& page : mPages)
			page.mOwner = this;

		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	AtlasAsset::~AtlasAsset()
	{}

	AtlasAsset& AtlasAsset::operator=(const AtlasAsset& asset)
	{
		return *this;
	}

	AssetInfosVec AtlasAsset::GetImages() const
	{
		return mImagesAssetsInfos;
	}

	Vector<ImageAsset*> AtlasAsset::GetImagesAssets() const
	{
		Vector<ImageAsset*> res;
		for (auto inf : mImagesAssetsInfos)
			res.Add(mnew ImageAsset(inf.mId));

		return res;
	}

	AtlasAsset::PagesVec AtlasAsset::GetPages() const
	{
		return mPages;
	}

	bool AtlasAsset::ContainsImage(ImageAsset* image)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.mId == image->GetAssetId(); });
	}

	bool AtlasAsset::ContainsImage(const AssetInfo& imageAssetInfo)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.mId == imageAssetInfo.mId; });
	}

	bool AtlasAsset::ContainsImage(AssetId id)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.mId == id; });
	}

	bool AtlasAsset::ContainsImage(const String& path)
	{
		return ContainsImage(o2Assets.GetAssetId(path));
	}

	AtlasAsset::MetaInfo* AtlasAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	const char* AtlasAsset::GetFileExtensions() const
	{
		return "atlas";
	}

	String AtlasAsset::GetPageTextureFileName(AssetId atlasId, UInt pageIdx)
	{
		return GetPageTextureFileName(o2Assets.GetAssetPath(atlasId), pageIdx);
	}

	String AtlasAsset::GetPageTextureFileName(const String& atlasPath, UInt pageIdx)
	{
		return o2Assets.GetDataPath() + atlasPath + (String)pageIdx + ".png";
	}

	TextureRef AtlasAsset::GetPageTextureRef(AssetId atlasId, UInt pageIdx)
	{
		return TextureRef(GetPageTextureFileName(atlasId, pageIdx));
	}

	TextureRef AtlasAsset::GetPageTextureRef(const String& atlasPath, UInt pageIdx)
	{
		return TextureRef(GetPageTextureFileName(atlasPath, pageIdx));
	}

	void AtlasAsset::LoadData(const String& path)
	{
		DataNode data;
		data.LoadFromFile(path);
		Deserialize(data);

		for (auto& page : mPages)
			page.mOwner = this;
	}

	void AtlasAsset::SaveData(const String& path)
	{
		DataNode data;
		data.SaveToFile(path);
	}

	void AtlasAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(AtlasAsset, meta, GetMeta);
		INITIALIZE_GETTER(AtlasAsset, imagesInfos, GetImages);
		INITIALIZE_GETTER(AtlasAsset, images, GetImagesAssets);
		INITIALIZE_GETTER(AtlasAsset, pages, GetPages);
	}

	bool AtlasAsset::PlatformMeta::operator==(const PlatformMeta& other) const
	{
		return mMaxSize == other.mMaxSize && mFormat == other.mFormat;
	}

}