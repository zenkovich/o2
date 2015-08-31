#include "Asset.h"

#include "Assets/Assets.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	REGIST_TYPE(Asset);

	IOBJECT_INH_CPP(IMetaInfo, Asset);

	Asset::Asset()
	{}

	Asset::Asset(const String& path):
		mPath(path)
	{
		InitializeProperties();

		mId = o2Assets.GetAssetId(path);
	}

	Asset::Asset(UInt id):
		mId(id)
	{
		InitializeProperties();

		mPath = o2Assets.GetAssetPath(id);
	}

	Asset::Asset(const Asset& asset)
	{
		InitializeProperties();

		GenerateId();
	}

	Asset& Asset::operator=(const Asset& asset)
	{
		return *this;
	}

	Asset::~Asset()
	{}

	String Asset::GetPath() const
	{
		return mPath;
	}

	void Asset::SetPath(const String& path)
	{
		o2Assets.MoveAsset(Ptr<Asset>(this), mPath);
	}

	UInt Asset::GetId() const
	{
		return mId;
	}

	Ptr<Asset::IMetaInfo> Asset::GetMeta() const
	{
		return mMeta;
	}

	void Asset::Load(const String& path)
	{
		mPath = path;
		mId = o2Assets.GetAssetId(path);

		if (mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by path (%s): asset isn't exist", mPath);
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetFullPath());
	}

	void Asset::Load(UInt id)
	{
		if (!o2Assets.IsAssetExist(id))
		{
			GetAssetsLogStream()->Error("Failed to load asset by id (%ui): asset isn't exist", id);
			return;
		}

		mId = id;
		mPath = o2Assets.GetAssetPath(id);

		LoadMeta(GetMetaFullPath());
		LoadData(GetFullPath());
	}

	void Asset::Load()
	{
		if (mId == 0)
			mId = o2Assets.GetAssetId(mPath);

		if (mId == 0 || !o2Assets.IsAssetExist(mId))
		{
			GetAssetsLogStream()->Error("Failed to load asset (%s - %ui): isn't exist", mPath, mId);
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetFullPath());
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		if (path != mPath)
			GenerateId();

		UInt destPathAssetId = o2Assets.GetAssetId(path);
		if (destPathAssetId != 0 && destPathAssetId != mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (%s - %ui) to %s: another asset exist in target path", mPath, mId, path);
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::Save(bool rebuildAssetsImmediately /*= true*/)
	{
		UInt destPathAssetId = o2Assets.GetAssetId(mPath);
		if (destPathAssetId != 0 && destPathAssetId != mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (%s - %ui): another asset exist in this path", mPath, mId);
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::GenerateId()
	{
		mId = Math::Random<UInt>(1, UINT_MAX);
	}

	String Asset::GetFullPath() const
	{
		return o2Assets.GetAssetsPath() + mPath;
	}

	String Asset::GetMetaFullPath() const
	{
		return GetFullPath() + ".meta";
	}

	Ptr<LogStream> Asset::GetAssetsLogStream() const
	{
		return o2Assets.mLog;
	}

	TypeId Asset::IMetaInfo::GetAssetType() const
	{
		return Asset::GetStaticType();
	}

	bool Asset::IMetaInfo::IsEqual(Ptr<IMetaInfo> other) const
	{
		return GetAssetType() == other->GetAssetType();
	}

	void Asset::LoadMeta(const String& path)
	{
		DataNode metaData;
		metaData = mMeta;
		metaData.SaveToFile(path);
	}

	void Asset::SaveMeta(const String& path)
	{
		DataNode metaData;
		metaData.LoadFromFile(path);
		mMeta = metaData;
	}

	void Asset::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Asset, Path, SetPath, GetPath);
		INITIALIZE_GETTER(Asset, Id, GetId);
		INITIALIZE_GETTER(Asset, FullPath, GetFullPath);
		INITIALIZE_GETTER(Asset, AssetType, GetType);
		INITIALIZE_GETTER(Asset, Meta, GetMeta);
	}
}