#include "FolderAsset.h"

#include "Assets/Assets.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	Type::Id FolderAsset::MetaInfo::GetAssetType() const
	{
		return FolderAsset::type.ID();
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

	FolderAsset::FolderAsset(AssetId id)
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
		InitializeProperties();
	}

	FolderAsset::~FolderAsset()
	{}

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

	FolderAsset& FolderAsset::operator=(const FolderAsset& asset)
	{
		mContainingAssetsInfos = asset.mContainingAssetsInfos;
		return *this;
	}
}