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
			const Type* GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		char* mData;     // Asset data
		UInt  mDataSize; // Asset data size

	protected:
		// Default constructor
		BinaryAsset();

		// Constructor by path - loads asset by path
		BinaryAsset(const String& path);

		// Constructor by id - loads asset by id
		BinaryAsset(UID id);

		// Copy-constructor
		BinaryAsset(const BinaryAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------------
	// Binary data Asset reference
	// ---------------------------
	class BinaryAssetRef: public AssetRef
	{
	public:
		// Creates BinaryAsset and returns reference to it
		static BinaryAssetRef CreateAsset();

		// Default constructor, references to null
		BinaryAssetRef(): AssetRef() {}

		// Copy-constructor
		BinaryAssetRef(const AssetRef& other): AssetRef(other) { CheckType<BinaryAsset>(); }

		// Copy-constructor
		BinaryAssetRef(const BinaryAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		BinaryAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		BinaryAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~BinaryAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		BinaryAssetRef& operator=(const BinaryAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		BinaryAsset& operator*() { return *((BinaryAsset*)mAssetPtr); }

		// Constant getter operator
		const BinaryAsset& operator*() const { return *((BinaryAsset*)mAssetPtr); }

		// Asset members and field operator
		BinaryAsset* operator->() { return ((BinaryAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const BinaryAsset* operator->() const { return ((BinaryAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const BinaryAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const BinaryAssetRef& other) const { return AssetRef::operator!=(other); }

		SERIALIZABLE(BinaryAssetRef);

	protected:
		// Constructor for Assets manager
		BinaryAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}