#pragma once

#include "Assets/Asset.h"
#include "Utils/Serialization/DataNode.h"

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
		PROPERTIES(DataAsset);
		GETTER(MetaInfo*, meta, GetMeta); // Meta information getter

	public:
		DataNode data; // Asset data

	public:
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
		const char* GetFileExtensions() const override;

		SERIALIZABLE(DataAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const override;

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
		void LoadData(const String& path) override;

		// Saves data
		void SaveData(const String& path) override;

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

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(DataAsset); }

		SERIALIZABLE(DataAssetRef);

	protected:
		// Constructor for Assets manager
		DataAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::DataAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(data);
}
END_META;
CLASS_METHODS_META(o2::DataAsset)
{

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::DataAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::DataAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::DataAssetRef)
{

	PUBLIC_STATIC_FUNCTION(DataAssetRef, CreateAsset);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::DataAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::DataAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
