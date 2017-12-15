#include "FolderAsset.h"

#include "Assets/Assets.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	const Type* FolderAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(FolderAsset);
	}

	FolderAsset::FolderAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	FolderAsset::FolderAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		InitializeProperties();

		Load();
	}

	FolderAsset::FolderAsset(UID id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	FolderAsset::FolderAsset(const FolderAsset& asset):
		Asset(asset), mContainingAssetsInfos(asset.mContainingAssetsInfos)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
		InitializeProperties();
	}

	FolderAsset::~FolderAsset()
	{}

	FolderAsset& FolderAsset::operator=(const FolderAsset& asset)
	{
		Asset::operator=(asset);
		mContainingAssetsInfos = asset.mContainingAssetsInfos;
		*mMeta = *(MetaInfo*)(asset.mMeta);
		return *this;
	}

	bool FolderAsset::operator==(const FolderAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool FolderAsset::operator!=(const FolderAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	AssetInfosVec FolderAsset::GetContainingAssetsInfos() const
	{
		return mContainingAssetsInfos;
	}

	FolderAsset::MetaInfo* FolderAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	void FolderAsset::LoadData(const String& path)
	{
		auto folderAssetInfo = o2Assets.mAssetsTree.FindAsset(mPath);
		if (folderAssetInfo)
			mContainingAssetsInfos = folderAssetInfo->GetChilds().Select<AssetInfo>(
				[&](AssetTree::AssetNode* asset) { return (AssetInfo)(*asset); });
	}

	void FolderAsset::SaveData(const String& path)
	{
		if (!o2FileSystem.IsFolderExist(path))
			o2FileSystem.FolderCreate(path);
	}

	void FolderAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(FolderAsset, meta, GetMeta);
		INITIALIZE_GETTER(FolderAsset, insideAssets, GetContainingAssetsInfos);
	}

	FolderAssetRef FolderAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<FolderAsset>();
	}

}

CLASS_META(o2::FolderAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(insideAssets);
	PROTECTED_FIELD(mContainingAssetsInfos);

	PUBLIC_FUNCTION(AssetInfosVec, GetContainingAssetsInfos);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::FolderAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::FolderAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);


	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
