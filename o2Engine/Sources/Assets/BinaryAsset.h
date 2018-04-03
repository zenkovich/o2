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
		PROPERTIES(BinaryAsset);
		GETTER(char*, data, GetData);        // Data getter
		GETTER(UInt, dataSize, GetDataSize); // Data size getter
		GETTER(MetaInfo*, meta, GetMeta);    // Meta information getter

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

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(BinaryAsset); }

		SERIALIZABLE(BinaryAssetRef);

	protected:
		// Constructor for Assets manager
		BinaryAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::BinaryAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::BinaryAsset)
{
	PUBLIC_FIELD(data);
	PUBLIC_FIELD(dataSize);
	PUBLIC_FIELD(meta);
	PROTECTED_FIELD(mData);
	PROTECTED_FIELD(mDataSize);
}
END_META;
CLASS_METHODS_META(o2::BinaryAsset)
{

	PUBLIC_FUNCTION(char*, GetData);
	PUBLIC_FUNCTION(UInt, GetDataSize);
	PUBLIC_FUNCTION(void, SetData, char*, UInt);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::BinaryAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::BinaryAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::BinaryAssetRef)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::BinaryAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::BinaryAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::BinaryAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
