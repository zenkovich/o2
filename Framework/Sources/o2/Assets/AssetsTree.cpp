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

		for (auto& asset : rootAssets)
			asset->SetTree(Ref(this));
	}

	void AssetsTree::Build(const FolderInfo& folderInfo)
	{
		LoadFolder(folderInfo, nullptr);

		for (auto& asset : rootAssets)
			asset->SetTree(Ref(this));
	}

	void AssetsTree::Rebuild()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(assetsPath);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);

		for (auto& asset : rootAssets)
			asset->SetTree(Ref(this));
	}

	void AssetsTree::SortAssets()
	{
		allAssets.Sort([](const Ref<AssetInfo>& a, const Ref<AssetInfo>& b) { return a->path.Length() < b->path.Length(); });
	}

	void AssetsTree::SortAssetsInverse()
	{
		allAssets.Sort([](const Ref<AssetInfo>& a, const Ref<AssetInfo>& b) { return a->path.Length() > b->path.Length(); });
	}

	Ref<AssetInfo> AssetsTree::Find(const String& path) const
	{
		Ref<AssetInfo> res = nullptr;
		allAssetsByPath.TryGetValue(path, res);
		return res;
	}

	Ref<AssetInfo> AssetsTree::Find(const UID& id) const
	{
		Ref<AssetInfo> res = nullptr;
		allAssetsByUID.TryGetValue(id, res);
		return res;
	}

	Ref<AssetInfo> AssetsTree::AddAsset(const Ref<AssetInfo>& asset)
	{
		int delPos = asset->path.FindLast("/");
		if (delPos < 0 || delPos == asset->path.Length() - 1)
		{
			rootAssets.Add(asset);
			asset->SetTree(Ref(this));
		}
		else
		{
			String parentPath = asset->path.SubStr(0, delPos);
			Ref<AssetInfo> parent = nullptr;
			allAssetsByPath.TryGetValue(parentPath, parent);

			if (!parent)
			{
				if (log)
					log->Out("Failed to add built asset info: " + asset->path);
			}
			else 
				parent->AddChild(asset);

			asset->SetTree(Ref(this));
		}

		return asset;
	}

	void AssetsTree::RemoveAsset(const Ref<AssetInfo>& asset)
	{
		if (allAssetsByPath[asset->path] == asset)
			allAssetsByPath.Remove(asset->path);

		if (asset->meta && allAssetsByUID[asset->meta->ID()] == asset)
			allAssetsByUID.Remove(asset->meta->ID());

		allAssets.Remove(asset);
		rootAssets.Remove(asset);

		if (asset->parent)
			asset->parent.Lock()->RemoveChild(asset);
	}

	void AssetsTree::Clear()
	{
		allAssets.Clear();
		rootAssets.Clear();
		allAssetsByPath.Clear();
		allAssetsByUID.Clear();
	}

	void AssetsTree::LoadFolder(const FolderInfo& folder, Ref<AssetInfo> parentAsset)
	{
		for (auto& fileInfo : folder.files)
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

		for (auto& subFolder : folder.folders)
		{
			Ref<AssetInfo> asset = nullptr;

			String subFolderPath = subFolder.path;
			if (subFolderPath.EndsWith("/"))
				subFolderPath.Erase(subFolderPath.Length() - 1);

			String folderFullPath = assetsPath + subFolderPath;
			String metaFullPath = folderFullPath + ".meta";

			bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
			if (!isExistMetaForFolder)
			{
				if (log)
					log->Warning("Can't load asset info for " + subFolder.path + " - missing meta file");

				continue;
			}

			asset = LoadAssetNode(subFolderPath, parentAsset, TimeStamp());

			LoadFolder(subFolder, asset);
		}
	}

	Ref<AssetInfo> AssetsTree::LoadAssetNode(const String& path, const Ref<AssetInfo>& parent, const TimeStamp& time)
	{
		DataDocument metaData;
		metaData.LoadFromFile(this->assetsPath + path + ".meta");

		Ref<AssetMeta> meta;
		meta = metaData;

		Ref<AssetInfo> asset = mmake<AssetInfo>();

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
		for (auto& asset : rootAssets)
			asset->SetTree(Ref(this));
	}

}

DECLARE_CLASS(o2::AssetsTree);
