#include "asset_file_building_convertor.h"

#include "app/application.h"
#include "asset_build_system.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

FileAssetBuildingConvertor::FileAssetBuildingConvertor():
AssetBuildingConvertor()
{
}

FileAssetBuildingConvertor::FileAssetBuildingConvertor(AssetBuildSystem* buildSystem):
AssetBuildingConvertor(buildSystem)
{
}

FileAssetBuildingConvertor::~FileAssetBuildingConvertor()
{
}

void FileAssetBuildingConvertor::Convert(AssetBuildInfo* asset)
{
	HightLog("Converting asset: %s", asset->mLocation.mPath.c_str());

	AppFileSystem()->CopyFile(mBuildSystem->GetAssetsFolderPath() + "/" + asset->mLocation.mPath,
							  mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

UniqueType FileAssetBuildingConvertor::GetConvertingType() const
{
	return AssetBuildInfo::GetStaticType();
}

CLOSE_O2_NAMESPACE