#include "o2/stdafx.h"
#include "AssetRef.h"

namespace o2
{
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
