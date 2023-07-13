#include "o2/stdafx.h"
#include "AssetInfo.h"

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetsTree.h"

namespace o2
{
	AssetInfo::AssetInfo()
	{
		if (Assets::IsSingletonInitialzed())
			tree = &o2Assets.GetAssetsTree();
	}

	AssetInfo::AssetInfo(const AssetInfo& other):
		path(other.path), editTime(other.editTime), tree(other.tree), 
		meta(other.meta ? other.meta->CloneAs<AssetMeta>() : nullptr),
		mOwnChildren(false), mChildren(other.mChildren)
	{}

	AssetInfo::AssetInfo(AssetMeta* meta):
		meta(meta)
	{
		if (Assets::IsSingletonInitialzed())
			tree = &o2Assets.GetAssetsTree();
	}

	AssetInfo::~AssetInfo()
	{
		if (meta)
			delete meta;

		if (parent)
			parent->RemoveChild(this, false);

		if (mOwnChildren)
		{
			for (auto child : mChildren)
			{
				child->parent = nullptr;
				delete child;
			}
		}
	}

	AssetInfo& AssetInfo::operator=(const AssetInfo& other)
	{
		if (mOwnChildren)
		{
			for (auto child : mChildren)
			{
				child->parent = nullptr;
				delete child;
			}
		}

		meta = other.meta ? other.meta->CloneAs<AssetMeta>() : nullptr;
		path = other.path;
		editTime = other.editTime;
		tree = other.tree;
		mChildren = other.mChildren;
		mOwnChildren = false;

		return *this;
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

		mChildren.Add(node);

		return node;
	}

	void AssetInfo::RemoveChild(AssetInfo* node, bool release /*= true*/)
	{
		node->parent = nullptr;

		mChildren.Remove(node);

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

	void AssetInfo::OnDeserialized(const DataValue& node)
	{
		for (auto child : mChildren)
			child->parent = this;
	}

	void AssetInfo::SetTree(AssetsTree* tree)
	{
		this->tree = tree;
		tree->allAssets.Add(this);
		tree->allAssetsByPath[path] = this;

		if (meta)
			tree->allAssetsByUID[meta->ID()] = this;

		for (auto child : mChildren)
			child->SetTree(tree);
	}

	const Vector<AssetInfo*>& AssetInfo::GetChildren() const
	{
		if (mOwnChildren)
			return mChildren;

		if (tree)
		{
			AssetInfo* sameInTree = nullptr;
			if (tree->allAssetsByUID.TryGetValue(meta->ID(), sameInTree))
				return sameInTree->GetChildren();
		}

		return mChildren;
	}

	bool AssetInfo::IsValid() const
	{
		return meta && meta->ID() != UID::empty;
	}

	AssetInfo AssetInfo::empty = AssetInfo(mnew AssetMeta());

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		if (meta && other.meta)
			return meta->ID() == other.meta->ID();

		return path == other.path;
	}
}
// --- META ---

DECLARE_CLASS(o2::AssetInfo);
// --- END META ---
