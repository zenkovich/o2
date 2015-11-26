#include "AssetsBuilder.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/FolderAsset.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Timer.h"

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

	void AssetsBuilder::BuildAssets(const String& assetsPath, const String& dataAssetsPath, bool forcible /*= false*/)
	{
		Reset();

		mSourceAssetsPath = assetsPath;
		mBuildedAssetsPath = dataAssetsPath;

		mLog->OutStr("===================================");
		mLog->Out("Started assets building \n        from: %s\n        to: %s",
				  assetsPath, dataAssetsPath);
		mLog->OutStr("===================================\n");

		Timer timer;

		if (forcible)
			RemoveBuildedAssets();

		CheckBasicAtlas();

		CreateMissingMetas();

		mSourceAssetsTree.BuildTree(assetsPath);
		mBuildedAssetsTree.BuildTree(dataAssetsPath);

		ProcessModifiedAssets();
		ProcessRemovedAssets();
		ProcessNewAssets();

		ConvertersPostProcess();

		mLog->OutStr("===================================");
		mLog->Out("Completed assets building \n        from: %s\n        to: %s\n        for %f seconds",
				  assetsPath, dataAssetsPath, timer.GetDeltaTime());
		mLog->OutStr("===================================\n");
	}

	void AssetsBuilder::InitializeConverters()
	{
		auto converterTypes = IAssetConverter::type->DerivedTypes();
		for (auto converterType : converterTypes)
		{
			Ptr<IAssetConverter> converter = converterType->CreateSample();
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
		String basicAtlasFullPath = mSourceAssetsPath + BASIC_ATLAS_PATH;

		if (!o2FileSystem.IsFileExist(basicAtlasFullPath))
		{
			AtlasAsset basicAtlas;
			basicAtlas.Save(BASIC_ATLAS_PATH, false);
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
					mLog->Warning("Missing asset for meta: %s - removing meta", fileInfo.mPath);
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
					GenerateMeta(assetType, metaFullPath);
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
				auto assetType = FolderAsset::type;
				GenerateMeta(assetType, metaFullPath);
			}

			ProcessMissingMetasCreation(subFolder);
		}
	}

	void AssetsBuilder::ProcessRemovedAssets()
	{
		Type::Id folderTypeId = FolderAsset::type->ID();

		// in first pass skipping folders (only files), in second - files
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto bldAssetInfoIt = mBuildedAssetsTree.mAllAssets.Begin(); bldAssetInfoIt != mBuildedAssetsTree.mAllAssets.End(); )
			{
				bool isFolder = (*bldAssetInfoIt)->mType == folderTypeId;
				bool skip = pass == 0 ? isFolder:!isFolder;
				if (skip)
				{
					++bldAssetInfoIt;
					continue;
				}

				bool needRemove = true;
				for (auto srcAssetInfo : mSourceAssetsTree.mAllAssets)
				{
					if ((*bldAssetInfoIt)->mPath == srcAssetInfo->mPath &&
						(*bldAssetInfoIt)->mMeta->ID() == srcAssetInfo->mMeta->ID())
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

				GetAssetConverter((*bldAssetInfoIt)->mType)->RemoveAsset(**bldAssetInfoIt);

				mLog->OutStr("Removed asset: " + (*bldAssetInfoIt)->mPath);

				auto bldAssetInfo = *bldAssetInfoIt;

				bldAssetInfoIt = mBuildedAssetsTree.mAllAssets.Remove(bldAssetInfoIt);

				if ((*bldAssetInfoIt)->GetParent())
					(*bldAssetInfoIt)->GetParent()->RemoveChild(bldAssetInfo);
			}
		}
	}

	void AssetsBuilder::ProcessModifiedAssets()
	{
		Type::Id folderTypeId = FolderAsset::type->ID();

		// in first pass skipping files (only folders), in second - folders
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto srcAssetInfo : mSourceAssetsTree.mAllAssets)
			{
				bool isFolder = srcAssetInfo->mType == folderTypeId;
				bool skip = pass == 0 ? !isFolder:isFolder;
				if (skip)
					continue;

				for (auto buildedAssetInfo : mBuildedAssetsTree.mAllAssets)
				{
					if (srcAssetInfo->mMeta->ID() == buildedAssetInfo->mMeta->ID())
					{
						if (srcAssetInfo->mPath == buildedAssetInfo->mPath)
						{
							if (srcAssetInfo->mTime != buildedAssetInfo->mTime ||
								!srcAssetInfo->mMeta->IsEqual(buildedAssetInfo->mMeta))
							{
								GetAssetConverter(srcAssetInfo->mType)->ConvertAsset(*srcAssetInfo);
								buildedAssetInfo->mTime = srcAssetInfo->mTime;
								buildedAssetInfo->mMeta.Release();
								buildedAssetInfo->mMeta = static_cast<Asset::IMetaInfo*>(srcAssetInfo->mMeta->Clone());

								mModifiedAssets.Add(buildedAssetInfo);

								mLog->Out("Modified asset: %s", srcAssetInfo->mPath);
							}
						}
						else
						{
							if (srcAssetInfo->mTime != buildedAssetInfo->mTime ||
								!srcAssetInfo->mMeta->IsEqual(buildedAssetInfo->mMeta))
							{
								GetAssetConverter(buildedAssetInfo->mType)->RemoveAsset(*buildedAssetInfo);

								mBuildedAssetsTree.RemoveAsset(buildedAssetInfo, false);

								buildedAssetInfo->mPath = srcAssetInfo->mPath;
								buildedAssetInfo->mTime = srcAssetInfo->mTime;
								buildedAssetInfo->mMeta.Release();
								buildedAssetInfo->mMeta = static_cast<Asset::IMetaInfo*>(srcAssetInfo->mMeta->Clone());
								buildedAssetInfo->mId = buildedAssetInfo->mMeta->ID();

								GetAssetConverter(srcAssetInfo->mType)->ConvertAsset(*srcAssetInfo);
								mLog->Out("Modified and moved to %s asset: %s", srcAssetInfo->mPath, buildedAssetInfo->mPath);

								mModifiedAssets.Add(buildedAssetInfo);

								mBuildedAssetsTree.AddAsset(buildedAssetInfo);
							}
							else
							{
								GetAssetConverter(srcAssetInfo->mType)->MoveAsset(*buildedAssetInfo, *srcAssetInfo);
								mLog->Out("Moved asset: %s to %s", buildedAssetInfo->mPath, srcAssetInfo->mPath);

								mBuildedAssetsTree.RemoveAsset(buildedAssetInfo, false);

								buildedAssetInfo->mPath = srcAssetInfo->mPath;
								buildedAssetInfo->mTime = srcAssetInfo->mTime;
								buildedAssetInfo->mMeta.Release();
								buildedAssetInfo->mMeta = static_cast<Asset::IMetaInfo*>(srcAssetInfo->mMeta->Clone());

								mBuildedAssetsTree.AddAsset(buildedAssetInfo);
							}
						}

						break;
					}
				}
			}
		}
	}

	void AssetsBuilder::ProcessNewAssets()
	{
		Type::Id folderTypeId = FolderAsset::type->ID();

		// in first pass skipping files (only folders), in second - folders
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto srcAssetInfoIt = mSourceAssetsTree.mAllAssets.Begin(); srcAssetInfoIt != mSourceAssetsTree.mAllAssets.End(); ++srcAssetInfoIt)
			{
				bool isFolder = (*srcAssetInfoIt)->mType == folderTypeId;
				bool skip = pass == 0 ? !isFolder:isFolder;

				if (skip)
					continue;

				bool isNew = true;
				for (auto buildedAssetInfo : mBuildedAssetsTree.mAllAssets)
				{
					if ((*srcAssetInfoIt)->mPath == buildedAssetInfo->mPath &&
						(*srcAssetInfoIt)->mMeta->ID() == buildedAssetInfo->mMeta->ID())
					{
						isNew = false;
						break;
					}
				}

				if (!isNew)
					continue;

				GetAssetConverter((*srcAssetInfoIt)->mType)->ConvertAsset(**srcAssetInfoIt);

				mLog->Out("New asset: %s", (*srcAssetInfoIt)->mPath);

				Ptr<AssetTree::AssetNode> newBuildedAsset = mnew AssetTree::AssetNode();
				newBuildedAsset->mPath = (*srcAssetInfoIt)->mPath;
				newBuildedAsset->mType = (*srcAssetInfoIt)->mType;
				newBuildedAsset->mTime = (*srcAssetInfoIt)->mTime;
				newBuildedAsset->mMeta = static_cast<Asset::IMetaInfo*>((*srcAssetInfoIt)->mMeta->Clone());
				newBuildedAsset->mId   = newBuildedAsset->mMeta->ID();

				mBuildedAssetsTree.AddAsset(newBuildedAsset);
			}
		}
	}

	void AssetsBuilder::ConvertersPostProcess()
	{
		for (auto it = mAssetConverters.Begin(); it != mAssetConverters.End(); ++it)
			it.Value()->AssetsPostProcess();

		mStdAssetConverter.AssetsPostProcess();
	}

	void AssetsBuilder::GenerateMeta(Ptr<Type> assetType, const String& metaFullPath)
	{
		auto assetTypeSample = static_cast<const Asset*>(assetType->Sample());
		auto assetTypeSampleMeta = assetTypeSample->GetMeta();

		DataNode metaData;
		metaData = assetTypeSampleMeta;
		*metaData["Value/mId"] = Assets::GetRandomAssetId();

		metaData.SaveToFile(metaFullPath);
	}

	Ptr<IAssetConverter> AssetsBuilder::GetAssetConverter(Type::Id assetTypeId)
	{
		if (mAssetConverters.ContainsKey(assetTypeId))
			return mAssetConverters[assetTypeId];

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