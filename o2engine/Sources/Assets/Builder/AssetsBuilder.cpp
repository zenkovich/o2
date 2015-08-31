#include "AssetsBuilder.h"

#include "Utils\Debug.h"
#include "Utils\FileSystem\FileSystem.h"
#include "Utils\Log\LogStream.h"
#include "Utils\Timer.h"

namespace o2
{
	bool AssetsBuilder::AssetInfo::operator==(const AssetInfo& other) const
	{
		return mId == other.mId && mTime == other.mTime && mMeta->IsEqual(other.mMeta);
	}

	AssetsBuilder::AssetsBuilder()
	{
		mLog = mnew LogStream("Assets builder");
		Debug.GetLog()->BindStream(mLog);
	}

	AssetsBuilder::~AssetsBuilder()
	{
	}

	void AssetsBuilder::BuildAssets(const String& assetsPath, const String& dataAssetsPath)
	{
		mSourceAssetsPath = assetsPath;
		mBuildedAssetsPath = dataAssetsPath;

		mLog->Out("Started assets building from %s\nto %s", assetsPath, dataAssetsPath);
		Timer timer;

		CreateMissingMetas();

		LoadSourceAssetsInfos();
		LoadBuildedAssetsInfos();

		ProcessMovedAssets();
		ProcessRemovedAssets();
		ProcessModifiedAssets();
		ProcessNewAssets();

		mLog->Out("Completed assets building from %s\nto %s\nfor %f seconds", assetsPath, dataAssetsPath, timer.GetDeltaTime());
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

			if (extension == ".meta")
			{
				String metaFullPath = mSourceAssetsPath + fileInfo.mPath;
				String assetForMeta = o2FileSystem.GetFileNameWithoutExtension(metaFullPath);
				bool isExistAssetForMeta = o2FileSystem.IsFileExist(assetForMeta) || o2FileSystem.IsDirectoryExist(assetForMeta);
				if (!isExistAssetForMeta)
				{
					mLog->Warning("Missing asset for meta: %s - removing meta", fileInfo.mPath);
					o2FileSystem.RemoveFile(metaFullPath);
				}
			}
			else
			{
				String assetFullPath = mSourceAssetsPath + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";
				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
				{
					OutFile file(metaFullPath);
					file.Close();
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
				OutFile file(metaFullPath);
				file.Close();
			}

			ProcessMissingMetasCreation(_folder);
		}
	}

	void AssetsBuilder::LoadSourceAssetsInfos()
	{
		mSourceAssetsInfos.Clear();

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(mSourceAssetsPath);
		ProcessSourceAssetsInfodLoading(folderInfo);
	}

	void AssetsBuilder::ProcessSourceAssetsInfodLoading(FolderInfo& folder)
	{
		for (auto fileInfo : folder.mFiles)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.mPath);

			if (extension != ".meta")
			{
				String assetFullPath = mSourceAssetsPath + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
				{
					mLog->Warning("Can't load asset info for %s - missing meta file", fileInfo.mPath);
					continue;
				}


			}
		}

// 		for (auto _folder : folder.mFolders)
// 		{
// 			String folderFullPath = mSourceAssetsPath + _folder.mPath;
// 			String metaFullPath = folderFullPath + ".meta";
// 			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
// 			if (!isExistMetaForFolder)
// 			{
// 				OutFile file(metaFullPath);
// 				file.Close();
// 			}
// 
// 			ProcessMissingMetasCreation(_folder);
// 		}
	}

	void AssetsBuilder::LoadBuildedAssetsInfos()
	{
		mBuildedAssetsInfos.Clear();

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(mBuildedAssetsPath);
	}

	void AssetsBuilder::ProcessMovedAssets()
	{
	}

	void AssetsBuilder::ProcessRemovedAssets()
	{
	}

	void AssetsBuilder::ProcessModifiedAssets()
	{
	}

	void AssetsBuilder::ProcessNewAssets()
	{
	}

}