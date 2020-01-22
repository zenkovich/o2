#include "o2/stdafx.h"
#include "AssetInfo.h"

#include "AssetsTree.h"

namespace o2
{
	AssetInfo::AssetInfo()
	{}

	AssetInfo::AssetInfo(const AssetInfo& other):
		mAssetType(other.mAssetType), mPath(other.mPath), mEditTime(other.mEditTime), 
		mMeta(other.mMeta ? other.mMeta->CloneAs<Asset::IMetaInfo>() : nullptr)
	{
		for (auto child : other.mChildren)
		{
			auto newChild = child->CloneAs<AssetInfo>();
			newChild->mParent = this;
			mChildren.Add(newChild);
		}
	}

	AssetInfo::~AssetInfo()
	{
		if (mMeta)
			delete mMeta;

		if (mParent)
			mParent->RemoveChild(this, false);

		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			delete child;
		}
	}

	AssetInfo& AssetInfo::operator=(const AssetInfo& other)
	{
		if (mMeta)
			delete mMeta;
	}

	AssetInfo::operator bool() const
	{
		return IsValid();
	}

	AssetInfo* AssetInfo::AddChild(AssetInfo* node)
	{
		if (node->mParent)
			node->mParent->RemoveChild(node, false);

		node->mParent = this;

		mChildren.Add(node);

		return node;
	}

	void AssetInfo::RemoveChild(AssetInfo* node, bool release /*= true*/)
	{
		node->mParent = nullptr;

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

	void AssetInfo::OnSerialize(DataNode& node) const
	{
		if (mAssetType)
			node["type"] = mAssetType->GetName();
	}

	void AssetInfo::OnDeserialized(const DataNode& node)
	{
		if (auto typeNode = node.GetNode("type"))
			mAssetType = o2Reflection.GetType(typeNode->Data());

		for (auto child : mChildren)
			child->mParent = this;
	}

	void AssetInfo::SetTree(AssetsTree* tree)
	{
		this->mTree = tree;
		tree->allAssets.Add(this);
		tree->allAssetsByPath[mPath] = this;

		if (mMeta)
			tree->allAssetsByUID[mMeta->ID()] = this;

		for (auto child : mChildren)
			child->SetTree(tree);
	}

	bool AssetInfo::IsValid() const
	{
		return mMeta && mMeta->ID() != UID::empty;
	}

	AssetInfo AssetInfo::empty;

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		if (mMeta && other.mMeta)
			return mMeta->ID() == other.mMeta->ID();

		return mPath == other.mPath;
	}
}

DECLARE_CLASS(o2::AssetInfo);
