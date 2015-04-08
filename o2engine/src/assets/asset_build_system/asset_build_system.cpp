#include "asset_build_system.h"

#include "app/application.h"
#include "asset_atlas_building_convertor.h"
#include "asset_building_convertor.h"
#include "asset_file_building_convertor.h"
#include "asset_folder_building_convertor.h"
#include "asset_image_building_convertor.h"
#include "assets/assets.h"
#include "config/asset_config.h"
#include "config/project_config.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

AssetBuildSystem::AssetBuildSystem(Assets* assets):
mAssets(assets)
{
	mAssetsFolderPath = ASSETS_PATH;
	mBuildedAssetsFolderPath = ASSETS_BUILDED_PATH;
	mAssetsFolderConfigFilePath = ASSETS_FOLDER_CONFIG_FILE_PATH;
	mBuildedAssetsInfoFilePath = ASSETS_BUILDED_INFO_FILE_PATH;

	mAssetConverters.Add(mnew FileAssetBuildingConvertor(this));
	mAssetConverters.Add(mnew FolderAssetBuildingConvertor(this));
	mAssetConverters.Add(mnew ImageAssetBuildingConvertor(this));
	mAssetConverters.Add(mnew AtlasAssetBuildingConvertor(this));
}

AssetBuildSystem::~AssetBuildSystem()
{
	ReleaseArray(mAssetConverters);
}

void AssetBuildSystem::RebuildAssets(bool forcible /*= false*/)
{
	if (forcible)
		RemoveAllBuildedAssets();

	LoadAssetFolderInfo();
	LoadBuildedAssetsFolderInfo();

	CheckMovedFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	CheckRemovedFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	ConvertFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	CheckNewFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);

	SaveBuildInfo();
	SaveAssetsInfo();
}

void AssetBuildSystem::RemoveAllBuildedAssets()
{
	AppFileSystem()->RemoveDirectory(mBuildedAssetsFolderPath);
	AppFileSystem()->CreateDirectory(mBuildedAssetsFolderPath);
	AppFileSystem()->DeleteFile(AppFileSystem()->GetFilePathByExt(mBuildedAssetsInfoFilePath, FileType::Config));
}

void AssetBuildSystem::LoadAssetFolderInfo()
{
	mAssetsFolderInfo.Clear();

	CheckBasicAtlas();

	//load assets path info
	PathInfo assetsPathInfo = AppFileSystem()->GetPathInfo(mAssetsFolderPath);
	assetsPathInfo.ClampPathNames();

	//process loading assets infos
	AssetsConfigsArr oldAssetsConfigs = mAssets->mAssetsConfigs;
	mAssets->mAssetsConfigs.Clear();

	ProcessLoadingAssetsFolderInfo(assetsPathInfo, oldAssetsConfigs, mAssetsFolderInfo);

	ReleaseArray(oldAssetsConfigs);

	//make links between folders, atlases and images
	mAssetsFolderInfo.LinkAtlases();

	//save configs
	mAssets->SaveAssetsConfigs();
}

