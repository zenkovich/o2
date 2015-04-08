#pragma once

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE

class FolderAssetBuildingConvertor: public AssetBuildingConvertor
{
public:
	FolderAssetBuildingConvertor();
	FolderAssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~FolderAssetBuildingConvertor();

	virtual void Convert(AssetBuildInfo* asset);
	virtual void Remove(AssetBuildInfo* asset);
	virtual UniqueType GetConvertingType() const;
};

CLOSE_O2_NAMESPACE
