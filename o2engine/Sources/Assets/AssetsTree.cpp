#include "AssetsTree.h"

#include "Utils\Debug.h"
#include "Utils\FileSystem\FileSystem.h"
#include "Utils\Log\LogStream.h"

namespace o2
{
	AssetTree::AssetNode::AssetNode()
	{}

	AssetTree::AssetNode::AssetNode(const String& path, UInt id, Type::Id type):
		AssetInfo(path, id, type)
	{
	}

	AssetTree::AssetTree()
	{}

	AssetTree::~AssetTree()
	{
		Clear();
	}

	void AssetTree::BuildTree(const String& path, Ptr<LogStream> log /*= nullptr*/)
	{
		Clear();

		mPath = path;
		if (log)
			mLog = log;
		else
			log = Debug.GetLog();

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(path);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);
	}

	Ptr<AssetTree::AssetNode> AssetTree::FindAsset(const String& path) const
	{
		return mAllAssets.FindMatch([&](Ptr<AssetNode> asset) { return asset->mPath == path; });
	}

	Ptr<AssetTree::AssetNode> AssetTree::FindAsset(UInt id) const
	{
		return mAllAssets.FindMatch([&](Ptr<AssetNode> asset) { return asset->mId == id; });
	}

	AssetInfo AssetTree::FindAssetInfo(const String& path) const
	{
		auto asset = FindAsset(path);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	AssetInfo AssetTree::FindAssetInfo(UInt id) const
	{
		auto asset = FindAsset(id);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	Ptr<AssetTree::AssetNode> AssetTree::AddAsset(Ptr<AssetNode> asset)
	{
		int delPos = asset->mPath.FindLast("/");
		if (delPos < 0)
		{
			mAllAssets.Add(asset);
		}
		else
		{
			String parentPath = asset->mPath.SubStr(0, delPos);
			Ptr<AssetNode> parent = mAllAssets.FindMatch([&](auto info) { return info->mPath == parentPath; });

			if (!parent)
				mLog->Out("Failed to add builded asset info: %s", asset->mPath);
			else
			{
				parent->AddChild(asset);
			}

			mAllAssets.Add(asset);
		}


		return asset;
	}

	void AssetTree::RemoveAsset(Ptr<AssetNode> asset, bool release /*= true*/)
	{
		mAllAssets.Remove(asset);

		if (asset->GetParent())
			asset->GetParent()->RemoveChild(asset, release);
	}

	void AssetTree::Clear()
	{
		for (auto asset : mRootAssets)
			asset.Release();

		mAllAssets.Clear();
		mRootAssets.Clear();
	}

	void AssetTree::LoadFolder(FolderInfo& folder, Ptr<AssetNode> parentAsset)
	{
		for (auto fileInfo : folder.mFiles)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.mPath);

			if (extension != "meta")
			{
				String assetFullPath = ASSETS_PATH + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
					continue;

				LoadAsset(fileInfo.mPath, parentAsset, fileInfo.mEditDate);
			}
		}

		for (auto subFolder : folder.mFolders)
		{
			String folderFullPath = ASSETS_PATH + subFolder.mPath;
			String metaFullPath = folderFullPath + ".meta";

			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				mLog->Warning("Can't load asset info for %s - missing meta file", subFolder.mPath);
				continue;
			}

			auto asset = LoadAsset(subFolder.mPath, parentAsset, TimeStamp());

			LoadFolder(subFolder, asset);
		}
	}

	Ptr<AssetTree::AssetNode> AssetTree::LoadAsset(const String& path, Ptr<AssetNode> parent, const TimeStamp& time)
	{
		DataNode metaData;
		metaData.LoadFromFile(mPath + path + ".meta");
		Ptr<o2::Asset::IMetaInfo> meta;
		meta = metaData;

		Ptr<AssetNode> asset = mnew AssetNode();

		asset->mMeta = meta;
		asset->mPath = path;
		asset->mTime = time;
		asset->mId   = meta->ID();
		asset->mType = meta->GetAssetType();

		asset->SetParent(parent);

		mAllAssets.Add(asset);

		if (!parent)
			mRootAssets.Add(asset);

		return asset;
	}
}