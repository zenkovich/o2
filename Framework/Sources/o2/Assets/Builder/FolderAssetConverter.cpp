#include "o2/stdafx.h"
#include "FolderAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/BinaryAsset.h"
#include "o2/Assets/FolderAsset.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "AssetsBuilder.h"

namespace o2
{
	Vector<const Type*> FolderAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(FolderAsset));
		return res;
	}

	void FolderAssetConverter::ConvertAsset(const AssetsTree::AssetInfo& node)
	{
		String sourceAssetPath = mAssetsBuilder->GetSourceAssetsPath() + node.mPath;
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.mPath;

		o2FileSystem.FolderCreate(buildedAssetPath);
	}

	void FolderAssetConverter::RemoveAsset(const AssetsTree::AssetInfo& node)
	{
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.mPath;

		o2FileSystem.FolderRemove(buildedAssetPath);
	}

	void FolderAssetConverter::MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo)
	{
		String fullPathFrom = mAssetsBuilder->GetBuiltAssetsPath() + nodeFrom.mPath;
		String fullPathTo = mAssetsBuilder->GetBuiltAssetsPath() + nodeTo.mPath;

		o2FileSystem.FolderCreate(fullPathTo);

		mRemovedFolders.Add(fullPathFrom);
		mRemovedFolders.Remove(fullPathTo);
	}

	Vector<UID> FolderAssetConverter::AssetsPostProcess()
	{
		for (auto fold : mRemovedFolders)
			o2FileSystem.FolderRemove(fold);

		return Vector<UID>();
	}

	void FolderAssetConverter::Reset()
	{
		mRemovedFolders.Clear();
	}
}

DECLARE_CLASS(o2::FolderAssetConverter);
