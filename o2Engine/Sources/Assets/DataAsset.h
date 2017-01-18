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
			const Type* GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:

	protected:
		// Default constructor
		DataAsset();

		// Constructor by path - loads asset by path
		DataAsset(const String& path);

		// Constructor by id - loads asset by id
		DataAsset(UID id);

		// Copy-constructor
		DataAsset(const DataAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------
	// Data Asset reference
	// ---------------------
	class DataAssetRef: public AssetRef
	{
	public:
		// Creates DataAsset and returns reference to it
		static DataAssetRef CreateAsset();

		// Default constructor, references to null
		DataAssetRef(): AssetRef() {}

		// Copy-constructor
		DataAssetRef(const AssetRef& other): AssetRef(other) { CheckType<DataAsset>(); }

		// Copy-constructor
		DataAssetRef(const DataAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		DataAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		DataAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~DataAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		DataAssetRef& operator=(const DataAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		DataAsset& operator*() { return *((DataAsset*)mAssetPtr); }

		// Constant getter operator
		const DataAsset& operator*() const { return *((DataAsset*)mAssetPtr); }

		// Asset members and field operator
		DataAsset* operator->() { return ((DataAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const DataAsset* operator->() const { return ((DataAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const DataAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const DataAssetRef& other) const { return AssetRef::operator!=(other); }

		SERIALIZABLE(DataAssetRef);

	protected:
		// Constructor for Assets manager
		DataAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}