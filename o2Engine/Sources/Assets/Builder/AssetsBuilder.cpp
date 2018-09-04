#include "stdafx.h"
#include "AssetsBuilder.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/Builder/AtlasAssetConverter.h"
#include "Assets/Builder/FolderAssetConverter.h"
#include "Assets/Builder/ImageAssetConverter.h"
#include "Assets/FolderAsset.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/System/Time/Timer.h"

namespace o2
{
	AssetsBuilder::AssetsBuilder()
	{
		mLog = mnew LogStream("Assets builder");
		o2Debug.GetLog()->BindStream(mLog);

		InitializeConverters();
	}

	AssetsBuilder::~AssetsBuilder()
	{
		Reset();
	}

	AssetsBuilder::AssetsIdsVec AssetsBuilder::BuildAssets(const String& assetsPath, const String& dataAssetsPath, 
														   bool forcible /*= false*/)
	{
		Reset();

		AssetsIdsVec res;

		mSourceAssetsPath = assetsPath;
		mBuildedAssetsPath = dataAssetsPath;

		mLog->OutStr("===================================");
		mLog->Out("Started assets building \n from: " + assetsPath + "\n to: " + dataAssetsPath);
		mLog->OutStr("===================================\n");

		Timer timer;

		if (forcible)
			RemoveBuildedAssets();

		CheckBasicAtlas();

		CreateMissingMetas();

		mSourceAssetsTree.BuildTree(assetsPath);
		mBuildedAssetsTree.BuildTree(dataAssetsPath);

		res.Add(ProcessModifiedAssets());
		res.Add(ProcessRemovedAssets());
		res.Add(ProcessNewAssets());
		res.Add(ConvertersPostProcess());

		mLog->OutStr("===================================");
		mLog->Out("Completed assets building \n from: " + assetsPath + "\n to: " + dataAssetsPath + 
				  " for " + (String)timer.GetDeltaTime() + " seconds");
		mLog->OutStr("===================================\n");

		return res;
	}

	void AssetsBuilder::InitializeConverters()
	{
		auto converterTypes = TypeOf(IAssetConverter).GetDerivedTypes();
		for (auto converterType : converterTypes)
		{
			IAssetConverter* converter = (IAssetConverter*)converterType->CreateSample();
			converter->SetAssetsBuilder(this);
			auto availableAssetTypes = converter->GetProcessingAssetsTypes();
			for (auto tp : availableAssetTypes)
				mAssetConverters.Add(tp, converter);
		}
	}

	void AssetsBuilder::RemoveBuildedAssets()
	{
		o2FileSystem.FolderRemove(mBuildedAssetsPath);
		o2FileSystem.FolderCreate(mBuildedAssetsPath);
	}

	void AssetsBuilder::CheckBasicAtlas()
	{
		String basicAtlasFullPath = mSourceAssetsPath + GetBasicAtlasPath();

		if (!o2FileSystem.IsFileExist(basicAtlasFullPath))
		{
			AtlasAssetRef basicAtlas = AtlasAssetRef::CreateAsset();
			basicAtlas->Save(GetBasicAtlasPath(), false);
		}
	}

	void AssetsBuilder::CreateMissingMetas()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(mSourceAssetsPath);
		folderInfo.ClampPathNames();

