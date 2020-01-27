#include "o2/stdafx.h"
#include "Asset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Assets/AssetInfo.h"

namespace o2
{
	Asset::Asset(const Asset& other):
		mInfo(other.mInfo)
	{}

	Asset& Asset::operator=(const Asset& other)
	{
		mInfo = other.mInfo;
		return *this;
	}

	Asset::~Asset()
	{}

	const String& Asset::GetPath() const
	{
		return mInfo.path;
	}

	void Asset::SetPath(const String& path)
	{
		mInfo.path = path;
		mInfo.meta->mId.Randomize();
	}

	const UID& Asset::GetAssetId() const
	{
		return mInfo.meta->ID();
	}

	UID& Asset::ID()
	{
		return mInfo.meta->mId;
	}

	AssetMeta* Asset::GetMeta() const
	{
		return mInfo.meta;
	}

	void Asset::Load(const String& path)
	{
		mInfo = o2Assets.GetAssetInfo(path);

		if (mInfo.meta->mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by path (" + path + "): asset isn't exist");
			return;
		}

		DataNode data;
		data.LoadFromFile(GetFullPath());
		Deserialize(data);
	}

	void Asset::Load(const UID& id)
	{
		mInfo = o2Assets.GetAssetInfo(path);

		if (mInfo.meta->mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by UID (" + id + "): asset isn't exist");
			return;
		}

		DataNode data;
		data.LoadFromFile(GetFullPath());
		Deserialize(data);
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		SetPath(path);
		Save(rebuildAssetsImmediately);
	}

	void Asset::Save(bool rebuildAssetsImmediately /*= true*/)
	{
		if (ID() == 0)
			ID().Randomize();

		UID destPathAssetId = o2Assets.GetAssetId(mInfo.path);
		if (destPathAssetId != 0 && destPathAssetId != mInfo.meta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (" + mInfo.path + " - " + (String)mInfo.meta->mId +
										"): another asset exist in this path");
			return;
		}

		DataNode metaData;
		metaData = mInfo.meta;
		metaData.SaveToFile(GetMetaFullPath());

		DataNode data;
		data.LoadFromFile(GetFullPath());
		Deserialize(data);

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	const char* Asset::GetFileExtensions() const
	{
		return "";
	}

	String Asset::GetFullPath() const
	{
		return (mInfo.tree ? mInfo.tree->assetsPath : String()) + mInfo.path;
	}

	String Asset::GetDataFullPath() const
	{
		return (mInfo.tree ? mInfo.tree->builtAssetsPath : String()) + mInfo.path;
	}

	String Asset::GetMetaFullPath() const
	{
		return GetFullPath() + ".meta";
	}

	LogStream* Asset::GetAssetsLogStream() const
	{
		return o2Assets.mLog;
	}
}

DECLARE_CLASS(o2::Asset);
