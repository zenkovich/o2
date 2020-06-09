#include "o2/stdafx.h"
#include "AssetsTree.h"

#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	AssetsTree::AssetsTree() :
		log(nullptr)
	{}

	AssetsTree::~AssetsTree()
	{
		Clear();
	}

	void AssetsTree::Build(const String& path)
	{
		this->assetsPath = path;

		if (!o2FileSystem.IsFolderExist(path))
			o2FileSystem.FolderCreate(path);

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(path);
		folderInfo.ClampPathNames();

		LoadFolder(folderInfo, nullptr);

		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

	void AssetsTree::Build(const FolderInfo& folderInfo)
	{
		LoadFolder(folderInfo, nullptr);

		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

	void AssetsTree::Rebuild()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(assetsPath);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);

		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

	void AssetsTree::SortAssets()
	{
		allAssets.Sort([](AssetInfo* a, AssetInfo* b) { return a->path.Length() < b->path.Length(); });
	}

	void AssetsTree::SortAssetsInverse()
	{
		allAssets.Sort([](AssetInfo* a, AssetInfo* b) { return a->path.Length() > b->path.Length(); });
	}

	AssetInfo* AssetsTree::Find(const String& path) const
	{
		AssetInfo* res = nullptr;
		allAssetsByPath.TryGetValue(path, res);
		return res;
	}

	AssetInfo* AssetsTree::Find(const UID& id) const
	{
		AssetInfo* res = nullptr;
		allAssetsByUID.TryGetValue(id, res);
		return res;
	}

	AssetInfo* AssetsTree::AddAsset(AssetInfo* asset)
	{
		int delPos = asset->path.FindLast("/");
		if (delPos < 0)
		{
			rootAssets.Add(asset);
			asset->SetTree(this);
		}
		else
		{
			String parentPath = asset->path.SubStr(0, delPos);
			AssetInfo* parent = nullptr;
			allAssetsByPath.TryGetValue(parentPath, parent);

			if (!parent)
			{
				if (log)
					log->Out("Failed to add built asset info: " + asset->path);
			}
			else parent->AddChild(asset);

			asset->SetTree(this);
		}

		return asset;
	}

	void AssetsTree::RemoveAsset(AssetInfo* asset, bool release /*= true*/)
	{
		if (allAssetsByPath[asset->path] == asset)
			allAssetsByPath.Remove(asset->path);

		if (asset->meta && allAssetsByUID[asset->meta->ID()] == asset)
			allAssetsByUID.Remove(asset->meta->ID());

		allAssets.Remove(asset);
		rootAssets.Remove(asset);

		if (asset->parent)
			asset->parent->RemoveChild(asset, false);

		if (asset->meta->GetAssetType() == &TypeOf(FolderAsset) && release)
		{
			auto& childs = asset->children;
			for (auto ch : childs)
				RemoveAsset(ch, release);
		}

		if (release)
			delete asset;
	}

	void AssetsTree::Clear()
	{
		for (auto asset : rootAssets)
			delete asset;

		allAssets.Clear();
		rootAssets.Clear();
		allAssetsByPath.Clear();
		allAssetsByUID.Clear();
	}

	void AssetsTree::LoadFolder(const FolderInfo& folder, AssetInfo* parentAsset)
	{
		auto& parentChilds = parentAsset ? parentAsset->children : rootAssets;

		for (auto fileInfo : folder.files)
		{
			String extension = o2FileSystem.GetFileExtension(fileInfo.path);

			if (extension != "meta")
			{
				String assetFullPath = assetsPath + fileInfo.path;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
					continue;

				LoadAssetNode(fileInfo.path, parentAsset, fileInfo.editDate);
			}
		}

		for (auto subFolder : folder.folders)
		{
			AssetInfo* asset = nullptr;

			String folderFullPath = assetsPath + subFolder.path;
			String metaFullPath = folderFullPath + ".meta";

			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				if (log)
					log->Warning("Can't load asset info for " + subFolder.path + " - missing meta file");

				continue;
			}

			asset = LoadAssetNode(subFolder.path, parentAsset, TimeStamp());

			LoadFolder(subFolder, asset);
		}
	}

	AssetInfo* AssetsTree::LoadAssetNode(const String& path, AssetInfo* parent, const TimeStamp& time)
	{
		DataDocument metaData;
		metaData.LoadFromFile(this->assetsPath + path + ".meta");

		AssetMeta* meta = nullptr;;
		meta = metaData;

		AssetInfo* asset = mnew AssetInfo();

		asset->meta = meta;
		asset->path = path;
		asset->editTime = time;
		asset->SetParent(parent);

		if (!parent)
			rootAssets.Add(asset);

		return asset;
	}

	void AssetsTree::OnDeserialized(const DataValue& node)
	{
		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

}

DECLARE_CLASS(o2::AssetsTree);
