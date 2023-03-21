#include "o2/stdafx.h"
#include "Meta.h"

#include "o2/Assets/Asset.h"

namespace o2
{
	AssetMeta::AssetMeta() :
		mId(0)
	{}

	AssetMeta::~AssetMeta()
	{}

	const Type* AssetMeta::GetAssetType() const
	{
		return &TypeOf(Asset);
	}

	bool AssetMeta::IsEqual(const AssetMeta& other) const
	{
		return GetAssetType() == other.GetAssetType() && mId == other.mId;
	}

	const UID& AssetMeta::ID() const
	{
		return mId;
	}
}

DECLARE_CLASS(o2::AssetMeta);
