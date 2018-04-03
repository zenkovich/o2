#pragma once

#include "Assets/Asset.h"

namespace o2
{
	// ------------
	// Folder asset
	// ------------
	class FolderAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		PROPERTIES(FolderAsset);
		GETTER(MetaInfo*, meta, GetMeta);                              // Meta information getter
		GETTER(AssetInfosVec, insideAssets, GetContainingAssetsInfos); // Inside assets infos getter

		// Destructor
		~FolderAsset();

		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Check equals operator
		bool operator==(const FolderAsset& other) const;

		// Check not equals operator
		bool operator!=(const FolderAsset& other) const;

		// Returns containing assets infos
		AssetInfosVec GetContainingAssetsInfos() const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		SERIALIZABLE(FolderAsset);

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
		AssetInfosVec mContainingAssetsInfos; // Inside assets infos

	protected:
		// Default constructor
		FolderAsset();

		// Constructor by path - loads asset by path
		FolderAsset(const String& path);

		// Constructor by id - loads asset by id
		FolderAsset(UID id);

		// Copy-constructor
		FolderAsset(const FolderAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves asset data
		void SaveData(const String& path);

		friend class Assets;
	};

	// ----------------------
	// Folder Asset reference
	// ----------------------
	class FolderAssetRef: public AssetRef
	{
	public:
		// Creates FolderAsset and returns reference to it
		static FolderAssetRef CreateAsset();

		// Default constructor, references to null
		FolderAssetRef(): AssetRef() {}

		// Copy-constructor
		FolderAssetRef(const AssetRef& other): AssetRef(other) { CheckType<FolderAsset>(); }

		// Copy-constructor
		FolderAssetRef(const FolderAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		FolderAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		FolderAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~FolderAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		FolderAssetRef& operator=(const FolderAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		FolderAsset& operator*() { return *((FolderAsset*)mAssetPtr); }

		// Constant getter operator
		const FolderAsset& operator*() const { return *((FolderAsset*)mAssetPtr); }

		// Asset members and field operator
		FolderAsset* operator->() { return ((FolderAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const FolderAsset* operator->() const { return ((FolderAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const FolderAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const FolderAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(FolderAsset); }

		SERIALIZABLE(FolderAssetRef);

	protected:
		// Constructor for Assets manager
		FolderAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::FolderAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::FolderAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(insideAssets);
	PROTECTED_FIELD(mContainingAssetsInfos);
}
END_META;
CLASS_METHODS_META(o2::FolderAsset)
{

	PUBLIC_FUNCTION(AssetInfosVec, GetContainingAssetsInfos);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::FolderAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::FolderAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::FolderAssetRef)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::FolderAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::FolderAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::FolderAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
