#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/Builder/StdAssetConverter.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class FolderInfo;
	class IAssetConverter;

	// -------------
	// Asset builder
	// -------------
	class AssetsBuilder
	{
	public:
		// Default constructor
		AssetsBuilder();

		// Destructor
		~AssetsBuilder();

		// Builds asset from assets path to dataAssetsPath. Removes all builded assets if forcible is true
		Vector<UID> BuildAssets(const String& assetsPath, const String& dataAssetsPath, const String& dataAssetsTreePath, bool forcible = false);

		// Returns source assets path in building
		const String& GetSourceAssetsPath() const;

		// Returns built assets path in building
		const String& GetBuiltAssetsPath() const;

	protected:
		LogStream* mLog; // Asset builder log stream

		String     mSourceAssetsPath;     // Source assets path
		AssetsTree mSourceAssetsTree;     // Source assets tree

		String     mBuiltAssetsPath;     // Built assets path
		String     mBuiltAssetsTreePath; // Built assets tree data path
		AssetsTree mBuiltAssetsTree;     // Built assets tree

		Vector<AssetsTree::AssetNode*> mModifiedAssets; // Modified assets infos

		Map<const Type*, IAssetConverter*> mAssetConverters;   // Assets converters by type
		StdAssetConverter                  mStdAssetConverter; // Standard assets converter

	protected:
		// Initializes converters
		void InitializeConverters();

		// Removes all builded assets
		void RemoveBuiltAssets();

		// Checks basic atlas exist
		void CheckBasicAtlas();

		// Creates missing meta fields for source assets
		void CreateMissingMetas();

		// Searching and removing assets
		Vector<UID> ProcessRemovedAssets();

		// Searching modified and moved assets
		Vector<UID> ProcessModifiedAssets();

		// Searches new assets
		Vector<UID> ProcessNewAssets();

		// Launches converters post process
		Vector<UID> ConvertersPostProcess();

		// Saving data assets tree
		void SaveAssetsTree();
		
		// Processes folder for missing metas
		void ProcessMissingMetasCreation(FolderInfo& folder);
		
		// Generates meta information file for asset
		void GenerateMeta(const Type& assetType, const String& metaFullPath);

		// Returns assets converter by asset type
		IAssetConverter* GetAssetConverter(const Type* assetType);

		// Resets builder
		void Reset();

		friend class AtlasAssetConverter;
	};
}
