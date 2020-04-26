#include "o2/stdafx.h"
#include "AssetRef.h"

#include "o2/Assets/Assets.h"

namespace o2
{
	AssetRef::AssetRef()
	{}

	AssetRef::AssetRef(const AssetRef& other) :
		mAssetPtr(other.mAssetPtr), mRefCounter(other.mRefCounter), mAssetOwner(other.mAssetOwner)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	AssetRef::AssetRef(const String& path):
		AssetRef(o2Assets.GetAssetRef(path))
	{}

	AssetRef::AssetRef(const UID& id):
		AssetRef(o2Assets.GetAssetRef(id))
	{}

	AssetRef::AssetRef(Asset* assetPtr, int* refCounter):
		mAssetPtr(assetPtr), mRefCounter(refCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	AssetRef::AssetRef(Asset* instance)
	{
		mAssetPtr = instance;
		mRefCounter = &o2Assets.AddAssetCache(mAssetPtr)->referencesCount;
		mAssetOwner = true;
	}

	void AssetRef::OnSerialize(DataNode& node) const
	{
		if (mAssetOwner)
		{
			node["own"] = true;

			if (mAssetPtr)
			{
				node["asset"] = mAssetPtr->Serialize();
				node["meta"] = mAssetPtr->GetMeta();
			}
		}
		else if (mAssetPtr)
		{
			node["id"] = mAssetPtr->GetUID().ToString();
			node["path"] = mAssetPtr->GetPath();
		}
	}

	void AssetRef::OnDeserialized(const DataNode& node)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = nullptr;
		mRefCounter = nullptr;

		if (node.GetNode("own"))
		{
			mAssetPtr = *node.GetNode("asset");
			mAssetPtr->mInfo.meta = *node.GetNode("meta");
			mRefCounter = &o2Assets.AddAssetCache(mAssetPtr)->referencesCount;
			UpdateSpecAsset();
		}
		else if (auto idNode = node.GetNode("id"))
		{
			*this = o2Assets.GetAssetRef((UID)(*idNode));
			UpdateSpecAsset();
		}
		else if (auto pathNode = node.GetNode("path"))
		{
			*this = o2Assets.GetAssetRef(pathNode->Data());
			UpdateSpecAsset();
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
		mAssetOwner = other.mAssetOwner;

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

	Asset* AssetRef::Get()
	{
		return mAssetPtr;
	}

	const Asset* AssetRef::Get() const
	{
		return mAssetPtr;
	}

	const Type& AssetRef::GetAssetType() const
	{
		return TypeOf(Asset);
	}

	void AssetRef::SetInstance(Asset* asset)
	{
		if (mAssetOwner)
			return;

		if (mAssetPtr)
			*mRefCounter--;

		mAssetPtr = asset;

		mRefCounter = &o2Assets.AddAssetCache(mAssetPtr)->referencesCount;
		UpdateSpecAsset();

		mAssetOwner = true;
	}

	void AssetRef::CreateInstance()
	{
		if (mAssetOwner)
			return;

		if (mAssetPtr)
			*mRefCounter--;

		if (mAssetPtr)
			mAssetPtr = mAssetPtr->CloneAs<Asset>();
		else
		{
			auto objectType = dynamic_cast<const ObjectType*>(&GetAssetType());
			mAssetPtr = dynamic_cast<Asset*>(objectType->DynamicCastToIObject(objectType->CreateSample()));
		}

		mRefCounter = &o2Assets.AddAssetCache(mAssetPtr)->referencesCount;
		UpdateSpecAsset();

		mAssetOwner = true;
	}

	void AssetRef::RemoveInstance()
	{
		if (!mAssetOwner)
			return;

		*mRefCounter--;

		mAssetOwner = false;
		mAssetPtr = nullptr;

		UpdateSpecAsset();
	}

	void AssetRef::SaveInstance(const String& path)
	{
		if (!mAssetOwner)
			return;

		mAssetPtr->SetPath(path);
		mAssetPtr->Save();

		*this = AssetRef(path);
	}

	bool AssetRef::IsInstance() const
	{
		return mAssetOwner;
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

DECLARE_CLASS(o2::AssetRef);
