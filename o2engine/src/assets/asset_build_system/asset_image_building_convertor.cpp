#include "asset_image_building_convertor.h"

#include "app/application.h"
#include "asset_build_system.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

ImageAssetBuildingConvertor::ImageAssetBuildingConvertor():
AssetBuildingConvertor()
{
}

ImageAssetBuildingConvertor::ImageAssetBuildingConvertor(AssetBuildSystem* buildSystem):
AssetBuildingConvertor(buildSystem)
{
}

ImageAssetBuildingConvertor::~ImageAssetBuildingConvertor()
{
}

void ImageAssetBuildingConvertor::Convert(AssetBuildInfo* asset)
{
	AppFileSystem()->CopyFile(mBuildSystem->GetAssetsFolderPath() + "/" + asset->mLocation.mPath,
							  mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);

	HightLog("Converting image: %s", asset->mLocation.mPath.c_str());

	ImageAssetBuildInfo* imgAsset = static_cast<ImageAssetBuildInfo*>(asset);

	Serializer serializer;
	serializer.Serialize(imgAsset->mAtlasName, "atlas");

	String fullPath = mBuildSystem->GetBuildedAssetsFolderPath() + "/" + ExtractExtension(asset->mLocation.mPath) + ".atl_img";
	serializer.Save(fullPath, false);
}

UniqueType ImageAssetBuildingConvertor::GetConvertingType() const
{
	return ImageAssetBuildInfo::GetStaticType();
}

CLOSE_O2_NAMESPACE