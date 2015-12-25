#pragma once

#include "asset_building_convertor.h"
#include "util/rects_packer.h"

OPEN_O2_NAMESPACE

class ImageAssetBuildInfo;
class Bitmap;

class AtlasAssetBuildingConvertor: public AssetBuildingConvertor
{
public:
	struct ImageDef: public Serializable
	{
		ImageAssetBuildInfo* mImageAssetInfo;
		Bitmap*              mImage;
		RectsPacker::Rect*   mPackRect;

		ImageDef(ImageAssetBuildInfo* imageAssetInfo = NULL, Bitmap* image = NULL, RectsPacker::Rect* packRect = NULL);

		bool operator==(const ImageDef& other);

		SERIALIZBLE_METHODS(ImageDef);
	};

private:
	typedef Array<ImageAssetBuildInfo*> ImageAssetsInfosArr;
	typedef Array<ImageDef>             ImageDefsArr;

	ImageDefsArr mImageDefs;
	RectsPacker  mRectsPacker;

public:
	AtlasAssetBuildingConvertor();
	AtlasAssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~AtlasAssetBuildingConvertor();

	virtual void Convert(AssetBuildInfo* asset);
	virtual void Remove(AssetBuildInfo* asset);
	virtual UniqueType GetConvertingType() const;
};

CLOSE_O2_NAMESPACE
