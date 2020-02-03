#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Utils/Serialization/DataNode.h"

namespace o2
{
	// ----------
	// Data asset
	// ----------
	class DataAsset: public AssetWithDefaultMeta<DataAsset>
	{
	public:
		DataNode data; // Asset data

	public:
		// Check equals operator
		DataAsset& operator=(const DataAsset& asset);

		// Returns extensions string
		const char* GetFileExtensions() const override;

	protected:
		// Default constructor
		DataAsset();

		// Copy-constructor
		DataAsset(const DataAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves data
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<DataAsset> DataAssetRef;
}