		ProcessMissingMetasCreation(folderInfo);
	}

	void AssetsBuilder::ProcessMissingMetasCreation(FolderInfo& folder)
	{
		for (auto fileInfo : folder.mFiles)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.mPath);

			if (extension == "meta")
			{
				String metaFullPath = mSourceAssetsPath + fileInfo.mPath;
				String assetForMeta = o2FileSystem.GetFileNameWithoutExtension(metaFullPath);
				bool isExistAssetForMeta = o2FileSystem.IsFileExist(assetForMeta) ||
					o2FileSystem.IsFolderExist(assetForMeta);
				if (!isExistAssetForMeta)
				{
					mLog->Warning("Missing asset for meta: " + fileInfo.mPath + " - removing meta");
					o2FileSystem.FileDelete(metaFullPath);
				}
			}
			else
			{
				String assetFullPath = mSourceAssetsPath + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";
				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
				{
					auto assetType = o2Assets.GetAssetTypeByExtension(extension);
					GenerateMeta(*assetType, metaFullPath);
				}
			}
		}

		for (auto subFolder : folder.mFolders)
		{
			String folderFullPath = mSourceAssetsPath + subFolder.mPath;
			String metaFullPath = folderFullPath + ".meta";
			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				auto& assetType = TypeOf(FolderAsset);
				GenerateMeta(assetType, metaFullPath);
			}

			ProcessMissingMetasCreation(subFolder);
		}
	}

	AssetsBuilder::AssetsIdsVec AssetsBuilder::ProcessRemovedAssets()
	{
		AssetsIdsVec res;
		const Type* folderTypeId = &TypeOf(FolderAsset);

		// in first pass skipping folders (only files), in second - files
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto bldAssetInfoIt = mBuildedAssetsTree.mAllAssets.Begin(); bldAssetInfoIt != mBuildedAssetsTree.mAllAssets.End(); )
			{
				bool isFolder = (*bldAssetInfoIt)->assetType == folderTypeId;
				bool skip = pass == 0 ? isFolder:!isFolder;
				if (skip)
				{
					++bldAssetInfoIt;
					continue;
				}

				bool needRemove = true;
				for (auto srcAssetInfo : mSourceAssetsTree.mAllAssets)
				{
					if ((*bldAssetInfoIt)->path == srcAssetInfo->path &&
						(*bldAssetInfoIt)->meta->ID() == srcAssetInfo->meta->ID())
					{
						needRemove = false;
						break;
					}
				}

				if (!needRemove)
				{
					++bldAssetInfoIt;
					continue;
				}

				for (auto srcAssetInfo : mSourceAssetsTree.mAllAssets)
				{
					if ((*bldAssetInfoIt)->path == srcAssetInfo->path &&
						(*bldAssetInfoIt)->meta->ID() == srcAssetInfo->meta->ID())
					{
						needRemove = false;
						break;
					}
				}

				GetAssetConverter((*bldAssetInfoIt)->assetType)->RemoveAsset(**bldAssetInfoIt);

				res.Add((*bldAssetInfoIt)->id);

				mLog->OutStr("Removed asset: " + (*bldAssetInfoIt)->path);

				auto bldAssetInfo = *bldAssetInfoIt;

				bldAssetInfoIt = mBuildedAssetsTree.mAllAssets.Remove(bldAssetInfoIt);

				if ((*bldAssetInfoIt)->GetParent())
					(*bldAssetInfoIt)->GetParent()->RemoveChild(bldAssetInfo);
			}
		}

		return res;
	}

	AssetsBuilder::AssetsIdsVec AssetsBuilder::ProcessModifiedAssets()
	{
		AssetsIdsVec res;
		const Type* folderType = &TypeOf(FolderAsset);

		// in first pass skipping files (only folders), in second - folders
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto srcAssetInfo : mSourceAssetsTree.mAllAssets)
			{
				bool isFolder = srcAssetInfo->assetType == folderType;
				bool skip = pass == 0 ? !isFolder:isFolder;
				if (skip)
					continue;

				for (auto buildedAssetInfo : mBuildedAssetsTree.mAllAssets)
				{
					if (srcAssetInfo->meta->ID() == buildedAssetInfo->meta->ID())
					{
						if (srcAssetInfo->path == buildedAssetInfo->path)
						{
							if (srcAssetInfo->time != buildedAssetInfo->time ||
								!srcAssetInfo->meta->IsEqual(buildedAssetInfo->meta))
							{
								GetAssetConverter(srcAssetInfo->assetType)->ConvertAsset(*srcAssetInfo);
								res.Add(srcAssetInfo->id);
								buildedAssetInfo->time = srcAssetInfo->time;
								delete buildedAssetInfo->meta;
								buildedAssetInfo->meta = srcAssetInfo->meta->CloneAs<Asset::IMetaInfo>();

								mModifiedAssets.Add(buildedAssetInfo);

								mLog->Out("Modified asset: " + srcAssetInfo->path);
							}
						}
						else
						{
							if (srcAssetInfo->time != buildedAssetInfo->time ||
								!srcAssetInfo->meta->IsEqual(buildedAssetInfo->meta))
							{
								GetAssetConverter(buildedAssetInfo->assetType)->RemoveAsset(*buildedAssetInfo);

								mBuildedAssetsTree.RemoveAsset(buildedAssetInfo, false);

								buildedAssetInfo->path = srcAssetInfo->path;
								buildedAssetInfo->time = srcAssetInfo->time;
								delete buildedAssetInfo->meta;
								buildedAssetInfo->meta = srcAssetInfo->meta->CloneAs<Asset::IMetaInfo>();
								buildedAssetInfo->id = buildedAssetInfo->meta->ID();

								GetAssetConverter(srcAssetInfo->assetType)->ConvertAsset(*srcAssetInfo);
								mLog->Out("Modified and moved to " + srcAssetInfo->path + " asset: " + buildedAssetInfo->path);

								res.Add(srcAssetInfo->id);

								mModifiedAssets.Add(buildedAssetInfo);

								mBuildedAssetsTree.AddAsset(buildedAssetInfo);
							}
							else
							{
								GetAssetConverter(srcAssetInfo->assetType)->MoveAsset(*buildedAssetInfo, *srcAssetInfo);
								res.Add(srcAssetInfo->id);
								mLog->Out("Moved asset: " + buildedAssetInfo->path + " to " + srcAssetInfo->path);

								mBuildedAssetsTree.RemoveAsset(buildedAssetInfo, false);

								buildedAssetInfo->path = srcAssetInfo->path;
								buildedAssetInfo->time = srcAssetInfo->time;
								delete buildedAssetInfo->meta;
								buildedAssetInfo->meta = srcAssetInfo->meta->CloneAs<Asset::IMetaInfo>();

								mBuildedAssetsTree.AddAsset(buildedAssetInfo);
							}
						}

						break;
					}
				}
			}
		}

		return res;
	}

	AssetsBuilder::AssetsIdsVec AssetsBuilder::ProcessNewAssets()
	{
		AssetsIdsVec res;
		const Type* folderType = &TypeOf(FolderAsset);

		// in first pass skipping files (only folders), in second - folders
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto srcAssetInfoIt = mSourceAssetsTree.mAllAssets.Begin(); srcAssetInfoIt != mSourceAssetsTree.mAllAssets.End(); ++srcAssetInfoIt)
			{
				bool isFolder = (*srcAssetInfoIt)->assetType == folderType;
				bool skip = pass == 0 ? !isFolder:isFolder;

				if (skip)
					continue;

				bool isNew = true;
				for (auto buildedAssetInfo : mBuildedAssetsTree.mAllAssets)
				{
					if ((*srcAssetInfoIt)->path == buildedAssetInfo->path &&
						(*srcAssetInfoIt)->meta->ID() == buildedAssetInfo->meta->ID())
					{
						isNew = false;
						break;
					}
				}

				if (!isNew)
					continue;

				GetAssetConverter((*srcAssetInfoIt)->assetType)->ConvertAsset(**srcAssetInfoIt);

				res.Add((*srcAssetInfoIt)->id);

				mLog->Out("New asset: " + (*srcAssetInfoIt)->path);

				AssetTree::AssetNode* newBuildedAsset = mnew AssetTree::AssetNode();
				newBuildedAsset->path = (*srcAssetInfoIt)->path;
				newBuildedAsset->assetType = (*srcAssetInfoIt)->assetType;
				newBuildedAsset->time = (*srcAssetInfoIt)->time;
				newBuildedAsset->meta = (*srcAssetInfoIt)->meta->CloneAs<Asset::IMetaInfo>();
				newBuildedAsset->id   = newBuildedAsset->meta->ID();

				mBuildedAssetsTree.AddAsset(newBuildedAsset);
			}
		}

		return res;
	}

	AssetsBuilder::AssetsIdsVec AssetsBuilder::ConvertersPostProcess()
	{
		AssetsIdsVec res;

		for (auto it = mAssetConverters.Begin(); it != mAssetConverters.End(); ++it)
			res.Add(it.Value()->AssetsPostProcess());

		res.Add(mStdAssetConverter.AssetsPostProcess());

		return res;
	}

	void AssetsBuilder::GenerateMeta(const Type& assetType, const String& metaFullPath)
	{
		auto assetTypeSample = (Asset*)assetType.CreateSample();
		auto assetTypeSampleMeta = assetTypeSample->GetMeta();

		DataNode metaData;
		metaData = assetTypeSampleMeta;
		metaData["Value/mId"] = Assets::GetRandomAssetId();

		metaData.SaveToFile(metaFullPath);

		delete assetTypeSample;
	}

	IAssetConverter* AssetsBuilder::GetAssetConverter(const Type* assetType)
	{
		if (mAssetConverters.ContainsKey(assetType))
			return mAssetConverters[assetType];

		return &mStdAssetConverter;
	}

	void AssetsBuilder::Reset()
	{
		mModifiedAssets.Clear();
		mSourceAssetsTree.Clear();
		mBuildedAssetsTree.Clear();

		for (auto it = mAssetConverters.Begin(); it != mAssetConverters.End(); ++it)
			it.Value()->Reset();

		mStdAssetConverter.Reset();
	}
}