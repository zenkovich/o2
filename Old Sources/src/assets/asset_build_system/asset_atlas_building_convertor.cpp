#include "asset_atlas_building_convertor.h"

#include "app/application.h"
#include "asset_build_system.h"
#include "util/file_system/file_system.h"
#include "util/image/bitmap.h"

OPEN_O2_NAMESPACE

AtlasAssetBuildingConvertor::AtlasAssetBuildingConvertor():
AssetBuildingConvertor()
{
}

AtlasAssetBuildingConvertor::AtlasAssetBuildingConvertor(AssetBuildSystem* buildSystem):
AssetBuildingConvertor(buildSystem)
{
}

AtlasAssetBuildingConvertor::~AtlasAssetBuildingConvertor()
{
}

void AtlasAssetBuildingConvertor::Convert(AssetBuildInfo* asset)
{
	AtlasAssetBuildInfo* atlas = static_cast<AtlasAssetBuildInfo*>(asset);
	String buildedPath = mBuildSystem->GetBuildedAssetsFolderPath() + ExtractExtension(asset->mLocation.mPath);

	mImageDefs.Clear();
	mRectsPacker.Clear();

	foreach(atlas->mImages, imgIt)
	{
		Bitmap* image = mnew Bitmap(mBuildSystem->GetAssetsFolderPath() + ExtractExtension((*imgIt)->mLocation.mPath));
		RectsPacker::Rect* packRect = mRectsPacker.AddRect(image->GetSize());

		mImageDefs.Add(ImageDef(*imgIt, image, packRect));
	}

	mRectsPacker.SetMaxSize(atlas->mMaxSize);
	mRectsPacker.Pack();

	Bitmap resAtlasImage(Bitmap::Format::R8G8B8A8, atlas->mMaxSize);

	foreach(mImageDefs, imgDefIt)
	{
		resAtlasImage.CopyImage(imgDefIt->mImage, imgDefIt->mPackRect->mRect.getltCorner());
		SafeRelease(imgDefIt->mImage);
	}

	resAtlasImage.Save(buildedPath + ".png", Bitmap::ImageType::Png);

	Serializer serializer;
	serializer.Serialize(mImageDefs, "images");
	serializer.Save(buildedPath);
}

void AtlasAssetBuildingConvertor::Remove(AssetBuildInfo* asset)
{
	HightLog("Removing atlas: %s", asset->mLocation.mPath.c_str());
	String buildedPath = mBuildSystem->GetBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath;

	AppFileSystem()->DeleteFile(buildedPath + ".xml");
	AppFileSystem()->DeleteFile(ExtractExtension(buildedPath) + ".png");
}

UniqueType AtlasAssetBuildingConvertor::GetConvertingType() const
{
	return AtlasAssetBuildInfo::GetStaticType();
}


AtlasAssetBuildingConvertor::ImageDef::ImageDef(ImageAssetBuildInfo* imageAssetInfo /*= NULL*/, Bitmap* image /*= NULL*/,
												RectsPacker::Rect* packRect /*= NULL*/):
												mImageAssetInfo(imageAssetInfo), mImage(image), mPackRect(packRect)
{
}

bool AtlasAssetBuildingConvertor::ImageDef::operator==(const ImageDef& other)
{
	return mImage == other.mImage && mPackRect == other.mPackRect && mImageAssetInfo == other.mImageAssetInfo;
}

SERIALIZE_METHOD_IMPL(AtlasAssetBuildingConvertor::ImageDef)
{
	SERIALIZE_ID(&mImageAssetInfo->mLocation, "location");
	SERIALIZE_ID(mPackRect->mRect, "rect");

	bool linkedDirectly = !mImageAssetInfo->mAtlasName.empty();
	SERIALIZE_ID(linkedDirectly, "linkedDirectly");

	return true;
}

CLOSE_O2_NAMESPACE