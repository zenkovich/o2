#include "Assets.h"

#include "Assets/Asset.h"
#include "Assets/Builder/AssetsBuilder.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(Assets);

	IOBJECT_INH_CPP(AssetInfo, Assets);

	Assets::Assets()
	{
		InitializeProperties();

		mLog = mnew LogStream("Assets");
		Debug.GetLog()->BindStream(mLog);

		mAssetsBuilder = mnew AssetsBuilder();

		if (ASSETS_PREBUILDING_ENABLE)
			RebuildAssets();
		else
			LoadAssetsInfos();
	}

	Assets::~Assets()
	{
		mAssetsBuilder.Release();
	}

	String Assets::GetAssetsPath() const
	{
		return ASSETS_PATH;
	}

	String Assets::GetDataPath() const
	{
		return DATA_PATH;
	}

	String Assets::GetAssetPath(UInt id) const
	{
		return FindAssetInfo(id).mPath;
	}

	UInt Assets::GetAssetId(const String& path) const
	{
		return FindAssetInfo(path).mId;
	}

	bool Assets::IsAssetExist(const String& path) const
	{
		return FindAssetInfo(path).mId != 0;
	}

	bool Assets::IsAssetExist(UInt id) const
	{
		return FindAssetInfo(id).mId != 0;
	}

	bool Assets::RemoveAsset(Ptr<Asset> asset, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(asset->GetId());
	}

	bool Assets::RemoveAsset(const String& path, bool rebuildAssets /*= true*/)
	{
		AssetInfo info = FindAssetInfo(path);

		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by path (%s) - asset isn't exist", (char*)path);
			return false;
		}

		return o2FileSystem.RemoveFile(GetAssetsPath() + info.mPath);
	}

	bool Assets::RemoveAsset(UInt id, bool rebuildAssets /*= true*/)
	{
		AssetInfo info = FindAssetInfo(id);

		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by id (%i) - asset isn't exist", id);
			return false;
		}

		return o2FileSystem.RemoveFile(GetAssetsPath() + info.mPath);
	}

	bool Assets::MoveAsset(Ptr<Asset> asset, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(asset->GetId(), newPath);
	}

	bool Assets::MoveAsset(const String& path, const String& newPath, bool rebuildAssets /*= true*/)
	{
		AssetInfo info = FindAssetInfo(path);

		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by path (%s) - asset isn't exist", (char*)path);
			return false;
		}

		if (GetAssetId(newPath) != 0)
		{
			mLog->Error("Can't remove asset by path (%s) \nto new path (%s)\n - another asset exist in target path", 
						(char*)path, (char*)newPath);
			return false;
		}

		return o2FileSystem.MoveFile(GetAssetsPath() + info.mPath, GetAssetsPath() + newPath);
	}

	bool Assets::MoveAsset(UInt id, const String& newPath, bool rebuildAssets /*= true*/)
	{
		AssetInfo info = FindAssetInfo(id);

		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by id (%i) - asset isn't exist", id);
			return false;
		}

		if (GetAssetId(newPath) != 0)
		{
			mLog->Error("Can't remove asset by id (%s) \nto new path (%s)\n - another asset exist in target path",
						id, (char*)newPath);
			return false;
		}

		return o2FileSystem.MoveFile(GetAssetsPath() + info.mPath, GetAssetsPath() + newPath);
	}

	void Assets::RebuildAssets()
	{
		mAssetsBuilder->BuildAssets(GetAssetsPath(), GetDataPath());
		LoadAssetsInfos();
	}

	void Assets::LoadAssetsInfos()
	{
		String buildedAssetsPath = (String)DATA_PATH + "AssetsInfo.xml";
		DataNode data;
		data.LoadFromFile(buildedAssetsPath);
		mAssetsInfos = data;
	}

	void Assets::LoadAssetTypes()
	{
	}

	Assets::AssetInfo Assets::FindAssetInfo(const String& path) const
	{
		for (auto info : mAssetsInfos)
		{
			if (info.mPath == path)
				return info;

			AssetInfo childSearch = info.FindAssetInfo(path);
			if (childSearch.mId != 0)
				return childSearch;
		}

		return AssetInfo();
	}

	Assets::AssetInfo Assets::FindAssetInfo(UInt id) const
	{
		for (auto info : mAssetsInfos)
		{
			if (info.mId == id)
				return info;

			AssetInfo childSearch = info.FindAssetInfo(id);
			if (childSearch.mId != 0)
				return childSearch;
		}

		return AssetInfo();
	}

	void Assets::InitializeProperties()
	{
		INITIALIZE_GETTER(Assets, AssetsPath, GetAssetsPath);
	}

	Assets::AssetInfo::AssetInfo():
		mId(0), mType(0)
	{}

	Assets::AssetInfo::AssetInfo(const String& path, UInt id, TypeId type):
		mPath(path), mId(id), mType(type)
	{}

	Assets::AssetInfo Assets::AssetInfo::FindAssetInfo(const String& path) const
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

	Assets::AssetInfo Assets::AssetInfo::FindAssetInfo(UInt id) const
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

	bool Assets::AssetInfo::operator==(const AssetInfo& other) const
	{
		return mId == other.mId;
	}

}