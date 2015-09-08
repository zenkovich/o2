#include "FolderAsset.h"

#include "Assets/Assets.h"

namespace o2
{
	IOBJECT_CPP(FolderAsset);
	IOBJECT_CPP(FolderAsset::MetaInfo);

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

	FolderAsset::FolderAsset(UInt id)
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

	Ptr<FolderAsset::MetaInfo> FolderAsset::GetMeta() const
	{
		return mMeta.Cast<MetaInfo>();
	}

	void FolderAsset::LoadData(const String& path)
	{
		auto folderAssetInfo = o2Assets.mAssetsTree.FindAsset(mPath);
		if (folderAssetInfo)
			mContainingAssetsInfos = folderAssetInfo->GetChilds().Select<AssetInfo>(
				[&](Ptr<AssetTree::AssetNode> asset) { return (AssetInfo)(*asset); });
	}

	void FolderAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(FolderAsset, Meta, GetMeta);
		INITIALIZE_GETTER(FolderAsset, InsideAssets, GetContainingAssetsInfos);
	}

	FolderAsset& FolderAsset::operator=(const FolderAsset& asset)
	{
		mContainingAssetsInfos = asset.mContainingAssetsInfos;
		return *this;
	}
}