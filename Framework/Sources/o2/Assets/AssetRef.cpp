#include "o2/stdafx.h"
#include "AssetRef.h"

#include "o2/Assets/Assets.h"

namespace o2
{
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

	AssetRef::AssetRef(const UID& id):
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

			o2Assets.AddAssetCache(mAssetPtr);
		}
		else if (auto idNode = node.GetNode("id"))
		{
			*this = o2Assets.GetAssetRef((UID)(*idNode));
		}
		else if (auto pathNode = node.GetNode("path"))
		{
			*this = o2Assets.GetAssetRef(pathNode->Data());
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

DECLARE_CLASS(o2::AssetRef);
