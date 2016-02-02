#include "Asset.h"

#include "Assets/Assets.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	Asset::Asset():
		mMeta(nullptr)
	{
		InitializeProperties();
	}

	Asset::Asset(const Asset& asset)
	{
		InitializeProperties();
	}

	Asset& Asset::operator=(const Asset& asset)
	{
		return *this;
	}

	Asset::~Asset()
	{
		delete mMeta;
	}

	AssetInfo Asset::GetAssetInfo() const
	{
		return AssetInfo(mPath, GetAssetId(), GetType().ID());
	}

	String Asset::GetPath() const
	{
		return mPath;
	}

	void Asset::SetPath(const String& path)
	{
		o2Assets.MoveAsset(this, mPath);
	}

	AssetId Asset::GetAssetId() const
	{
		return mMeta->mId;
	}

	AssetId& Asset::IdRef()
	{
		return mMeta->mId;
	}

	Asset::IMetaInfo* Asset::GetMeta() const
	{
		return mMeta;
	}

	void Asset::Load(const String& path)
	{
		mPath = path;
		mMeta->mId = o2Assets.GetAssetId(path);

		if (mMeta->mId == 0)
		{
			GetAssetsLogStream()->Error("Failed to load asset by path (%s): asset isn't exist", mPath);
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetFullPath());
	}

	void Asset::Load(AssetId id)
	{
		if (!o2Assets.IsAssetExist(id))
		{
			GetAssetsLogStream()->Error("Failed to load asset by id (%ui): asset isn't exist", id);
			return;
		}

		mMeta->mId = id;
		mPath = o2Assets.GetAssetPath(id);

		LoadMeta(GetMetaFullPath());
		LoadData(GetFullPath());
	}

	void Asset::Load()
	{
		if (mMeta->mId == 0)
			mMeta->mId = o2Assets.GetAssetId(mPath);

		if (mMeta->mId == 0 || !o2Assets.IsAssetExist(mMeta->mId))
		{
			GetAssetsLogStream()->Error("Failed to load asset (%s - %ui): isn't exist", mPath, mMeta->mId);
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetDataFullPath());
	}

	void Asset::Load(const AssetInfo& info)
	{
		if (info.mType != type.ID())
		{
			GetAssetsLogStream()->Error("Failed to load asset by info (%s - %i): incorrect type (%i)",
										info.mPath, info.mId, info.mType);
			return;
		}

		mMeta->mId = info.mId;
		mPath = info.mPath;

		Load();
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		if (path != mPath)
		{
			IdRef() = Assets::GetRandomAssetId();
			mPath = path;
		}

		AssetId destPathAssetId = o2Assets.GetAssetId(path);
		if (destPathAssetId != 0 && destPathAssetId != mMeta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (%s - %ui) to %s: another asset exist in target path", 
										mPath, mMeta->mId, path);
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::Save(bool rebuildAssetsImmediately /*= true*/)
	{
		AssetId destPathAssetId = o2Assets.GetAssetId(mPath);
		if (destPathAssetId != 0 && destPathAssetId != mMeta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (%s - %ui): another asset exist in this path", mPath, mMeta->mId);
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::Save(const AssetInfo& info, bool rebuildAssetsImmediately /*= true*/)
	{
		mMeta->mId = info.mId;
		mPath = info.mPath;

		Save(rebuildAssetsImmediately);
	}

	const char* Asset::GetFileExtensions() const
	{
		return "";
	}

	String Asset::GetFullPath() const
	{
		return o2Assets.GetAssetsPath() + mPath;
	}

	String Asset::GetDataFullPath() const
	{
		return o2Assets.GetDataPath() + mPath;
	}

	String Asset::GetMetaFullPath() const
	{
		return GetFullPath() + ".meta";
	}

	LogStream* Asset::GetAssetsLogStream() const
	{
		return o2Assets.mLog;
	}

	Asset::IMetaInfo::IMetaInfo():
		mId(Assets::GetRandomAssetId())
	{
	}

	Asset::IMetaInfo::~IMetaInfo()
	{
	}

	Type::Id Asset::IMetaInfo::GetAssetType() const
	{
		return Asset::type.ID();
	}

	bool Asset::IMetaInfo::IsEqual(IMetaInfo* other) const
	{
		return GetAssetType() == other->GetAssetType() && mId == other->mId;
	}

	AssetId Asset::IMetaInfo::ID() const
	{
 		return mId;
	}

	void Asset::LoadMeta(const String& path)
	{
		DataNode metaData;
		metaData.LoadFromFile(path);
		delete mMeta;
		mMeta = metaData;
	}

	void Asset::SaveMeta(const String& path)
	{
		DataNode metaData;
		metaData = mMeta;
		metaData.SaveToFile(path);
	}

	void Asset::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Asset, path, SetPath, GetPath);
		INITIALIZE_GETTER(Asset, id, GetAssetId);
		INITIALIZE_GETTER(Asset, fullPath, GetFullPath);
		INITIALIZE_GETTER(Asset, meta, GetMeta);
	}
}