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
		Getter<Ptr<MetaInfo>> meta;         // Meta information getter
		Getter<AssetInfosVec> insideAssets; // Inside assets infos getter

		// Default constructor
		FolderAsset();

		// Constructor by path - loads asset by path
		FolderAsset(const String& path);

		// Constructor by id - loads asset by id
		FolderAsset(AssetId id);

		// Copy-constructor
		FolderAsset(const FolderAsset& asset);

		// Destructor
		~FolderAsset();

		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Returns containing assets infos
		AssetInfosVec GetContainingAssetsInfos() const;

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

		SERIALIZABLE(FolderAsset);

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
		AssetInfosVec mContainingAssetsInfos; // Inside assets infos

	protected:
		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}
