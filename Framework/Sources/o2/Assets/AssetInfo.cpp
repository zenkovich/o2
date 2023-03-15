#include "o2/stdafx.h"
#include "AssetInfo.h"

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetsTree.h"

namespace o2
{
	AssetInfo::AssetInfo()
	{
		if (Assets::IsSingletonInitialzed())
			tree = Ref(&o2Assets.GetAssetsTree());
	}

	AssetInfo::AssetInfo(const AssetInfo& other):
		path(other.path), editTime(other.editTime), tree(other.tree), 
		meta(other.meta ? other.meta->CloneAs<AssetMeta>() : nullptr),
		mChildren(other.mChildren)
	{}

	AssetInfo::AssetInfo(const Ref<AssetMeta>& meta):
		meta(meta)
	{
		if (Assets::IsSingletonInitialzed())
			tree = Ref(&o2Assets.GetAssetsTree());
	}

	AssetInfo::~AssetInfo()
	{
		if (parent)
			parent.Lock()->RemoveChild(Ref(this));
	}

	AssetInfo& AssetInfo::operator=(const AssetInfo& other)
	{
		mChildren.Clear();

		meta = other.meta ? Ref(other.meta->CloneAs<AssetMeta>()) : nullptr;

		path = other.path;
		editTime = other.editTime;
		tree = other.tree;
		mChildren = other.mChildren;

		return *this;
	}

	AssetInfo::operator bool() const
	{
		return IsValid();
	}

	const Ref<AssetInfo>& AssetInfo::AddChild(const Ref<AssetInfo>& node)
	{
		if (node->parent)
			node->parent.Lock()->RemoveChild(node);

		node->parent = Ref(this);

		mChildren.Add(node);

		return node;
	}

	void AssetInfo::RemoveChild(const Ref<AssetInfo>& node)
	{
		node->parent = nullptr;

		mChildren.Remove(node);
	}

	void AssetInfo::SetParent(const Ref<AssetInfo>& parent)
	{
		if (!parent)
			return;

		parent->AddChild(Ref(this));
	}

	void AssetInfo::OnDeserialized(const DataValue& node)
	{
		for (auto child : mChildren)
			child->parent = this;
	}

	void AssetInfo::SetTree(const Ref<AssetsTree>& tree)
	{
		this->tree = tree;
		tree->allAssets.Add(Ref(this));
		tree->allAssetsByPath[path] = Ref(this);

		if (meta)
			tree->allAssetsByUID[meta->ID()] = Ref(this);

		for (auto child : mChildren)
			child->SetTree(tree);
	}

	const Vector<Ref<AssetInfo>>& AssetInfo::GetChildren() const
	{
		return mChildren;
	}

	bool AssetInfo::IsValid() const
	{
		return meta && meta->ID() != UID::empty;
	}

	AssetInfo AssetInfo::empty = AssetInfo(mmake<AssetMeta>());

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		if (meta && other.meta)
			return meta->ID() == other.meta->ID();

		return path == other.path;
	}
}

DECLARE_CLASS(o2::AssetInfo);
