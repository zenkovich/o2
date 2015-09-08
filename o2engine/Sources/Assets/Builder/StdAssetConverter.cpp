#include "StdAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/BinaryAsset.h"
#include "Assets/ImageAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	IOBJECT_CPP(StdAssetConverter);

	Vector<Type::Id> StdAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(BinaryAsset::type.ID());
		return res;
	}

	void StdAssetConverter::ConvertAsset(const String& path)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + path;
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileCopy(sourceAssetPath, buildedAssetPath);
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void StdAssetConverter::RemoveAsset(const String& path)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void StdAssetConverter::MoveAsset(const String& pathFrom, const String& pathTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + pathFrom;
		String fullPathTo = o2Assets.GetDataPath() + pathTo;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}