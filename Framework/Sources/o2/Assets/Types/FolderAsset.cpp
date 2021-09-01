#include "o2/stdafx.h"
#include "FolderAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	FolderAsset::FolderAsset()
	{}

	FolderAsset::FolderAsset(const FolderAsset& asset):
		AssetWithDefaultMeta<FolderAsset>(asset)
	{}

	FolderAsset& FolderAsset::operator=(const FolderAsset& asset)
	{
		Asset::operator=(asset);
		return *this;
	}

	Vector<AssetRef> FolderAsset::GetChildrenAssets() const
	{
		Vector<AssetRef> res;
		for (auto asset : mInfo.GetChildren())
			res.Add(AssetRef(asset->meta->ID()));

		return res;
	}

	void FolderAsset::LoadData(const String& path)
	{}

	void FolderAsset::SaveData(const String& path) const
	{
		if (!o2FileSystem.IsFolderExist(path))
			o2FileSystem.FolderCreate(path);
	}
}

DECLARE_TEMPLATE_CLASS(o2::AssetWithDefaultMeta<o2::FolderAsset>);
DECLARE_TEMPLATE_CLASS(o2::DefaultAssetMeta<o2::FolderAsset>);
DECLARE_TEMPLATE_CLASS(o2::Ref<o2::FolderAsset>);

DECLARE_CLASS(o2::FolderAsset);
