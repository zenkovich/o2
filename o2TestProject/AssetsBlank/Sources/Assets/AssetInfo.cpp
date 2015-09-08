#include "AssetInfo.h"

namespace o2
{
	IOBJECT_CPP(AssetInfo);

	AssetInfo::AssetInfo():
		mId(0), mType(0)
	{}

	AssetInfo::AssetInfo(const String& path, UInt id, Type::Id type):
		mPath(path), mId(id), mType(type)
	{}

	AssetInfo AssetInfo::FindAssetInfo(const String& path) const
	{
		for (auto info : mChilds)
		{
			if (info.mPath == path)
				return info;

			AssetInfo childSearch = info.FindAssetInfo(path);
			if (childSearch.mId != 0)
				return childSearch;
		}

		return AssetInfo();
	}

	AssetInfo AssetInfo::FindAssetInfo(UInt id) const
	{
		for (auto info : mChilds)
		{
			if (info.mId == id)
				return info;

			AssetInfo childSearch = info.FindAssetInfo(id);
			if (childSearch.mId != 0)
				return childSearch;
		}

		return AssetInfo();
	}

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		return mId == other.mId;
	}
}