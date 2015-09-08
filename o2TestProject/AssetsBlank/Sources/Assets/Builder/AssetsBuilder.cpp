#include "AssetsBuilder.h"

#include "Assets\FolderAsset.h"
#include "Utils\Debug.h"
#include "Utils\FileSystem\FileSystem.h"
#include "Utils\Log\LogStream.h"
#include "Utils\Timer.h"

namespace o2
{
	bool AssetsBuilder::AssetInfo::operator==(const AssetInfo& other) const
	{
		return mType == other.mType && mTime == other.mTime && mMeta->IsEqual(other.mMeta);
	}

	AssetsBuilder::AssetsBuilder()
	{
		mLog = mnew LogStream("Assets builder");
		Debug.GetLog()->BindStream(mLog);

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

		CreateMissingMetas();

		LoadSourceAssetsInfos();
		LoadBuildedAssetsInfos();

		ProcessModifiedAssets();
		ProcessRemovedAssets();
		ProcessNewAssets();

		mLog->OutStr("===================================");
		mLog->Out("Completed assets building \n        from: %s\n        to: %s\n        for %f seconds",
				  assetsPath, dataAssetsPath, timer.GetDeltaTime());
		mLog->OutStr("===================================\n");
	}

	void AssetsBuilder::InitializeConverters()
	{
		auto converterTypes = IAssetConverter::type.InheritedTypes();
		for (auto converterType : converterTypes)
		{
			Ptr<IAssetConverter> converter = static_cast<IAssetConverter*>(converterType->Sample()->Clone());
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

		for (auto _folder : folder.mFolders)
		{
			String folderFullPath = mSourceAssetsPath + _folder.mPath;
			String metaFullPath = folderFullPath + ".meta";
			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				auto assetType = &FolderAsset::type;
				GenerateMeta(assetType, metaFullPath);
			}

			ProcessMissingMetasCreation(_folder);
		}
	}

