#include "o2/stdafx.h"
#include "AtlasAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/ImageAsset.h"

namespace o2
{
	const Type* AtlasAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(AtlasAsset);
	}

	bool AtlasAsset::MetaInfo::IsEqual(AssetMeta* other) const
	{
		if (!AssetMeta::IsEqual(other))
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
		return AtlasAsset::GetPageTextureRef(mOwner->GetUID(), mId);
	}

	String AtlasAsset::Page::GetTextureFileName() const
	{
		return AtlasAsset::GetPageTextureFileName(mOwner->GetUID(), mId);
	}

	const Map<UID, RectI>& AtlasAsset::Page::ImagesRects() const
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
	}

	AtlasAsset::AtlasAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		Load();
	}

	AtlasAsset::AtlasAsset(const UID& id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	AtlasAsset::AtlasAsset(const AtlasAsset& asset):
		Asset(asset), meta(this), imagesInfos(this), images(this), pages(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetUID();

		mImages = asset.mImages;
		mPages = asset.mPages;
		for (auto& page : mPages)
			page.mOwner = this;
	}

	AtlasAsset::~AtlasAsset()
	{}

	AtlasAsset& AtlasAsset::operator=(const AtlasAsset& asset)
	{
		Asset::operator=(asset);

		mImages = asset.mImages;
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

	Vector<AssetInfo> AtlasAsset::GetImages() const
	{
		return mImages;
	}

	Vector<AssetRef> AtlasAsset::GetImagesAssets() const
	{
		Vector<AssetRef> res;
		for (auto inf : mImages)
			res.Add(ImageAssetRef(inf.id));

		return res;
	}

	Vector<AtlasAsset::Page> AtlasAsset::GetPages() const
	{
		return mPages;
	}

	bool AtlasAsset::ContainsImage(const ImageAssetRef& image)
	{
		return mImages.ContainsPred([&](const AssetInfo& info) { return info.id == image->GetUID(); });
	}

	bool AtlasAsset::ContainsImage(const AssetInfo& imageAssetInfo)
	{
		return mImages.ContainsPred([&](const AssetInfo& info) { return info.id == imageAssetInfo.id; });
	}

	bool AtlasAsset::ContainsImage(const UID& id)
	{
		return mImages.ContainsPred([&](const AssetInfo& info) { return info.id == id; });
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

	String AtlasAsset::GetPageTextureFileName(const UID& atlasId, UInt pageIdx)
	{
		return GetPageTextureFileName(o2Assets.GetAssetPath(atlasId), pageIdx);
	}

	String AtlasAsset::GetPageTextureFileName(const String& atlasPath, UInt pageIdx)
	{
		return o2Assets.GetBuiltAssetsPath() + atlasPath + (String)pageIdx + ".png";
	}

	TextureRef AtlasAsset::GetPageTextureRef(const UID& atlasId, UInt pageIdx)
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

		if (auto node = data.GetNode("images"))
			mImages = *node;

		if (auto node = data.GetNode("pages"))
			mPages = *node;

		for (auto& page : mPages)
			page.mOwner = this;
	}

	void AtlasAsset::SaveData(const String& path)
	{
		DataNode data;
		data.SaveToFile(path);
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

DECLARE_CLASS(o2::AtlasAsset);

DECLARE_CLASS(o2::AtlasAssetRef);

DECLARE_CLASS(o2::AtlasAsset::PlatformMeta);

DECLARE_CLASS(o2::AtlasAsset::MetaInfo);

DECLARE_CLASS(o2::AtlasAsset::Page);
