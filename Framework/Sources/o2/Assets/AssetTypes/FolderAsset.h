#pragma once

#include "o2/Assets/Asset.h"

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
		GETTER(Vector<AssetInfo>, insideAssets, GetContainingAssetsInfos); // Inside assets infos getter

	public:
		// Destructor
		~FolderAsset();

		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Check equals operator
		bool operator==(const FolderAsset& other) const;

		// Check not equals operator
		bool operator!=(const FolderAsset& other) const;

		// Returns containing assets infos
		Vector<AssetInfo> GetContainingAssetsInfos() const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		SERIALIZABLE(FolderAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public AssetMeta
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const override;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		Vector<AssetInfo> mContainingAssetsInfos; // Inside assets infos

	protected:
		// Default constructor
		FolderAsset();

		// Constructor by path - loads asset by path
		FolderAsset(const String& path);

		// Constructor by id - loads asset by id
		FolderAsset(const UID& id);

		// Copy-constructor
		FolderAsset(const FolderAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves asset data
		void SaveData(const String& path) override;

		friend class Assets;
	};

	// ----------------------
	// Folder Asset reference
	// ----------------------
	class FolderAssetRef: public IAssetRef
	{
	public:
		// Creates FolderAsset and returns reference to it
		static FolderAssetRef CreateAsset();

		// Default constructor, references to null
		FolderAssetRef(): IAssetRef() {}

		// Copy-constructor
		FolderAssetRef(const IAssetRef& other): IAssetRef(other) { CheckType<FolderAsset>(); }

		// Copy-constructor
		FolderAssetRef(const FolderAssetRef& other): IAssetRef(other) {}

		// Constructor from asset path
		FolderAssetRef(const String& path): IAssetRef(path) {}

		// Constructor from asset id
		FolderAssetRef(const UID& id): IAssetRef(id) {}

		// Destructor
		~FolderAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		FolderAssetRef& operator=(const FolderAssetRef& other) { IAssetRef::operator=(other); return *this; }

		// Getter operator
		FolderAsset& operator*() { return *((FolderAsset*)mAssetPtr); }

		// Constant getter operator
		const FolderAsset& operator*() const { return *((FolderAsset*)mAssetPtr); }

		// Asset members and field operator
		FolderAsset* operator->() { return ((FolderAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const FolderAsset* operator->() const { return ((FolderAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const FolderAssetRef& other) const { return IAssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const FolderAssetRef& other) const { return IAssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(FolderAsset); }

		SERIALIZABLE(FolderAssetRef);

	protected:
		// Constructor for Assets manager
		FolderAssetRef(Asset* assetPtr, int* refCounter): IAssetRef(assetPtr, refCounter) {}
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

	PUBLIC_FUNCTION(Vector<AssetInfo>, GetContainingAssetsInfos);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::FolderAssetRef)
{
	BASE_CLASS(o2::IAssetRef);
}
END_META;
CLASS_FIELDS_META(o2::FolderAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::FolderAssetRef)
{

	PUBLIC_STATIC_FUNCTION(FolderAssetRef, CreateAsset);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::FolderAsset::MetaInfo)
{
	BASE_CLASS(o2::AssetMeta);
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
