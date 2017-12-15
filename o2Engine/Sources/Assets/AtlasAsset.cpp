#include "AtlasAsset.h"

#include "Assets/Assets.h"
#include "Assets/ImageAsset.h"

namespace o2
{
	const Type* AtlasAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(AtlasAsset);
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

	const Dictionary<UID, RectI>& AtlasAsset::Page::ImagesRects() const
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

	AtlasAsset::AtlasAsset(UID id):
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
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		mImagesAssetsInfos = asset.mImagesAssetsInfos;
		mPages = asset.mPages;
		for (auto& page : mPages)
			page.mOwner = this;

		InitializeProperties();
	}

	AtlasAsset::~AtlasAsset()
	{}

	AtlasAsset& AtlasAsset::operator=(const AtlasAsset& asset)
	{
		Asset::operator=(asset);

		mImagesAssetsInfos = asset.mImagesAssetsInfos;
		mPages = asset.mPages;

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool AtlasAsset::operator==(const AtlasAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool AtlasAsset::operator!=(const AtlasAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	AssetInfosVec AtlasAsset::GetImages() const
	{
		return mImagesAssetsInfos;
	}

	Vector<AssetRef> AtlasAsset::GetImagesAssets() const
	{
		Vector<AssetRef> res;
		for (auto inf : mImagesAssetsInfos)
			res.Add(ImageAssetRef(inf.id));

		return res;
	}

	AtlasAsset::PagesVec AtlasAsset::GetPages() const
	{
		return mPages;
	}

	bool AtlasAsset::ContainsImage(const ImageAssetRef& image)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.id == image->GetAssetId(); });
	}

	bool AtlasAsset::ContainsImage(const AssetInfo& imageAssetInfo)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.id == imageAssetInfo.id; });
	}

	bool AtlasAsset::ContainsImage(UID id)
	{
		return mImagesAssetsInfos.ContainsPred([&](const AssetInfo& info) { return info.id == id; });
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

	String AtlasAsset::GetPageTextureFileName(UID atlasId, UInt pageIdx)
	{
		return GetPageTextureFileName(o2Assets.GetAssetPath(atlasId), pageIdx);
	}

	String AtlasAsset::GetPageTextureFileName(const String& atlasPath, UInt pageIdx)
	{
		return o2Assets.GetDataPath() + atlasPath + (String)pageIdx + ".png";
	}

	TextureRef AtlasAsset::GetPageTextureRef(UID atlasId, UInt pageIdx)
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

		if (auto node = data.GetNode("mImagesAssetsInfos"))
			mImagesAssetsInfos = *node;

		if (auto node = data.GetNode("mPages"))
			mPages = *node;

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

	AtlasAssetRef AtlasAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<AtlasAsset>();
	}

}

CLASS_META(o2::AtlasAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(imagesInfos);
	PUBLIC_FIELD(images);
	PUBLIC_FIELD(pages);
	PROTECTED_FIELD(mImagesAssetsInfos);
	PROTECTED_FIELD(mPages);

	PUBLIC_FUNCTION(AssetInfosVec, GetImages);
	PUBLIC_FUNCTION(Vector<AssetRef>, GetImagesAssets);
	PUBLIC_FUNCTION(PagesVec, GetPages);
	PUBLIC_FUNCTION(bool, ContainsImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(bool, ContainsImage, const AssetInfo&);
	PUBLIC_FUNCTION(bool, ContainsImage, UID);
	PUBLIC_FUNCTION(bool, ContainsImage, const String&);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::AtlasAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::AtlasAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mMaxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFormat).SERIALIZABLE_ATTRIBUTE();
}
END_META;

CLASS_META(o2::AtlasAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);

	PUBLIC_FIELD(mIOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAndroid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMacOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBorder).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
}
END_META;

CLASS_META(o2::AtlasAsset::Page)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mImagesRects).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mOwner);

	typedef const Dictionary<UID, RectI>& _tmp1;

	PUBLIC_FUNCTION(UInt, ID);
	PUBLIC_FUNCTION(Vec2I, Size);
	PUBLIC_FUNCTION(TextureRef, GetTextureRef);
	PUBLIC_FUNCTION(String, GetTextureFileName);
	PUBLIC_FUNCTION(_tmp1, ImagesRects);
}
END_META;
