#include "AssetInfo.h"

namespace o2
{
	AssetInfo::AssetInfo():
		mId(0), mType(0)
	{}

	AssetInfo::AssetInfo(const String& path, AssetId id, Type::Id type):
		mPath(path), mId(id), mType(type)
	{}

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		return mId == other.mId;
	}
}