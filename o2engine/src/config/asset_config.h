#pragma once

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/serialize_util.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class AssetConfig: public Serializable
{
public:
	DEFINE_TYPE(AssetConfig);

	FileLocation mLocation;
	bool         mIncludeBuild;

public:
	AssetConfig();
	AssetConfig(const FileLocation& location, bool includeBuild = true);
	virtual ~AssetConfig();

	virtual AssetConfig* Clone() const;

	SERIALIZBLE_METHODS(AssetConfig);
};
typedef Array<AssetConfig*> AssetsConfigsArr;


class ImageAssetConfig: public AssetConfig
{
public:
	DEFINE_TYPE(ImageAssetConfig);

	float  mScale;
	String mAtlas;

public:
	ImageAssetConfig();
	ImageAssetConfig(const FileLocation& location, const String& atlas = "", float scale = 1.0f, bool includeBuild = true);

	virtual AssetConfig* Clone() const;

	SERIALIZBLE_INHERITED_METHODS(ImageAssetConfig, AssetConfig);
};


class AtlasAssetConfig: public AssetConfig
{
public:
	DEFINE_TYPE(AtlasAssetConfig);

	Vec2F        mMaxSize;
	String       mName;
	bool         mAttachedToFolder;
	FileLocation mAttachFolderLocation;

public:
	AtlasAssetConfig();
	AtlasAssetConfig(const FileLocation& location, const String& name, const Vec2F maxSize = Vec2F(2048, 2048),
					 bool includeBuild = true);

	virtual AssetConfig* Clone() const;

	SERIALIZBLE_INHERITED_METHODS(AtlasAssetConfig, AssetConfig);
};

CLOSE_O2_NAMESPACE
