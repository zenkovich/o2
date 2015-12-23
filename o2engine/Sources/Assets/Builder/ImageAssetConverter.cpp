#include "ImageAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/ImageAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<Type::Id> ImageAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(ImageAsset::type->ID());
		return res;
	}

	void ImageAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.mPath;
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.WriteFile(buildedAssetPath, "");
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
		o2FileSystem.SetFileEditDate(buildedAssetPath, node.mTime);
	}

	void ImageAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void ImageAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + nodeFrom.mPath;
		String fullPathTo = o2Assets.GetDataPath() + nodeTo.mPath;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}