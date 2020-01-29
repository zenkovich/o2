#include "o2/stdafx.h"
#include "ImageAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Builder/AssetsBuilder.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<const Type*> ImageAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(ImageAsset));
		return res;
	}

	void ImageAssetConverter::ConvertAsset(const AssetInfo& node)
	{
		String sourceAssetPath = mAssetsBuilder->GetSourceAssetsPath() + node.path;
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;

		o2FileSystem.WriteFile(buildedAssetPath, "");
		o2FileSystem.SetFileEditDate(buildedAssetPath, node.editTime);
	}

	void ImageAssetConverter::RemoveAsset(const AssetInfo& node)
	{
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.path;

		o2FileSystem.FileDelete(buildedAssetPath);
	}

	void ImageAssetConverter::MoveAsset(const AssetInfo& nodeFrom, const AssetInfo& nodeTo)
	{
		String fullPathFrom = mAssetsBuilder->GetBuiltAssetsPath() + nodeFrom.path;
		String fullPathTo = mAssetsBuilder->GetBuiltAssetsPath() + nodeTo.path;

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
	}
}

DECLARE_CLASS(o2::ImageAssetConverter);
