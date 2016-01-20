#include "AssetsTree.h"

#include "Utils\Debug.h"
#include "Utils\FileSystem\FileSystem.h"
#include "Utils\Log\LogStream.h"

namespace o2
{
	AssetTree::AssetNode::AssetNode():
		mMeta(nullptr)
	{}

	AssetTree::AssetNode::AssetNode(const String& path, AssetId id, Type::Id type) :
		AssetInfo(path, id, type)
	{}

	AssetTree::AssetNode::~AssetNode()
	{
		delete mMeta;
	}

	AssetTree::AssetTree():
		mLog(nullptr)
	{}

	AssetTree::~AssetTree()
	{
		Clear();
	}

	void AssetTree::BuildTree(const String& path)
	{
		Clear();

		mPath = path;

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(path);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);
	}

	AssetTree::AssetNode* AssetTree::FindAsset(const String& path) const
	{
		return mAllAssets.FindMatch([&](const AssetNode* asset) { return asset->mPath == path; });
	}

	AssetTree::AssetNode* AssetTree::FindAsset(AssetId id) const
	{
		return mAllAssets.FindMatch([&](const AssetNode* asset) { return asset->mId == id; });
	}

	AssetInfo AssetTree::FindAssetInfo(const String& path) const
	{
		auto asset = FindAsset(path);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	AssetInfo AssetTree::FindAssetInfo(AssetId id) const
	{
		auto asset = FindAsset(id);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	AssetTree::AssetNode* AssetTree::AddAsset(AssetNode* asset)
	{
		int delPos = asset->mPath.FindLast("/");
		if (delPos < 0)
		{
			mAllAssets.Add(asset);
		}
		else
		{
			String parentPath = asset->mPath.SubStr(0, delPos);
			AssetNode* parent = mAllAssets.FindMatch([&](auto info) { return info->mPath == parentPath; });

			if (!parent)
			{
				if (mLog) mLog->Out("Failed to add builded asset info: %s", asset->mPath);
			}
			else
			{
				parent->AddChild(asset);
			}

			mAllAssets.Add(asset);
		}


		return asset;
	}

	void AssetTree::RemoveAsset(AssetNode* asset, bool release /*= true*/)
	{
		mAllAssets.Remove(asset);

		if (asset->GetParent())
			asset->GetParent()->RemoveChild(asset, release);
	}

	void AssetTree::Clear()
	{
		for (auto asset : mRootAssets)
			delete asset;

		mAllAssets.Clear();
		mRootAssets.Clear();
	}

	void AssetTree::LoadFolder(FolderInfo& folder, AssetNode* parentAsset)
	{
		for (auto fileInfo : folder.mFiles)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.mPath);

			if (extension != "meta")
			{
				String assetFullPath = mPath + fileInfo.mPath;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
					continue;

				LoadAsset(fileInfo.mPath, parentAsset, fileInfo.mEditDate);
			}
		}

		for (auto subFolder : folder.mFolders)
		{
			String folderFullPath = mPath + subFolder.mPath;
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

	AssetTree::AssetNode* AssetTree::LoadAsset(const String& path, AssetNode* parent, const TimeStamp& time)
	{
		DataNode metaData;
		metaData.LoadFromFile(mPath + path + ".meta");
		o2::Asset::IMetaInfo* meta;
		meta = metaData;

		AssetNode* asset = mnew AssetNode();

		asset->mMeta = meta;
		asset->mPath = path;
		asset->mTime = time;
		asset->mId = meta->ID();
		asset->mType = meta->GetAssetType();

		asset->SetParent(parent);

		mAllAssets.Add(asset);

		if (!parent)
			mRootAssets.Add(asset);

		return asset;
	}
}