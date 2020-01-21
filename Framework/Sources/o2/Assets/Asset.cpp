#include "o2/stdafx.h"
#include "Asset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	Asset::Asset():
		mMeta(nullptr)
	{}

	Asset::Asset(const Asset& asset):
		path(this), fullPath(this), id(this), meta(this)
	{}

	Asset& Asset::operator=(const Asset& asset)
	{
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		return *this;
	}

	Asset::~Asset()
	{
		delete mMeta;
	}

	AssetInfo Asset::GetAssetInfo() const
	{
		return AssetInfo(mPath, GetAssetId(), &GetType());
	}

	String Asset::GetPath() const
	{
		return mPath;
	}

	void Asset::SetPath(const String& path)
	{
		o2Assets.MoveAsset(GetAssetId(), mPath);
	}

	UID Asset::GetAssetId() const
	{
		return mMeta->mId;
	}

	UID& Asset::IdRef()
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
			GetAssetsLogStream()->Error("Failed to load asset by path (" + mPath + "): asset isn't exist");
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetDataFullPath());
	}

	void Asset::Load(UID id)
	{
		if (!o2Assets.IsAssetExist(id))
		{
			GetAssetsLogStream()->Error("Failed to load asset by id (" + (String)id + "): asset isn't exist");
			return;
		}

		mMeta->mId = id;
		mPath = o2Assets.GetAssetPath(id);

		LoadMeta(GetMetaFullPath());
		LoadData(GetDataFullPath());
	}

	void Asset::Load()
	{
		if (mMeta->mId == 0)
			mMeta->mId = o2Assets.GetAssetId(mPath);

		if (mMeta->mId == 0 || !o2Assets.IsAssetExist(mMeta->mId))
		{
			GetAssetsLogStream()->Error("Failed to load asset (" + mPath + " - " + (String)mMeta->mId + "): isn't exist");
			return;
		}

		LoadMeta(GetMetaFullPath());
		LoadData(GetDataFullPath());
	}

	void Asset::Load(const AssetInfo& info)
	{
		if (info.assetType != &GetType())
		{
			GetAssetsLogStream()->Error("Failed to load asset by info (" + info.path + " - " + (String)info.id + 
										"): incorrect type (" + (String)info.assetType + ")");
			return;
		}

		mMeta->mId = info.id;
		mPath = info.path;

		Load();
	}

	void Asset::Save(const String& path, bool rebuildAssetsImmediately /*= true*/)
	{
		if (path != mPath || IdRef() == 0)
		{
			IdRef() = Assets::GetRandomAssetId();
			mPath = path;
		}

		UID destPathAssetId = o2Assets.GetAssetId(path);
		if (destPathAssetId != 0 && destPathAssetId != mMeta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (" + mPath + " - " + (String)mMeta->mId + 
										") to " + path + ": another asset exist in target path");
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::Save(bool rebuildAssetsImmediately /*= true*/)
	{
		if (IdRef() == 0)
			IdRef() = Assets::GetRandomAssetId();

		UID destPathAssetId = o2Assets.GetAssetId(mPath);
		if (destPathAssetId != 0 && destPathAssetId != mMeta->mId)
		{
			GetAssetsLogStream()->Error("Failed to save asset (" + mPath + " - " + (String)mMeta->mId + 
										"): another asset exist in this path");
			return;
		}

		SaveMeta(GetMetaFullPath());
		SaveData(GetFullPath());

		if (rebuildAssetsImmediately)
			o2Assets.RebuildAssets();
	}

	void Asset::Save(const AssetInfo& info, bool rebuildAssetsImmediately /*= true*/)
	{
		mMeta->mId = info.id;
		mPath = info.path;

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
		return o2Assets.GetBuiltAssetsPath() + mPath;
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
		mId(0)
	{}

	Asset::IMetaInfo::~IMetaInfo()
	{}

	const Type* Asset::IMetaInfo::GetAssetType() const
	{
		return &TypeOf(Asset);
	}

	bool Asset::IMetaInfo::IsEqual(IMetaInfo* other) const
	{
		return GetAssetType() == other->GetAssetType() && mId == other->mId;
	}

	UID Asset::IMetaInfo::ID() const
	{
		return mId;
	}

	void Asset::OnSerialize(DataNode& node) const
	{
		*node.AddNode("path") = mPath;
		*node.AddNode("id") = GetAssetId();
	}

	void Asset::OnDeserialized(const DataNode& node)
	{
		if (auto pathNode = node.GetNode("path"))
			mPath = *pathNode;

		if (auto idNode = node.GetNode("id"))
			idNode->GetValue(IdRef());

		if (IdRef() != 0 || !mPath.IsEmpty())
			Load();
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

	AssetRef::AssetRef():
		mAssetPtr(nullptr), mRefCounter(nullptr)
	{}

	AssetRef::AssetRef(const AssetRef& other) :
		mAssetPtr(other.mAssetPtr), mRefCounter(other.mRefCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	AssetRef::AssetRef(const String& path):
		AssetRef(o2Assets.GetAssetRef(path))
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	AssetRef::AssetRef(UID id):
		AssetRef(o2Assets.GetAssetRef(id))
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	AssetRef::AssetRef(Asset* assetPtr, int* refCounter):
		mAssetPtr(assetPtr), mRefCounter(refCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	void AssetRef::OnSerialize(DataNode& node) const
	{
		if (mAssetPtr)
		{
			node["id"] = mAssetPtr->GetAssetId().ToString();
			node["path"] = mAssetPtr->GetPath();
		}
	}

	void AssetRef::OnDeserialized(const DataNode& node)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = nullptr;
		mRefCounter = nullptr;

		if (auto idNode = node.GetNode("id"))
		{
			UID id; idNode->GetValue(id);
			*this = o2Assets.GetAssetRef(id);
		}
		else if (auto pathNode = node.GetNode("path"))
		{
			String id;
			idNode->GetValue(id);
			*this = o2Assets.GetAssetRef(id);
		}
	}

	AssetRef::~AssetRef()
	{
		if (mAssetPtr)
			(*mRefCounter)--;
	}

	AssetRef& AssetRef::operator=(const AssetRef& other)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = other.mAssetPtr;
		mRefCounter = other.mRefCounter;

		if (mAssetPtr)
			(*mRefCounter)++;

		return *this;
	}

	AssetRef::operator bool() const
	{
		return mAssetPtr != nullptr;
	}

	Asset& AssetRef::operator*()
	{
		return *mAssetPtr;
	}

	const Asset& AssetRef::operator*() const
	{
		return *mAssetPtr;
	}

	Asset* AssetRef::operator->()
	{
		return mAssetPtr;
	}

	const Asset* AssetRef::operator->() const
	{
		return mAssetPtr;
	}

	bool AssetRef::IsValid() const
	{
		return mAssetPtr != nullptr;
	}

	const Type& AssetRef::GetAssetType() const
	{
		return TypeOf(Asset);
	}

	bool AssetRef::operator!=(const AssetRef& other) const
	{
		return mAssetPtr != other.mAssetPtr;
	}

	bool AssetRef::operator==(const AssetRef& other) const
	{
		return mAssetPtr == other.mAssetPtr;
	}

}

DECLARE_CLASS(o2::Asset);

DECLARE_CLASS(o2::AssetRef);

DECLARE_CLASS(o2::Asset::IMetaInfo);