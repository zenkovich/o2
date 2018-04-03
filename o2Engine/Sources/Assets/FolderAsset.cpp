#include "stdafx.h"
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
	}

	FolderAsset::FolderAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		Load();
	}

	FolderAsset::FolderAsset(UID id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	FolderAsset::FolderAsset(const FolderAsset& asset):
		Asset(asset), mContainingAssetsInfos(asset.mContainingAssetsInfos), meta(this), insideAssets(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
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

	FolderAssetRef FolderAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<FolderAsset>();
	}

}

DECLARE_CLASS(o2::FolderAsset);

DECLARE_CLASS(o2::FolderAssetRef);

DECLARE_CLASS(o2::FolderAsset::MetaInfo);
