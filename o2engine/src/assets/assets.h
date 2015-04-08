#pragma once

#include "public.h"
#include "asset.h"
#include "config/asset_config.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class Asset;
class LogStream;
class AssetBuildSystem;

class Assets
{
	friend class Asset;
	friend class AssetBuildSystem;
	friend class AtlasAsset;
	friend class FileAsset;
	friend class ImageAsset;
	friend class ImageAssetBuildInfo;
	friend class XmlAsset;

public:
	typedef Array<Asset*> AssetsArr;

protected:
	AssetsInfosArr    mAssetsInfos;
	AssetsConfigsArr  mAssetsConfigs;
	LogStream*        mLog;
	AssetBuildSystem* mBuildSystem;
	String            mBuildedAssetsPath;

public:
	Assets();
	~Assets();

	String GetAssetRealPath(const FileLocation& location);
	String GetAssetRealPath(const String& path);

	String GetAssetFullPath(const FileLocation& location);
	String GetAssetFullPath(const String& path);

	FileLocation GetAssetFileLocation(const String& path);

	String GetAssetSourceFullPath(const String& path);

	template<typename _asType>
	_asType LoadAsset(const String& path);

	template<typename _asType>
	_asType LoadAsset(const FileLocation& location);

	template<typename _asType>
	_asType CreateAsset();

	template<typename _asType>
	void SaveAsset(_asType& asset, const String& path);

	template<typename _asType>
	void RemoveAsset(_asType& asset);

	void RebuildAssets(bool forcible = false);

protected:
	void LoadBuildedAssetsInfo();
	uint GenerateFileId() const;
	void ReloadAssetsConfigs();
	void SaveAssetsConfigs();
};

template<typename _asType>
_asType Assets::CreateAsset()
{
	return _asType();
}

template<typename _asType>
void Assets::RemoveAsset(_asType& asset)
{
	AppFileSystem()->DeleteFile(mBuildedAssetsPath + "/" + asset.GetPath());
	RebuildAssets();
}

template<typename _asType>
_asType Assets::LoadAsset(const String& path)
{
	_asType res(path);
	return res;
}

template<typename _asType>
void Assets::SaveAsset(_asType& asset, const String& path)
{
	asset.Save(path);
}

CLOSE_O2_NAMESPACE
