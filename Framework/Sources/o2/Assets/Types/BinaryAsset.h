#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// -------------------
	// Binary format asset
	// -------------------
	class BinaryAsset: public AssetWithDefaultMeta<BinaryAsset>
	{
	public:
		PROPERTIES(BinaryAsset);
		GETTER(char*, data, GetData);        // Data getter
		GETTER(UInt, dataSize, GetDataSize); // Data size getter

	public:
		// Destructor
		~BinaryAsset();

		// Check equals operator
		BinaryAsset& operator=(const BinaryAsset& asset);

		// Returns data pointer
		char* GetData() const;

		// Returns data size
		UInt GetDataSize() const;

		// Sets data and size
		void SetData(char* data, UInt size);

		// Returns extensions string
		const char* GetFileExtensions() const override;

	protected:
		char* mData = nullptr; // Asset data
		UInt  mDataSize = 0;   // Asset data size

	protected:
		// Default constructor
		BinaryAsset();

		// Copy-constructor
		BinaryAsset(const BinaryAsset& asset);

		// Loads asset data, using DataNode and serialization
		void LoadData(const String& path) override;

		// Saves asset data, using DataNode and serialization
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<BinaryAsset> BinaryAssetRef;
}
