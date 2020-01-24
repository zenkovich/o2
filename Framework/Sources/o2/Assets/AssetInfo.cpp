#include "o2/stdafx.h"
#include "AssetInfo.h"

#include "AssetsTree.h"

namespace o2
{
	AssetInfo::AssetInfo()
	{}

	AssetInfo::AssetInfo(const AssetInfo& other):
		assetType(other.assetType), path(other.path), editTime(other.editTime), 
		meta(other.meta ? other.meta->CloneAs<AssetMeta>() : nullptr)
	{
		for (auto child : other.children)
		{
			auto newChild = child->CloneAs<AssetInfo>();
			newChild->parent = this;
			children.Add(newChild);
		}
	}

	AssetInfo::~AssetInfo()
	{
		if (meta)
			delete meta;

		if (parent)
			parent->RemoveChild(this, false);

		for (auto child : children)
		{
			child->parent = nullptr;
			delete child;
		}
	}

	AssetInfo& AssetInfo::operator=(const AssetInfo& other)
	{
		if (meta)
			delete meta;
	}

	AssetInfo::operator bool() const
	{
		return IsValid();
	}

	AssetInfo* AssetInfo::AddChild(AssetInfo* node)
	{
		if (node->parent)
			node->parent->RemoveChild(node, false);

		node->parent = this;

		children.Add(node);

		return node;
	}

	void AssetInfo::RemoveChild(AssetInfo* node, bool release /*= true*/)
	{
		node->parent = nullptr;

		children.Remove(node);

		if (release && node)
			delete node;
	}

	void AssetInfo::SetParent(AssetInfo* parent)
	{
		if (parent)
			parent->AddChild(this);
		else
		{
			if (parent)
				parent->RemoveChild(this, false);

			parent = nullptr;
		}
	}

	void AssetInfo::OnSerialize(DataNode& node) const
	{
		if (assetType)
			node["type"] = assetType->GetName();
	}

	void AssetInfo::OnDeserialized(const DataNode& node)
	{
		if (auto typeNode = node.GetNode("type"))
			assetType = o2Reflection.GetType(typeNode->Data());

		for (auto child : children)
			child->parent = this;
	}

	void AssetInfo::SetTree(AssetsTree* tree)
	{
		this->tree = tree;
		tree->allAssets.Add(this);
		tree->allAssetsByPath[path] = this;

		if (meta)
			tree->allAssetsByUID[meta->ID()] = this;

		for (auto child : children)
			child->SetTree(tree);
	}

	bool AssetInfo::IsValid() const
	{
		return meta && meta->ID() != UID::empty;
	}

	AssetInfo AssetInfo::empty;

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		if (meta && other.meta)
			return meta->ID() == other.meta->ID();

		return path == other.path;
	}
}

DECLARE_CLASS(o2::AssetInfo);
