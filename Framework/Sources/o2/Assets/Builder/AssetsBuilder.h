#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/Builder/StdAssetConverter.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class FolderInfo;
	class IAssetConverter;

	// -------------
	// Asset builder
	// -------------
	class AssetsBuilder: public RefCounterable
	{
	public:
		// Default constructor
		AssetsBuilder();

		// Destructor
		~AssetsBuilder();

		// Builds asset from assets path to dataAssetsPath. Removes all built assets if forcible is true
		const Vector<UID>& BuildAssets(const String& assetsPath, const String& dataAssetsPath, const String& dataAssetsTreePath, 
									   const Ref<AssetsTree>& assetsTree, bool forcible = false);

		// Returns source assets path in building
		const String& GetSourceAssetsPath() const;

		// Returns built assets path in building
		const String& GetBuiltAssetsPath() const;

	protected:
		Ref<LogStream> mLog; // Asset builder log stream

		String     mSourceAssetsPath;     // Source assets path
		AssetsTree mSourceAssetsTree;     // Source assets tree

		String      mBuiltAssetsPath;     // Built assets path
		String      mBuiltAssetsTreePath; // Built assets tree data path
		AssetsTree* mBuiltAssetsTree;     // Built assets tree

		Vector<UID> mModifiedAssets; // Modified assets infos

		Map<const Type*, IAssetConverter*> mAssetConverters;   // Assets converters by type
		StdAssetConverter                  mStdAssetConverter; // Standard assets converter

	protected:
		// Initializes converters
		void InitializeConverters();

		// Removes all built assets
		void RemoveBuiltAssets();

		// Checks basic atlas exist
		void CheckBasicAtlas();

		// Searching and removing assets
		void ProcessRemovedAssets();

		// Searching modified and moved assets
		void ProcessModifiedAssets();

		// Searches new assets
		void ProcessNewAssets();

		// Launches converters post process
		void ConvertersPostProcess();
		
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
