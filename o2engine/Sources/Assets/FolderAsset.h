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
		Getter<Ptr<MetaInfo>> Meta;         // Meta information getter
		Getter<AssetInfosVec> InsideAssets; // Inside assets infos getter

		// Default constructor
		FolderAsset();

		// Constructor by path - loads asset by path
		FolderAsset(const String& path);

		// Constructor by id - loads asset by id
		FolderAsset(UInt id);

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

		IOBJECT(FolderAsset)
		{
			BASE_CLASS(Asset);
			FIELD(mContainingAssetsInfos);
		}

	protected:
		AssetInfosVec mContainingAssetsInfos; // Inside assets infos

	protected:
		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}
