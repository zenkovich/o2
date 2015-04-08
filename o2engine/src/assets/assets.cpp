#include "assets.h"

#include "asset.h"
#include "asset_build_system/asset_build_system.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

Assets::Assets()
{
	mLog = mnew LogStream("assets");
	gLog->BindStream(mLog);

	mBuildSystem = mnew AssetBuildSystem(this);

	if (ASSETS_PREBUILDING_ENABLE)
		RebuildAssets();
	else
		LoadBuildedAssetsInfo();
}

Assets::~Assets()
{
	SafeRelease(mLog);
	SafeRelease(mBuildSystem);
}

String Assets::GetAssetSourceFullPath(const String& path)
{
	return ASSETS_PATH + GetAssetRealPath(path);
}

String Assets::GetAssetFullPath(const String& path)
{
	return ASSETS_BUILDED_PATH + GetAssetRealPath(path);
}

String Assets::GetAssetFullPath(const FileLocation& location)
{
	return ASSETS_BUILDED_PATH + GetAssetRealPath(location);
}


String Assets::GetAssetRealPath(const String& path)
{
	String convertedPath = path;

	if (convertedPath == "")
		return convertedPath;

	uint fileId = 0;
	if (convertedPath[0] == '@')
	{
		int idEndIdx = convertedPath.find('@', 1);
		String fileIdStr = convertedPath.substr(1, idEndIdx - 1);
		fileId = ToInt(fileIdStr);

		foreach(mAssetsInfos, fileLocIt)
		{
			if (fileLocIt->mLocation.mId == fileId)
			{
				convertedPath.replace(convertedPath.begin(), convertedPath.begin() + idEndIdx + 1,
									  fileLocIt->mLocation.mPath.begin(), fileLocIt->mLocation.mPath.end());

				break;
			}
		}
	}

	return convertedPath;
}

String Assets::GetAssetRealPath(const FileLocation& location)
{
	foreach(mAssetsInfos, fileLocIt)
	if (fileLocIt->mLocation == location)
		return fileLocIt->mLocation.mPath;

	return location.mPath;
}


FileLocation Assets::GetAssetFileLocation(const String& path)
{
	String convertedPath = GetAssetRealPath(path);

	foreach(mAssetsInfos, fileIt)
	if (fileIt->mLocation.mPath == convertedPath)
		return fileIt->mLocation;

	return FileLocation(path);
}

uint Assets::GenerateFileId() const
{
	return rand()%(UINT_MAX - 1) + 1;
}

void Assets::RebuildAssets(bool forcible /*= false*/)
{
	mBuildSystem->RebuildAssets(forcible);
	LoadBuildedAssetsInfo();
}

void Assets::LoadBuildedAssetsInfo()
{
	mAssetsInfos.Clear();

	Serializer serializer;
	if (serializer.Load(ASSETS_INFO_FILE_PATH))
	{
		serializer.Serialize(mAssetsInfos, "assets");
	}

	ReloadAssetsConfigs();
}

void Assets::ReloadAssetsConfigs()
{
	ReleaseArray(mAssetsConfigs);

	Serializer serializer;
	if (serializer.Load(ASSETS_FOLDER_CONFIG_FILE_PATH))
		serializer.Serialize(mAssetsConfigs, "assetsConfigs");
}

void Assets::SaveAssetsConfigs()
{
	Serializer serializer;
	serializer.Serialize(mAssetsConfigs, "assetsConfigs");
	serializer.Save(ASSETS_FOLDER_CONFIG_FILE_PATH);
}

CLOSE_O2_NAMESPACE