	void AssetsBuilder::LoadSourceAssetsInfos()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(mSourceAssetsPath);
		folderInfo.ClampPathNames();
		ProcessAssetsInfosLoading(folderInfo, &mSourceAssetsFolder, mSourceAssetsInfos);
	}

	void AssetsBuilder::LoadBuildedAssetsInfos()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(mBuildedAssetsPath);
		folderInfo.ClampPathNames();
		ProcessAssetsInfosLoading(folderInfo, &mBuildedAssetsFolder, mBuildedAssetsInfos);
	}

	void AssetsBuilder::ProcessAssetsInfosLoading(FolderInfo& folder, Ptr<AssetInfo> parentFolderInfo,
												  AssetsInfosVec& infosVec)
	{
		for (auto fileInfo : folder.mFiles)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.mPath);

			if (extension != "meta")
			{
				String assetFullPath = mSourceAssetsPath + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
				{
					mLog->Warning("Can't load asset info for %s - missing meta file", fileInfo.mPath);
					continue;
				}

				auto assetInfo = LoadAssetInfo(fileInfo.mPath, fileInfo.mEditDate, parentFolderInfo);
				infosVec.Add(assetInfo);
				parentFolderInfo->mChilds.Add(assetInfo);
			}
		}

		for (auto _folder : folder.mFolders)
		{
			String folderFullPath = mSourceAssetsPath + _folder.mPath;
			String metaFullPath = folderFullPath + ".meta";

			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				mLog->Warning("Can't load asset info for %s - missing meta file", _folder.mPath);
				continue;
			}

			auto assetInfo = LoadAssetInfo(_folder.mPath, TimeStamp(), parentFolderInfo);
			infosVec.Add(assetInfo);
			parentFolderInfo->mChilds.Add(assetInfo);

			ProcessAssetsInfosLoading(_folder, assetInfo, infosVec);
		}
	}

	void AssetsBuilder::ProcessRemovedAssets()
	{
		Type::Id folderTypeId = FolderAsset::type.ID();

		// in first pass skipping folders (only files), in second - files
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto bldAssetInfoIt = mBuildedAssetsInfos.Begin(); bldAssetInfoIt != mBuildedAssetsInfos.End(); )
			{
				bool isFolder = (*bldAssetInfoIt)->mType == folderTypeId;
				bool skip = pass == 0 ? isFolder:!isFolder;
				if (skip)
				{
					++bldAssetInfoIt;
					continue;
				}

				bool needRemove = true;
				for (auto srcAssetInfo : mSourceAssetsInfos)
				{
					if ((*bldAssetInfoIt)->mPath == srcAssetInfo->mPath &&
						(*bldAssetInfoIt)->mMeta->ID() == srcAssetInfo->mMeta->ID())
					{
						needRemove = false;
						break;
					}
				}

				if (needRemove)
				{
					++bldAssetInfoIt;
					continue;
				}

				GetAssetConverter((*bldAssetInfoIt)->mType)->RemoveAsset((*bldAssetInfoIt)->mPath);

				if ((*bldAssetInfoIt)->mParent)
					(*bldAssetInfoIt)->mParent->mChilds.Remove(*bldAssetInfoIt);

				bldAssetInfoIt = mBuildedAssetsInfos.Remove(bldAssetInfoIt);
			}
		}
	}

	void AssetsBuilder::ProcessModifiedAssets()
	{
	}

	void AssetsBuilder::ProcessNewAssets()
	{
		Type::Id folderTypeId = FolderAsset::type.ID();

		// in first pass skipping files (only folders), in second - folders
		for (int pass = 0; pass < 2; pass++)
		{
			for (auto srcAssetInfoIt = mSourceAssetsInfos.Begin(); srcAssetInfoIt != mSourceAssetsInfos.End(); )
			{
				bool isFolder = (*srcAssetInfoIt)->mType == folderTypeId;
				bool skip = pass == 0 ? !isFolder:isFolder;
				if (skip)
				{
					++srcAssetInfoIt;
					continue;
				}

				bool isNew = true;
				for (auto buildedAssetInfo : mBuildedAssetsInfos)
				{
					if ((*srcAssetInfoIt)->mPath == buildedAssetInfo->mPath &&
						(*srcAssetInfoIt)->mMeta->ID() == buildedAssetInfo->mMeta->ID())
					{
						isNew = false;
						break;
					}
				}

				if (!isNew)
				{
					++srcAssetInfoIt;
					continue;
				}

				GetAssetConverter((*srcAssetInfoIt)->mType)->ConvertAsset((*srcAssetInfoIt)->mPath);

				Ptr<AssetInfo> newBuildedAsset = mnew AssetInfo();
				newBuildedAsset->mPath = (*srcAssetInfoIt)->mPath;
				newBuildedAsset->mType = (*srcAssetInfoIt)->mType;
				newBuildedAsset->mTime = (*srcAssetInfoIt)->mTime;
				newBuildedAsset->mMeta = static_cast<Asset::IMetaInfo*>( (*srcAssetInfoIt)->mMeta->Clone() );

				mBuildedAssetsInfos.Add(newBuildedAsset);

				//and add to folders structure
				auto srcAssetInfoParent = (*srcAssetInfoIt)->mParent;
				auto buildedAssetParentFolder = mBuildedAssetsInfos.FindMatch(
					[&](Ptr<AssetInfo> info) { return *srcAssetInfoParent == *info; });

				if (!buildedAssetParentFolder)
				{
					mLog->Warning("Can't find builded asset target parent: %s", srcAssetInfoParent->mPath);
				}
				else
				{
					buildedAssetParentFolder->mChilds.Add(newBuildedAsset);
					newBuildedAsset->mParent = buildedAssetParentFolder;
				}
			}
		}
	}

	void AssetsBuilder::GenerateMeta(const Type* assetType, const String& metaFullPath)
	{
		auto assetTypeSample = static_cast<const Asset*>(assetType->Sample());
		auto assetTypeSampleMeta = assetTypeSample->GetMeta();

		DataNode metaData;
		metaData = assetTypeSampleMeta;
		*metaData["Value/mId"] = Assets::GetRandomAssetId();

		metaData.SaveToFile(metaFullPath);
	}

	Ptr<AssetsBuilder::AssetInfo> AssetsBuilder::LoadAssetInfo(const String& path, const TimeStamp& time,
															   Ptr<AssetInfo> parent)
	{
		DataNode metaData;
		metaData.LoadFromFile(mSourceAssetsPath + path + ".meta");
		Ptr<Asset::IMetaInfo> meta;
		meta = metaData;

		Ptr<AssetInfo> assetInfo = mnew AssetInfo();
		assetInfo->mMeta = meta;
		assetInfo->mPath = path;
		assetInfo->mTime = time;
		assetInfo->mType = meta->GetAssetType();
		assetInfo->mParent = parent;

		return assetInfo;
	}

	Ptr<IAssetConverter> AssetsBuilder::GetAssetConverter(Type::Id assetTypeId)
	{
		if (mAssetConverters.ContainsKey(assetTypeId))
			return mAssetConverters[assetTypeId];

		return &mStdAssetConverter;
	}

	void AssetsBuilder::Reset()
	{
		for (auto info : mSourceAssetsInfos)
			info.Release();

		mSourceAssetsInfos.Clear();
		mSourceAssetsFolder.mChilds.Clear();

		for (auto info : mBuildedAssetsInfos)
			info.Release();

		mBuildedAssetsInfos.Clear();
		mBuildedAssetsFolder.mChilds.Clear();
	}

}