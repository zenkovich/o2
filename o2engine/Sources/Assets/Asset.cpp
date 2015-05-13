#include "Asset.h"

namespace o2
{
	REGIST_TYPE(Asset);

	Asset::Asset()
	{
	}

	Asset::Asset(const String& location)
	{
		//mLocation = AppAssets()->GetAssetFileLocation(location);
	}

	Asset::Asset(const FileLocation& location):
		mLocation(location)
	{
	}

	Asset::Asset(const Asset& asset):
		mLocation()
	{
	}

	Asset::~Asset()
	{
	}

	Asset& Asset::operator=(const Asset& asset)
	{
		return *this;
	}

	FileLocation Asset::GetLocation() const
	{
		return mLocation;
	}

	String Asset::GetPath() const
	{
		return mLocation.mPath;
	}

	UInt Asset::GetFileId() const
	{
		return mLocation.mId;
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		if (mLocation.mPath == path)
			return;

		mLocation.mPath = path;
		//mLocation.mId = AppAssets()->GenerateFileId();

		SaveData();

		if (rebuildAssetsImmediately)
			AppAssets()->RebuildAssets();
	}


	AssetInfo::AssetInfo():
		mType(Type::File)
	{
	}

	AssetInfo::AssetInfo(const FileLocation& location, Type type, const WideTime& writeTime):
		mLocation(location), mType(type), mWriteTime(writeTime)
	{
	}

	SERIALIZE_METHOD_IMPL(AssetInfo)
	{
		SERIALIZE_ID(&mLocation, "location");

		int type = (int)mType;
		SERIALIZE_ID(type, "type");
		mType = (Type)type;

		SERIALIZE_ID(mWriteTime, "writeTime");

		return true;
	}

	bool AssetInfo::operator==(const AssetInfo& other) const
	{
		return mLocation == other.mLocation && mWriteTime == other.mWriteTime && mType == other.mType;
	}
}