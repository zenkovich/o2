#pragma once

#include "public.h"
#include "asset_file_info.h"
#include "config/asset_config.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class Assets;
class AssetBuildingConvertor;
class AssetConfig;
class AssetBuildInfo;

class AssetBuildSystem
{
	friend class AssetBuildingConvertor;
	friend class Assets;
	friend class AtlasAssetBuildingConvertor;
	friend class FileAssetBuildingConvertor;
	friend class FolderAssetBuildingConvertor;
	friend class ImageAssetBuildingConvertor;

	typedef Array<AssetBuildingConvertor*> AssetConvertersArr;

protected:
	Assets*              mAssets;
	AssetConvertersArr   mAssetConverters;
	FolderAssetBuildInfo mAssetsFolderInfo;
	FolderAssetBuildInfo mBuildedAssetsFolderInfo;
	String               mAssetsFolderPath;
	String               mBuildedAssetsFolderPath;
	String               mAssetsFolderConfigFilePath;
	String               mBuildedAssetsInfoFilePath;

protected:
	AssetBuildSystem(Assets* assets);
	~AssetBuildSystem();

	void RebuildAssets(bool forcible = false);

	String GetAssetsFolderPath() const;
	String GetBuildedAssetsFolderPath() const;

protected:
	void RemoveAllBuildedAssets();

	void LoadAssetFolderInfo();
	void LoadBuildedAssetsFolderInfo();

	void CheckBasicAtlas();
	void CheckMovedFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo);
	void CheckRemovedFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo);
	void CheckNewFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo);
	void ConvertFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo);
	void CopyAndConvertAsset(AssetBuildInfo* assetInfo, AssetBuildInfo* buildAssetInfo);
	void RemoveConvertedAsset(AssetBuildInfo* buildAssetInfo);
	void SaveBuildInfo();
	void SaveAssetsInfo();

	void ProcessLoadingAssetsFolderInfo(PathInfo& pathInfo, AssetsConfigsArr& configs, FolderAssetBuildInfo& asPathInfo);
	AssetBuildInfo* CreateAssetInfroFromFileInfo(const FileInfo& fileInfo);

	uint TryGetAssetsInfoMetaId(PathInfo &pathInfo, const String& path);
	uint GenerateFileId() const;
};

CLOSE_O2_NAMESPACE
