#include "ImageAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/ImageAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	IOBJECT_CPP(ImageAssetConverter);

	Vector<Type::Id> ImageAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(ImageAsset::type.ID());
		return res;
	}

	void ImageAssetConverter::ConvertAsset(const String& path)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + path;
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.WriteFile(buildedAssetPath, "");
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void ImageAssetConverter::RemoveAsset(const String& path)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void ImageAssetConverter::MoveAsset(const String& pathFrom, const String& pathTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + pathFrom;
		String fullPathTo = o2Assets.GetDataPath() + pathTo;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}