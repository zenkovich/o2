#include "o2/stdafx.h"
#include "ImageAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/ImageAsset.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<const Type*> ImageAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(ImageAsset));
		return res;
	}

	void ImageAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.path;
		String buildedAssetPath = o2Assets.GetDataPath() + node.path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.WriteFile(buildedAssetPath, "");
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
		o2FileSystem.SetFileEditDate(buildedAssetPath, node.time);
	}

	void ImageAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + node.path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void ImageAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + nodeFrom.path;
		String fullPathTo = o2Assets.GetDataPath() + nodeTo.path;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}

DECLARE_CLASS(o2::ImageAssetConverter);
