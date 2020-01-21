#include "o2/stdafx.h"
#include "ImageAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Builder/AssetsBuilder.h"
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

	void ImageAssetConverter::ConvertAsset(const AssetsTree::AssetNode& node)
	{
		String sourceAssetPath = mAssetsBuilder->GetSourceAssetsPath() + node.path;
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;

		o2FileSystem.WriteFile(buildedAssetPath, "");
		o2FileSystem.SetFileEditDate(buildedAssetPath, node.time);
	}

	void ImageAssetConverter::RemoveAsset(const AssetsTree::AssetNode& node)
	{
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;

		o2FileSystem.FileDelete(buildedAssetPath);
	}

	void ImageAssetConverter::MoveAsset(const AssetsTree::AssetNode& nodeFrom, const AssetsTree::AssetNode& nodeTo)
	{
		String fullPathFrom = mAssetsBuilder->GetBuiltAssetsPath() + nodeFrom.path;
		String fullPathTo = mAssetsBuilder->GetBuiltAssetsPath() + nodeTo.path;

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
	}
}

DECLARE_CLASS(o2::ImageAssetConverter);