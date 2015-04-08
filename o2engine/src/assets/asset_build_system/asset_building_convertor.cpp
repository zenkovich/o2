#include "asset_building_convertor.h"

#include "app/application.h"
#include "asset_build_system.h"
#include "assets/assets.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

AssetBuildingConvertor::AssetBuildingConvertor():
mBuildSystem(NULL)
{
}

AssetBuildingConvertor::AssetBuildingConvertor(AssetBuildSystem* buildSystem):
mBuildSystem(buildSystem)
{
}

AssetBuildingConvertor::~AssetBuildingConvertor()
{
}

void AssetBuildingConvertor::Remove(AssetBuildInfo* asset)
{
	HightLog("Removing asset: %s", asset->mLocation.mPath.c_str());
	AppFileSystem()->DeleteFile(mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

CLOSE_O2_NAMESPACE