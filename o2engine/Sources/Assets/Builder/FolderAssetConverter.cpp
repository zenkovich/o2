#include "FolderAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/BinaryAsset.h"
#include "Assets/FolderAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<Type::Id> FolderAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(FolderAsset::type->ID());
		return res;
	}

	void FolderAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.mPath;
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderCreate(buildedAssetPath);
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void FolderAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderRemove(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void FolderAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + nodeFrom.mPath;
		String fullPathTo = o2Assets.GetDataPath() + nodeTo.mPath;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FolderCreate(fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);

		mRemovedFolders.Add(fullPathFrom);
		mRemovedFolders.Remove(fullPathTo);
	}

	void FolderAssetConverter::AssetsPostProcess()
	{
		for (auto fold : mRemovedFolders)
			o2FileSystem.FolderRemove(fold);
	}

	void FolderAssetConverter::Reset()
	{
		mRemovedFolders.Clear();
	}
}