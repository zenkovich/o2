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
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			SERIALIZABLE_IMPL(MetaInfo);

			IOBJECT(MetaInfo)
			{
				BASE_CLASS(IMetaInfo);
			}
		};

	public:
		Getter<char*>         Data;     // Data getter
		Getter<UInt>          DataSize; // Data size getter
		Getter<Ptr<MetaInfo>> Meta;     // Meta information getter

		// Default constructor
		BinaryAsset();

		// Constructor by path - loads asset by path
		BinaryAsset(const String& path);

		// Constructor by id - loads asset by id
		BinaryAsset(UInt id);

		// Copy-constructor
		BinaryAsset(const BinaryAsset& asset);

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

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		IOBJECT(BinaryAsset)
		{
			BASE_CLASS(Asset);
			FIELD(mDataSize);
			FIELD(mData);
		}

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