void AssetBuildSystem::ProcessLoadingAssetsFolderInfo(PathInfo& pathInfo, AssetsConfigsArr& configs, FolderAssetBuildInfo& asPathInfo)
{
	foreach(pathInfo.mFiles, fileInfIt)
	{
		//skip metas
		if (fileInfIt->mPath.rfind(".meta") != String::npos)
			continue;

		//create asset info by file info type
		AssetBuildInfo* asFileInfo = CreateAssetInfroFromFileInfo(*fileInfIt);

		//copy basic parameters for asset info
		asFileInfo->mLocation.mPath = fileInfIt->mPath;
		asFileInfo->mLocation.mId = TryGetAssetsInfoMetaId(pathInfo, fileInfIt->mPath);
		asFileInfo->mWriteTime = fileInfIt->mEditDate;

		//seek asset config and clone
		AssetConfig* asFileConfig = NULL;
		foreach(configs, cfgIt)
		{
			if ((*cfgIt)->mLocation == asFileInfo->mLocation)
			{
				asFileConfig = (*cfgIt)->Clone();
				break;
			}
		}

		//skip if asset not included 
		if (asFileConfig && !asFileConfig->mIncludeBuild)
		{
			SafeRelease(asFileInfo);
			mAssets->mAssetsConfigs.Add(asFileConfig);
			continue;
		}

		//copy configuration into asset info, if config was found. Else - create new config
		if (asFileConfig)
			asFileInfo->InitFromConfigs(asFileConfig);
		else
			asFileConfig = asFileInfo->GetConfigsSample();

		//add asset config at new assets configs array
		mAssets->mAssetsConfigs.Add(asFileConfig);

		//and add asset info in assets infos hierarchy
		asPathInfo.AddInsideAsset(asFileInfo);
	}

	foreach(pathInfo.mPaths, pathInfIt)
	{
		//create folder asset info
		FolderAssetBuildInfo* folderInfo = mnew FolderAssetBuildInfo();

		//copy location parameters for folder asset info
		folderInfo->mLocation.mPath = pathInfIt->mPath;
		folderInfo->mLocation.mId = TryGetAssetsInfoMetaId(pathInfo, pathInfIt->mPath);

		//seek asset config and remove from searched place//seek asset config and clone
		AssetConfig* asFldrConfig = NULL;
		foreach(configs, cfgIt)
		{
			if ((*cfgIt)->mLocation == folderInfo->mLocation)
			{
				asFldrConfig = (*cfgIt)->Clone();
				break;
			}
		}

		if (asFldrConfig && !asFldrConfig->mIncludeBuild)
		{
			SafeRelease(folderInfo);
			mAssets->mAssetsConfigs.Add(asFldrConfig);
			continue;
		}

		//create new config, if needs
		if (!asFldrConfig)
			asFldrConfig = folderInfo->GetConfigsSample();

		//add asset config at new assets configs array
		mAssets->mAssetsConfigs.Add(asFldrConfig);

		//and add asset info in assets infos hierarchy
		asPathInfo.AddInsideAsset(folderInfo);

		//process files inside that folder
		ProcessLoadingAssetsFolderInfo(*pathInfIt, configs, *folderInfo);
	}
}

void AssetBuildSystem::LoadBuildedAssetsFolderInfo()
{
	mBuildedAssetsFolderInfo.Clear();

	Serializer serializer;
	if (serializer.Load(mBuildedAssetsInfoFilePath))
		serializer.Serialize(&mBuildedAssetsFolderInfo, "assets");

	mBuildedAssetsFolderInfo.LinkAtlases();
}

void AssetBuildSystem::CheckBasicAtlas()
{
}

void AssetBuildSystem::CheckMovedFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo)
{
}

void AssetBuildSystem::CheckRemovedFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo)
{
	//remove files first
	foreach_rem(buildedAssetFolderInfo.mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == FolderAssetBuildInfo::GetStaticType())
		{
			++assetIt;
			continue;
		}

		AssetBuildInfo* assetInfo = assetFolderInfo.GetInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			//remove builded asset
			RemoveConvertedAsset(*assetIt);
			assetIt = buildedAssetFolderInfo.mInsideAssets.Remove(assetIt);
			continue;
		}

		++assetIt;
	}

	//after remove folders
	foreach_rem(buildedAssetFolderInfo.mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() != FolderAssetBuildInfo::GetStaticType())
		{
			++assetIt;
			continue;
		}

		AssetBuildInfo* assetInfo = assetFolderInfo.GetInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			//remove builded asset
			RemoveConvertedAsset(*assetIt);
			assetIt = buildedAssetFolderInfo.mInsideAssets.Remove(assetIt);
			continue;
		}
		else CheckRemovedFiles(*static_cast<FolderAssetBuildInfo*>(assetInfo), *static_cast<FolderAssetBuildInfo*>(*assetIt));

		++assetIt;
	}
}

void AssetBuildSystem::CheckNewFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo)
{
	//create folders first
	foreach(assetFolderInfo.mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() != FolderAssetBuildInfo::GetStaticType())
			continue;

		AssetBuildInfo* assetInfo = buildedAssetFolderInfo.GetInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			assetInfo = (*assetIt)->Clone();
			buildedAssetFolderInfo.AddInsideAsset(assetInfo);

			//copy and convert new asset
			CopyAndConvertAsset(*assetIt, assetInfo);
		}

		CheckNewFiles(*static_cast<FolderAssetBuildInfo*>(*assetIt), *static_cast<FolderAssetBuildInfo*>(assetInfo));
	}

	//after create files
	foreach(assetFolderInfo.mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == FolderAssetBuildInfo::GetStaticType())
			continue;

		AssetBuildInfo* assetInfo = buildedAssetFolderInfo.GetInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			assetInfo = (*assetIt)->Clone();
			buildedAssetFolderInfo.AddInsideAsset(assetInfo);

			//copy and convert new asset
			CopyAndConvertAsset(*assetIt, assetInfo);
		}
	}
}

