#include "o2/stdafx.h"
#include "FolderAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	FolderAsset::FolderAsset():
	{}

	FolderAsset::FolderAsset(const FolderAsset& asset):
		TAsset(asset), meta(this), insideAssets(this)
	{}

	FolderAsset& FolderAsset::operator=(const FolderAsset& asset)
	{
		Asset::operator=(asset);
		return *this;
	}

	Vector<AssetRef> FolderAsset::GetContainingAssetsInfos() const
	{
		Vector<AssetRef> res;
		for (auto asset : mInfo.children)
			res.Add(AssetRef(asset->meta->ID()));

		return res;
	}

	FolderAsset::Meta* FolderAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	void FolderAsset::LoadData(const String& path)
	{}

	void FolderAsset::SaveData(const String& path) const
	{
		if (!o2FileSystem.IsFolderExist(path))
			o2FileSystem.FolderCreate(path);
	}
}

DECLARE_CLASS(o2::FolderAsset);

DECLARE_CLASS(o2::FolderAsset::Meta);
