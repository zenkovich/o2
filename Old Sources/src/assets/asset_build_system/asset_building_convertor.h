#pragma once

#include "public.h"
#include "asset_file_info.h"
#include "util/file_system/file_info.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class AssetBuildSystem;

class AssetBuildingConvertor
{
protected:
	AssetBuildSystem* mBuildSystem;

public:
	AssetBuildingConvertor();
	AssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~AssetBuildingConvertor();

	virtual void Convert(AssetBuildInfo* asset) = 0;
	virtual void Remove(AssetBuildInfo* asset);
	virtual UniqueType GetConvertingType() const = 0;
};

CLOSE_O2_NAMESPACE
