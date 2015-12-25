#include "image_asset.h"

#include "app/application.h"
#include "assets.h"
#include "util/image/bitmap.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(ImageAsset);

ImageAsset::ImageAsset():
Asset(), mImage(NULL)
{
}

ImageAsset::ImageAsset(const String& location):
Asset(location), mImage(NULL)
{
	loadData();
}

ImageAsset::ImageAsset(const FileLocation& location):
Asset(location), mImage(NULL)
{
	loadData();
}

ImageAsset::ImageAsset(Bitmap* image):
Asset(), mImage(image)
{
}

ImageAsset::~ImageAsset()
{
	SafeRelease(mImage);
}

Bitmap* ImageAsset::getImage()
{
	if (mImage ==  NULL)
		loadImage();

	return mImage;
}

void ImageAsset::setImage(Bitmap* image)
{
	SafeRelease(mImage);
	mImage = image;
}

ImageAsset& ImageAsset::operator=(const ImageAsset& asset)
{
	setImage(asset.mImage);
	return *this;
}

void ImageAsset::loadData()
{
	Serializer serializer;
	if (!serializer.Load(AppAssets()->GetAssetFullPath(mLocation.mPath) + ".atl_img", false))
	{
		AppAssets()->mLog->Error("Failed to load image asset: %s", mLocation.mPath.c_str());
		return;
	}

	serializer.Serialize(mAtlas, "atlas");
}

void ImageAsset::loadImage()
{
	SafeRelease(mImage);
	mImage = mnew Bitmap(AppAssets()->GetAssetSourceFullPath(mLocation.mPath));
}

void ImageAsset::SaveData()
{
	if (mImage)
		mImage->Save(AppAssets()->GetAssetSourceFullPath(mLocation.mPath), Bitmap::Png);

	AssetConfig* config = AppAssets()->mAssetsConfigs.getAssetConfig(mLocation);
	if (!config)
	{
		config = mnew ImageAssetConfig(mLocation, mAtlas);
		AppAssets()->mAssetsConfigs.addAssetConfig(config);
	}

	//config->
}

void ImageAsset::SetAtlasName(const String& atlasName)
{
	mAtlas = atlasName;
}

SERIALIZE_METHOD_IMPL(ImageAsset)
{
	SERIALIZE_ID(mAtlas, "atlas")

		return true;
}

CLOSE_O2_NAMESPACE