void AssetBuildSystem::ConvertFiles(FolderAssetBuildInfo& assetFolderInfo, FolderAssetBuildInfo& buildedAssetFolderInfo)
{
	foreach(assetFolderInfo.mInsideAssets, assetIt)
	{
		AssetBuildInfo* assetInfo = buildedAssetFolderInfo.GetInsideAsset((*assetIt)->mLocation);
		if (!assetInfo)
			continue;

		if (!assetInfo->IsEquals(*assetIt))
		{
			//copy and convert new asset
			RemoveConvertedAsset(assetInfo);
			CopyAndConvertAsset(*assetIt, assetInfo);
		}

		if ((*assetIt)->GetType() == FolderAssetBuildInfo::GetStaticType())
			ConvertFiles(*static_cast<FolderAssetBuildInfo*>(*assetIt), *static_cast<FolderAssetBuildInfo*>(assetInfo));
	}
}

void AssetBuildSystem::CopyAndConvertAsset(AssetBuildInfo* assetInfo, AssetBuildInfo* buildAssetInfo)
{
	buildAssetInfo->CopyFrom(assetInfo);

	foreach(mAssetConverters, convIt)
	{
		if ((*convIt)->GetConvertingType() == assetInfo->GetType())
			(*convIt)->Convert(assetInfo);
	}
}

void AssetBuildSystem::RemoveConvertedAsset(AssetBuildInfo* buildAssetInfo)
{
	foreach(mAssetConverters, convIt)
	{
		if ((*convIt)->GetConvertingType() == buildAssetInfo->GetType())
			(*convIt)->Remove(buildAssetInfo);
	}
}

uint AssetBuildSystem::TryGetAssetsInfoMetaId(PathInfo &pathInfo, const String& path)
{
	uint res = 0;

	String metaPath = path + ".meta";

	if (pathInfo.IsFileExist(metaPath))
	{
		Serializer serializer;
		serializer.Load(mAssetsFolderPath + "/" + metaPath, false);
		serializer.Serialize(res, "id");
	}
	else if (AppConfig()->mAssetsUsesMetaIds)
	{
		uint metaId = GenerateFileId();
		Serializer serializer;
		serializer.Serialize(metaId, "id");
		serializer.Save(mAssetsFolderPath + "/" + metaPath, false);
		return metaId;
	}

	return res;
}

uint AssetBuildSystem::GenerateFileId() const
{
	return mAssets->GenerateFileId();
}

String AssetBuildSystem::GetAssetsFolderPath() const
{
	return mAssetsFolderPath;
}

String AssetBuildSystem::GetBuildedAssetsFolderPath() const
{
	return mBuildedAssetsFolderPath;
}

AssetBuildInfo* AssetBuildSystem::CreateAssetInfroFromFileInfo(const FileInfo& fileInfo)
{
	if (fileInfo.mFileType == FileType::Image)
		return mnew ImageAssetBuildInfo();
	else if (fileInfo.mFileType == FileType::Atlas)
		return mnew AtlasAssetBuildInfo();

	return mnew AssetBuildInfo();
}

void AssetBuildSystem::SaveBuildInfo()
{
	Serializer serializer;
	serializer.Serialize(&mBuildedAssetsFolderInfo, "assets");
	serializer.Save(mBuildedAssetsInfoFilePath);
}

void AssetBuildSystem::SaveAssetsInfo()
{
	AssetsBuildInfosArr assetsBuildInfos = mBuildedAssetsFolderInfo.GetAllInsideAssets();
	AssetsInfosArr assetsInfos;
	foreach(assetsBuildInfos, assetIt)
	{
		FileLocation loc = (*assetIt)->mLocation;
		loc.mPath = ExtractExtension(loc.mPath);
		assetsInfos.Add(AssetInfo(loc, (*assetIt)->GetType(), (*assetIt)->mWriteTime));
	}

	Serializer serializer;
	serializer.Serialize(assetsInfos, "assets");
	serializer.Save(ASSETS_INFO_FILE_PATH);
}

CLOSE_O2_NAMESPACE