#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetsTree.h"
#include "Assets/Builder/StdAssetConverter.h"
#include "Utils/String.h"

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
		void BuildAssets(const String& assetsPath, const String& dataAssetsPath, bool forcible = false);

	protected:
		typedef Dictionary<Type::Id, Ptr<IAssetConverter>> ConvertersDict;

		Ptr<LogStream>    mLog;                 // Asset builder log stream

		String            mSourceAssetsPath;    // Source assets path
		AssetTree         mSourceAssetsTree;    // Source assets tree
		String            mBuildedAssetsPath;   // Builded assets path
		AssetTree         mBuildedAssetsTree;   // Builded assets tree

		ConvertersDict    mAssetConverters;     // Assets converters by type
		StdAssetConverter mStdAssetConverter;   // Standard assets converter

	protected:
		// Initializes converters
		void InitializeConverters();

		// Removes all builded assets
		void RemoveBuildedAssets();

		// Checks basic atlas exist
		void CheckBasicAtlas();

		// Creates missing meta fields for source assets
		void CreateMissingMetas();

		// Searching and removing assets
		void ProcessRemovedAssets();

		// Searching modified and moved assets
		void ProcessModifiedAssets();

		// Searches new assets
		void ProcessNewAssets();

		// Launches converters postprocess
		void ConvertersPostProcess();
		
		// Processes folder for missing metas
		void ProcessMissingMetasCreation(FolderInfo& folder);
		
		// Generates meta information file for asset
		void GenerateMeta(const Type* assetType, const String& metaFullPath);

		// Returns assets converter by asset type
		Ptr<IAssetConverter> GetAssetConverter(Type::Id assetTypeId);

		// Resets builder
		void Reset();

		friend class AtlasAssetConverter;
	};
}
