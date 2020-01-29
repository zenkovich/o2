#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// ------------
	// Folder asset
	// ------------
	class FolderAsset: public TAsset<FolderAsset>
	{
	public:
		class Meta;

	public:
		PROPERTIES(FolderAsset);
		GETTER(Meta*, meta, GetMeta);                                     // Meta information getter
		GETTER(Vector<AssetRef>, insideAssets, GetContainingAssetsInfos); // Inside assets infos getter

	public:
		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Returns containing assets infos
		Vector<AssetRef> GetContainingAssetsInfos() const;

		// Returns meta information
		Meta* GetMeta() const;

		SERIALIZABLE(FolderAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<FolderAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

	protected:
		// Default constructor
		FolderAsset();

		// Copy-constructor
		FolderAsset(const FolderAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves asset data
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<FolderAsset> FolderAssetRef;
}

CLASS_BASES_META(o2::FolderAsset)
{
	BASE_CLASS(o2::TAsset<FolderAsset>);
}
END_META;
CLASS_FIELDS_META(o2::FolderAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(insideAssets);
}
END_META;
CLASS_METHODS_META(o2::FolderAsset)
{

	PUBLIC_FUNCTION(Vector<AssetRef>, GetContainingAssetsInfos);
	PUBLIC_FUNCTION(Meta*, GetMeta);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::FolderAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<FolderAsset>);
}
END_META;
CLASS_FIELDS_META(o2::FolderAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::FolderAsset::Meta)
{
}
END_META;
