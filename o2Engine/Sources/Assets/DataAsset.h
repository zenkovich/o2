#pragma once

#include "Assets/Asset.h"
#include "Utils/Data/DataNode.h"

namespace o2
{
	// ----------
	// Data asset
	// ----------
	class DataAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		DataNode          data; // Asset data
		Getter<MetaInfo*> meta; // Meta information getter

		// Default constructor
		DataAsset();

		// Constructor by path - loads asset by path
		DataAsset(const String& path);

		// Constructor by id - loads asset by id
		DataAsset(UID id);

		// Copy-constructor
		DataAsset(const DataAsset& asset);

		// Destructor
		~DataAsset();

		// Check equals operator
		DataAsset& operator=(const DataAsset& asset);

		// Check equals operator
		bool operator==(const DataAsset& other) const;

		// Check not equals operator
		bool operator!=(const DataAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(DataAsset);

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

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}