#pragma once

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE

class ImageAssetBuildingConvertor: public AssetBuildingConvertor
{
public:
	ImageAssetBuildingConvertor();
	ImageAssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~ImageAssetBuildingConvertor();

	virtual void Convert(AssetBuildInfo* asset);
	virtual UniqueType GetConvertingType() const;
};

CLOSE_O2_NAMESPACE
