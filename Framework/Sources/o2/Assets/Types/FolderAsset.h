#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// ------------
	// Folder asset
	// ------------
	class FolderAsset: public AssetWithDefaultMeta<FolderAsset>
	{
	public:
		// Check equals operator
		FolderAsset& operator=(const FolderAsset& asset);

		// Returns containing assets infos
		Vector<AssetRef> GetContainingAssetsInfos() const;

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
