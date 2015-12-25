#pragma once

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE

class FileAssetBuildingConvertor: public AssetBuildingConvertor
{
public:
	FileAssetBuildingConvertor();
	FileAssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~FileAssetBuildingConvertor();

	virtual void Convert(AssetBuildInfo* asset);
	virtual UniqueType GetConvertingType() const;
};

CLOSE_O2_NAMESPACE
