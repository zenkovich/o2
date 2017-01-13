#pragma once

#include "Assets/Asset.h"
#include "Assets/Assets.h"

namespace o2
{
	// ------------------------
	// Template asset reference
	// ------------------------
	template<typename _asset_type>
	class AssetRef
	{
	public:
		// Default constructor, references to null
		AssetRef();

		// Copy-constructor
		AssetRef(const AssetRef<_asset_type>& other);

		// Destructor
		~AssetRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		AssetRef<_asset_type>& operator=(const AssetRef<_asset_type>& other);

		// Asset members and field operator
		_asset_type* operator->();

		// Constant asset members and field operator
		const _asset_type* operator->() const;

		// Check equals operator
		bool operator==(const AssetRef<_asset_type>& other) const;

		// Check not equals operator
		bool operator!=(const AssetRef<_asset_type>& other) const;

		// Returns is reference is valid
		bool IsValid() const;

	protected:
		int*         mRefCounter;
		_asset_type* mAssetPtr;

	protected:
		// Constructor for Assets manager
		AssetRef(_asset_type* assetPtr, int* refCounter);

		friend class Assets;
	};

	template<typename _asset_type>
	AssetRef<_asset_type>::AssetRef(_asset_type* assetPtr, int* refCounter):
		mAssetPtr(assetPtr), mRefCounter(refCounter)
	{
		if (mRefCounter)
			(*mRefCounter)++;
	}

	template<typename _asset_type>
	bool AssetRef<_asset_type>::IsValid() const
	{
		return mAssetPtr != nullptr;
	}

	template<typename _asset_type>
	bool AssetRef<_asset_type>::operator!=(const AssetRef<_asset_type>& other) const
	{
		return mAssetPtr != other.mAssetPtr;
	}

	template<typename _asset_type>
	bool AssetRef<_asset_type>::operator==(const AssetRef<_asset_type>& other) const
	{
		return mAssetPtr == other.mAssetPtr;
	}

	template<typename _asset_type>
	const _asset_type* AssetRef<_asset_type>::operator->() const
	{
		return mAssetPtr;
	}

	template<typename _asset_type>
	_asset_type* AssetRef<_asset_type>::operator->()
	{
		return mAssetPtr;
	}

	template<typename _asset_type>
	AssetRef<_asset_type>& AssetRef<_asset_type>::operator=(const AssetRef<_asset_type>& other)
	{
		if (mAssetPtr)
			(*mRefCounter)--;

		mAssetPtr = other.mAssetPtr;
		mRefCounter = other.mRefCounter;

		if (mAssetPtr)
			(*mRefCounter)++;
	}

	template<typename _asset_type>
	AssetRef<_asset_type>::operator bool() const
	{
		return mAssetPtr != nullptr;
	}

	template<typename _asset_type>
	AssetRef<_asset_type>::~AssetRef()
	{
		if (mAssetPtr)
			(*mRefCounter)--;
	}

	template<typename _asset_type>
	AssetRef<_asset_type>::AssetRef(const AssetRef<_asset_type>& other):
		mAssetPtr(other.mAssetPtr), mRefCounter(other.mRefCounter)
	{
		if (mAssetPtr)
			(*mRefCounter)++;
	}

	template<typename _asset_type>
	AssetRef<_asset_type>::AssetRef():
		mAssetPtr(nullptr), mRefCounter(nullptr)
	{}

}
