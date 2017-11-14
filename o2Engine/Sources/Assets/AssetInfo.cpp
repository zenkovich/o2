#include "AssetInfo.h"

namespace o2
{
	AssetInfo::AssetInfo():
		id(0), assetType(nullptr)
	{}

	AssetInfo::AssetInfo(const String& path, UID id, const Type* type):
		path(path), id(id), assetType(type)
	{}

	void AssetInfo::OnSerialize(DataNode& node) const
	{
		if (assetType)
			node["type"] = assetType->GetName();
	}

	void AssetInfo::OnDeserialized(const DataNode& node)
	{
		if (auto typeNode = node.GetNode("type"))
			assetType = o2Reflection.GetType((String)typeNode);
	}

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		return id == other.id;
	}
}

DECLARE_CLASS(o2::AssetInfo);
