#pragma once

#include "config/asset_config.h"
#include "util/file_system/file_info.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class AssetBuildInfo: public Serializable
{
public:
	DEFINE_TYPE(AssetBuildInfo);

	FileLocation mLocation;
	WideTime     mWriteTime;

public:
	virtual void InitFromConfigs(AssetConfig* config) {}

	virtual AssetConfig* GetConfigsSample() const;

	virtual bool IsEquals(AssetBuildInfo* other);

	virtual void CopyFrom(const AssetBuildInfo* other);

	virtual AssetBuildInfo* Clone() const;

	SERIALIZBLE_METHODS(AssetBuildInfo);

};
typedef Array<AssetBuildInfo*> AssetsBuildInfosArr;

class AtlasAssetBuildInfo;
class FolderAssetBuildInfo;

class ImageAssetBuildInfo: public AssetBuildInfo
{
	friend class FolderAssetBuildInfo;

	AtlasAssetBuildInfo* mAtlas;

public:
	DEFINE_TYPE(ImageAssetBuildInfo);

	String mAtlasName;
	float  mScale;

public:
	ImageAssetBuildInfo();

	virtual void InitFromConfigs(AssetConfig* config);

	virtual AssetConfig* GetConfigsSample() const;

	virtual bool IsEquals(AssetBuildInfo* other);

	virtual void CopyFrom(const AssetBuildInfo* other);

	virtual AssetBuildInfo* Clone() const;

	SERIALIZBLE_INHERITED_METHODS(ImageAssetBuildInfo, AssetBuildInfo);
};
typedef Array<ImageAssetBuildInfo*> ImageAssetsBuildInfosArr;

class AtlasAssetBuildInfo: public AssetBuildInfo
{
	friend class FolderAssetBuildInfo;
	friend class AssetBuildSystem;
	friend class AtlasAssetBuildingConvertor;

	ImageAssetsBuildInfosArr mImages;
	bool                     mAttachedToFolder;
	FileLocation             mAttachFolderLocation;
	FolderAssetBuildInfo*    mAttachFolder;

public:
	DEFINE_TYPE(AtlasAssetBuildInfo);

	String mName;
	Vec2F  mMaxSize;

public:
	AtlasAssetBuildInfo();

	virtual void InitFromConfigs(AssetConfig* config);

	virtual AssetConfig* GetConfigsSample() const;

	virtual bool IsEquals(AssetBuildInfo* other);

	virtual void CopyFrom(const AssetBuildInfo* other);

	virtual AssetBuildInfo* Clone() const;

	SERIALIZBLE_INHERITED_METHODS(AtlasAssetBuildInfo, AssetBuildInfo);
};
typedef Array<AtlasAssetBuildInfo*> AtlasAssetsBuildInfosArr;

class FolderAssetBuildInfo: public AssetBuildInfo
{
	friend class AssetBuildSystem;

public:
	DEFINE_TYPE(FolderAssetBuildInfo);

	FolderAssetBuildInfo* mParentFolder;
	AssetsBuildInfosArr   mInsideAssets;
	AtlasAssetBuildInfo*  mAttachedAtlas;

public:
	FolderAssetBuildInfo();
	~FolderAssetBuildInfo();

	AssetsBuildInfosArr GetAllInsideAssets() const;

	AssetBuildInfo* GetInsideAsset(const FileLocation& location, bool recursive = false);

	void AddInsideAsset(AssetBuildInfo* asset);

	void Clear();

	virtual AssetConfig* GetConfigsSample() const;

	virtual AssetBuildInfo* Clone() const;

	SERIALIZBLE_INHERITED_METHODS(FolderAssetBuildInfo, AssetBuildInfo);

private:
	void LinkAtlases();
	void LinkImages();
	void LinkChildFolders(FolderAssetBuildInfo* parentFolder);
};

CLOSE_O2_NAMESPACE
