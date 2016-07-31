#pragma once

#include "Assets/Asset.h"

namespace o2
{
	// -------------------
	// Binary format asset
	// -------------------
	class BinaryAsset: public Asset
	{
	public: 
		class MetaInfo;

	public:
		Getter<char*>     data;     // Data getter
		Getter<UInt>      dataSize; // Data size getter
		Getter<MetaInfo*> meta;     // Meta information getter

		// Default constructor
		BinaryAsset();

		// Constructor by path - loads asset by path
		BinaryAsset(const String& path);

		// Constructor by id - loads asset by id
		BinaryAsset(AssetId id);

		// Copy-constructor
		BinaryAsset(const BinaryAsset& asset);

		// Destructor
		~BinaryAsset();

		// Check equals operator
		BinaryAsset& operator=(const BinaryAsset& asset);

		// Check equals operator
		bool operator==(const BinaryAsset& other) const;

		// Check not equals operator
		bool operator!=(const BinaryAsset& other) const;

		// Returns data pointer
		char* GetData() const;

		// Returns data size
		UInt GetDataSize() const;

		// Sets data and size
		void SetData(char* data, UInt size);

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(BinaryAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		char* mData;     // Asset data
		UInt  mDataSize; // Asset data size

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}