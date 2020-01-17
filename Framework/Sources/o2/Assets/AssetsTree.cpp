#include "o2/stdafx.h"
#include "AssetsTree.h"

#include "o2/Assets/FolderAsset.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	AssetsTree::AssetNode::AssetNode():
		meta(nullptr)
	{}

	AssetsTree::AssetNode::AssetNode(const String& path, UID id, const Type* type) :
		AssetInfo(path, id, type)
	{}

	AssetsTree::AssetNode::~AssetNode()
	{
		delete meta;
	}

	AssetsTree::AssetNode* AssetsTree::AssetNode::AddChild(AssetNode* node)
	{
		if (node->parent)
			node->parent->RemoveChild(node, false);

		node->parent = this;

		children.Add(node);

		return node;
	}

	bool AssetsTree::AssetNode::RemoveChild(AssetNode* node, bool release /*= true*/)
	{
		node->parent = nullptr;

		children.Remove(node);

		if (release && node)
			delete node;

		return true;
	}

	void AssetsTree::AssetNode::SetParent(AssetNode* parent)
	{
		if (parent)
		{
			parent->AddChild(this);
		}
		else
		{
			if (parent)
				parent->RemoveChild(this, false);

			parent = nullptr;
		}
	}

	void AssetsTree::AssetNode::OnDeserialized(const DataNode& node)
	{
		AssetInfo::OnDeserialized(node);

		for (auto child : children)
			child->parent = this;
	}

	void AssetsTree::AssetNode::SetTree(AssetsTree* tree)
	{
		this->tree = tree;
		tree->allAssets.Add(this);
		tree->allAssetsByPath[path] = this;
		tree->allAssetsByUID[id] = this;

		for (auto child : children)
			child->SetTree(tree);
	}

	AssetsTree::AssetsTree() :
		log(nullptr)
	{}

	AssetsTree::~AssetsTree()
	{
		Clear();
	}

	void AssetsTree::BuildTree(const String& path)
	{
		this->path = path;

		if (!o2FileSystem.IsFolderExist(path))
			o2FileSystem.FolderCreate(path);

		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(path);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);

		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

	void AssetsTree::RebuildTree()
	{
		FolderInfo folderInfo = o2FileSystem.GetFolderInfo(path);
		folderInfo.ClampPathNames();
		LoadFolder(folderInfo, nullptr);

		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

	void AssetsTree::Sort()
	{
		allAssets.Sort([](AssetNode* a, AssetNode* b) { return a->path.Length() < b->path.Length(); });
	}

	void AssetsTree::SortInverse()
	{
		allAssets.Sort([](AssetNode* a, AssetNode* b) { return a->path.Length() > b->path.Length(); });
	}

	AssetsTree::AssetNode* AssetsTree::FindAsset(const String& path) const
	{
		AssetsTree::AssetNode* res = nullptr;
		allAssetsByPath.TryGetValue(path, res);
		return res;
	}

	AssetsTree::AssetNode* AssetsTree::FindAsset(UID id) const
	{
		AssetsTree::AssetNode* res = nullptr;
		allAssetsByPath.TryGetValue(path, res);
		return res;
	}

	AssetInfo AssetsTree::FindAssetInfo(const String& path) const
	{
		auto asset = FindAsset(path);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	AssetInfo AssetsTree::FindAssetInfo(UID id) const
	{
		auto asset = FindAsset(id);
		if (asset)
			return (AssetInfo)(*asset);

		return AssetInfo();
	}

	AssetsTree::AssetNode* AssetsTree::AddAsset(AssetNode* asset)
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
			AssetNode* parent = nullptr;
			allAssetsByPath.TryGetValue(parentPath, parent);

			if (!parent)
			{
				if (log) 
					log->Out("Failed to add built asset info: " + asset->path);
			}
			else parent->AddChild(asset);

			allAssets.Add(asset);
			asset->SetTree(this);
		}

		return asset;
	}

	void AssetsTree::RemoveAsset(AssetNode* asset, bool release /*= true*/)
	{
		if (allAssetsByPath[asset->path] == asset)
			allAssetsByPath.Remove(asset->path);

		if (allAssetsByUID[asset->id] == asset)
			allAssetsByUID.Remove(asset->id);

		allAssets.Remove(asset);
		rootAssets.Remove(asset);

		if (asset->parent)
			asset->parent->RemoveChild(asset, false);
		
		if (asset->assetType == &TypeOf(FolderAsset) && release)
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

	void AssetsTree::LoadFolder(FolderInfo& folder, AssetNode* parentAsset)
	{
		Vector<AssetNode*> missingAssetNodes;
		auto& parentChilds = parentAsset ? parentAsset->children : rootAssets;
		for (auto assetNode : parentChilds)
		{
			bool exist = false;

			if (assetNode->assetType == &TypeOf(FolderAsset))
				exist = folder.folders.ContainsPred([=](const FolderInfo& x) { return x.path == assetNode->path; });
			else
				exist = folder.files.ContainsPred([=](const FileInfo& x) { return x.path == assetNode->path; });

			if (!exist)
				missingAssetNodes.Add(assetNode);
		}

		for (auto assetNode : missingAssetNodes)
			RemoveAsset(assetNode);

		for (auto fileInfo : folder.files)
		{
			if (parentChilds.ContainsPred([&](AssetNode* x) { return x->path == fileInfo.path; }))
				continue;

			String extension = o2FileSystem.GetFileExtension(fileInfo.path);

			if (extension != "meta")
			{
				String assetFullPath = path + fileInfo.path;
				String metaFullPath = assetFullPath + ".meta";

				bool isExistMetaForAsset = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForAsset)
					continue;

				LoadAssetNode(fileInfo.path, parentAsset, fileInfo.editDate);
			}
		}

		for (auto subFolder : folder.folders)
		{
			AssetNode* asset = nullptr;

			if (!parentChilds.ContainsPred([&](AssetNode* x) { return x->path == subFolder.path; }))
			{
				String folderFullPath = path + subFolder.path;
				String metaFullPath = folderFullPath + ".meta";

				bool isExistMetaForFolder = o2FileSystem.IsFileExist(metaFullPath);
				if (!isExistMetaForFolder)
				{
					if (log)
						log->Warning("Can't load asset info for " + subFolder.path + " - missing meta file");

					continue;
				}

				asset = LoadAssetNode(subFolder.path, parentAsset, TimeStamp());
			}
			else asset = parentChilds.FindMatch([&](AssetNode* x) { return x->path == subFolder.path; });

			LoadFolder(subFolder, asset);
		}
	}

	AssetsTree::AssetNode* AssetsTree::LoadAssetNode(const String& path, AssetNode* parent, const TimeStamp& time)
	{
		DataNode metaData;
		metaData.LoadFromFile(this->path + path + ".meta");

		Asset::IMetaInfo* meta;
		meta = metaData;

		AssetNode* asset = mnew AssetNode();

		asset->meta = meta;
		asset->path = path;
		asset->time = time;
		asset->id = meta->ID();
		asset->assetType = meta->GetAssetType();
		asset->SetParent(parent);

		if (!parent)
			rootAssets.Add(asset);

		return asset;
	}

	void AssetsTree::OnDeserialized(const DataNode& node)
	{
		for (auto asset : rootAssets)
			asset->SetTree(this);
	}

}

DECLARE_CLASS(o2::AssetsTree);

DECLARE_CLASS(o2::AssetsTree::AssetNode);
