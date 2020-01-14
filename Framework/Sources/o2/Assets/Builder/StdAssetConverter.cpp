#include "o2/stdafx.h"
#include "StdAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/BinaryAsset.h"
#include "o2/Assets/Builder/AssetsBuilder.h"
#include "o2/Assets/ImageAsset.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<const Type*> StdAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(BinaryAsset));
		return res;
	}

	void StdAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = mAssetsBuilder->GetSourceAssetsPath() + node.path;
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileCopy(sourceAssetPath, buildedAssetPath);
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
		o2FileSystem.SetFileEditDate(buildedAssetPath, node.time);
	}

	void StdAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void StdAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = mAssetsBuilder->GetBuiltAssetsPath() + nodeFrom.path;
		String fullPathTo = mAssetsBuilder->GetBuiltAssetsPath() + nodeTo.path;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}

DECLARE_CLASS(o2::StdAssetConverter);
