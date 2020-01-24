#include "o2/stdafx.h"
#include "Asset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Assets/AssetInfo.h"

namespace o2
{
	Asset::Asset()
	{}

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
	}

	const UID& Asset::GetAssetId() const
	{
		return mInfo.meta->ID();
	}

	UID& Asset::ID()
	{
		return mMeta->mId;
	}

	AssetMeta* Asset::GetMeta() const
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

	void Asset::Load(const UID& id)
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
		if (path != mPath || ID() == 0)
		{
			ID().Randomize();
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
		if (ID() == 0)
			ID().Randomize();

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

	void Asset::OnSerialize(DataNode& node) const
	{
	}

	void Asset::OnDeserialized(const DataNode& node)
	{
		if (auto pathNode = node.GetNode("path"))
			mPath = *pathNode;

		if (auto idNode = node.GetNode("id"))
			idNode->GetValue(ID());

		if (ID() != 0 || !mPath.IsEmpty())
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

	IAssetRef::IAssetRef():
		mAssetPtr(nullptr), mRefCounter(nullptr)
	{}

	IAssetRef::IAssetRef(const IAssetRef& other) :
		mAssetPtr(other.mAssetPtr), mRefCounter(other.mRefCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	IAssetRef::IAssetRef(const String& path):
		IAssetRef(o2Assets.GetAssetRef(path))
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	IAssetRef::IAssetRef(const UID& id):
		IAssetRef(o2Assets.GetAssetRef(id))
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	IAssetRef::IAssetRef(Asset* assetPtr, int* refCounter):
		mAssetPtr(assetPtr), mRefCounter(refCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	void IAssetRef::OnSerialize(DataNode& node) const
	{
		if (mAssetPtr)
		{
			node["id"] = mAssetPtr->GetAssetId().ToString();
			node["path"] = mAssetPtr->GetPath();
		}
	}

	void IAssetRef::OnDeserialized(const DataNode& node)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = nullptr;
		mRefCounter = nullptr;

		if (auto idNode = node.GetNode("id"))
		{
			*this = o2Assets.GetAssetRef((UID)(*idNode));
		}
		else if (auto pathNode = node.GetNode("path"))
		{
			String id;
			idNode->GetValue(id);
			*this = o2Assets.GetAssetRef(id);
		}
	}

	IAssetRef::~IAssetRef()
	{
		if (mAssetPtr)
			(*mRefCounter)--;
	}

	IAssetRef& IAssetRef::operator=(const IAssetRef& other)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = other.mAssetPtr;
		mRefCounter = other.mRefCounter;

		if (mAssetPtr)
			(*mRefCounter)++;

		return *this;
	}

	IAssetRef::operator bool() const
	{
		return mAssetPtr != nullptr;
	}

	Asset& IAssetRef::operator*()
	{
		return *mAssetPtr;
	}

	const Asset& IAssetRef::operator*() const
	{
		return *mAssetPtr;
	}

	Asset* IAssetRef::operator->()
	{
		return mAssetPtr;
	}

	const Asset* IAssetRef::operator->() const
	{
		return mAssetPtr;
	}

	bool IAssetRef::IsValid() const
	{
		return mAssetPtr != nullptr;
	}

	const Type& IAssetRef::GetAssetType() const
	{
		return TypeOf(Asset);
	}

	bool IAssetRef::operator!=(const IAssetRef& other) const
	{
		return mAssetPtr != other.mAssetPtr;
	}

	bool IAssetRef::operator==(const IAssetRef& other) const
	{
		return mAssetPtr == other.mAssetPtr;
	}

}

DECLARE_CLASS(o2::Asset);

DECLARE_CLASS(o2::IAssetRef);
