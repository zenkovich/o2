#include "FolderAssetConverter.h"

#include "Assets/BinaryAsset.h"
#include "Assets/ImageAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	IOBJECT_CPP(FolderAssetConverter);

	Vector<Type::Id> FolderAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(FolderAssetConverter::type.ID());
		return res;
	}

	void FolderAssetConverter::ConvertAsset(const String& path)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + path;
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderCreate(buildedAssetPath);
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void FolderAssetConverter::RemoveAsset(const String& path)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderRemove(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void FolderAssetConverter::MoveAsset(const String& pathFrom, const String& pathTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + pathFrom;
		String fullPathTo = o2Assets.GetDataPath() + pathFrom;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FolderCreate(fullPathTo);
		o2FileSystem.FolderRemove(fullPathFrom);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}
}