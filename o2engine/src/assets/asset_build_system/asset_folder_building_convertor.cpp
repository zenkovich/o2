#include "asset_folder_building_convertor.h"

#include "app/application.h"
#include "asset_build_system.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

FolderAssetBuildingConvertor::FolderAssetBuildingConvertor():
AssetBuildingConvertor()
{
}

FolderAssetBuildingConvertor::FolderAssetBuildingConvertor(AssetBuildSystem* buildSystem):
AssetBuildingConvertor(buildSystem)
{
}

FolderAssetBuildingConvertor::~FolderAssetBuildingConvertor()
{
}

void FolderAssetBuildingConvertor::Convert(AssetBuildInfo* asset)
{
	HightLog("Converting folder: %s", asset->mLocation.mPath.c_str());
	AppFileSystem()->CreateDirectory(mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

UniqueType FolderAssetBuildingConvertor::GetConvertingType() const
{
	return FolderAssetBuildInfo::GetStaticType();
}

void FolderAssetBuildingConvertor::Remove(AssetBuildInfo* asset)
{
	HightLog("Removing folder: %s", asset->mLocation.mPath.c_str());
	AppFileSystem()->RemoveDirectory(mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

CLOSE_O2_NAMESPACE