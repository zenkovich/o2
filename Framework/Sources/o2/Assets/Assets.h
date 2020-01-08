#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/FileSystem/FileInfo.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"

// Assets system access macros
#define  o2Assets o2::Assets::Instance()

namespace o2
{
	class AssetsBuilder;
	class LogStream;

	// ----------------
	// Assets utilities
	// ----------------
	class Assets: public Singleton<Assets>
	{
	public:
		typedef Map<String, const Type*> TypesExtsMap;

	public:
		PROPERTIES(Assets);
		GETTER(String, assetsPath, GetAssetsPath); // Assets path getter

	public:
		Function<void(const Vector<UID>&)> onAssetsRebuilded; // Assets rebuilding event

	public:
		// Default constructor
		Assets();

		// Destructor
		~Assets();

		// Returns assets path
		String GetAssetsPath() const;

		// Returns data path
		String GetDataPath() const;

		// Returns asset path by asset id
		String GetAssetPath(UID id) const;

		// Returns asset id by path
		UID GetAssetId(const String& path) const;

		// Returns asset info by id
		AssetInfo GetAssetInfo(UID id) const;

		// Returns asset info by path
		AssetInfo GetAssetInfo(const String& path) const;

		// Returns assets types and extensions dictionary. Key - extension, value - type of asset
		const TypesExtsMap GetAssetsExtensionsTypes() const;

		// Returns standard asset type
		const Type* GetStdAssetType() const;

		// Returns asset type for extension
		const Type* GetAssetTypeByExtension(const String& extension) const;

		// Returns asset reference by path
		AssetRef GetAssetRef(const String& path);

		// Returns asset reference by id
		AssetRef GetAssetRef(UID id);

		// Creates asset type _asset_type
		template<typename _asset_type>
		AssetRef CreateAsset();

		// Returns true if asset exist by path
		bool IsAssetExist(const String& path) const;

		// Returns true if asset exist by id
		bool IsAssetExist(UID id) const;

		// Returns true if asset exist
		bool IsAssetExist(const AssetInfo& info) const;

		// Removes asset
		bool RemoveAsset(const AssetRef& asset, bool rebuildAssets = true);

		// Removes asset by path
		bool RemoveAsset(const String& path, bool rebuildAssets = true);

		// Removes asset by id
		bool RemoveAsset(UID id, bool rebuildAssets = true);

		// Removes asset by info
		bool RemoveAsset(const AssetInfo& info, bool rebuildAssets = true);

		// Copies asset
		bool CopyAsset(const AssetRef& asset, const String& dest, bool rebuildAssets = true);

		// Copies asset by path
		bool CopyAsset(const String& path, const String& dest, bool rebuildAssets = true);

		// Copies asset by id
		bool CopyAsset(UID id, const String& dest, bool rebuildAssets = true);

		// Copies asset by info
		bool CopyAsset(const AssetInfo& info, const String& dest, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(const AssetRef& asset, const String& newPath, bool rebuildAssets = true);

		// Moves asset by path to new path
		bool MoveAsset(const String& path, const String& newPath, bool rebuildAssets = true);

		// Moves asset by id to new path
		bool MoveAsset(UID id, const String& newPath, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(const AssetInfo& info, const String& newPath, bool rebuildAssets = true);

		// Moves assets to new path
		bool MoveAssets(const Vector<AssetInfo>& assets, const String& destPath, bool rebuildAssets = true);

		// Renames asset to new path
		bool RenameAsset(const AssetRef& asset, const String& newName, bool rebuildAssets = true);

		// Renames asset by path to new path
		bool RenameAsset(const String& path, const String& newName, bool rebuildAssets = true);

		// Renames asset by id to new path
		bool RenameAsset(UID id, const String& newName, bool rebuildAssets = true);

		// Renames asset to new path
		bool RenameAsset(const AssetInfo& info, const String& newName, bool rebuildAssets = true);

		// Rebuilds all assets
		void RebuildAssets(bool forcible = false);

		// Returns assets tree
		const AssetTree& GetAssetsTree() const;

		// Makes unique asset name from first path variant
		String MakeUniqueAssetName(const String& path);

		// Returns random asset id
		static UID GetRandomAssetId();

	protected:
		struct AssetCache
		{
			Asset* asset;
			int    referencesCount;

			~AssetCache();
		};
		typedef Vector<AssetCache*> AssetsCachesVec;

		String mAssetsFolderPath; // Project assets path
		String mDataFolderPath;   // Project data (builded assets) path

		AssetTree      mAssetsTree;    // Assets information tree
		LogStream*     mLog;           // Log stream
		AssetsBuilder* mAssetsBuilder; // Assets builder
		TypesExtsMap   mAssetsTypes;   // Assets types and extensions dictionary
		const Type*    mStdAssetType;  // Standard asset type

		AssetsCachesVec mCachedAssets; // Current cached assets

	protected:
		// Loads asset infos
		void LoadAssetsTree();

		// Initializes types extensions dictionary
		void LoadAssetTypes();

		// Returns asset cache by path
		AssetCache* FindAssetCache(const String& path);

		// Returns asset cache by id
		AssetCache* FindAssetCache(UID id);

		// Clears assets cache
		void ClearAssetsCache();

		friend class Asset;
		friend class FolderAsset;
	};

	template<typename _asset_type>
	AssetRef Assets::CreateAsset()
	{
		_asset_type* newAset = mnew _asset_type();

		auto cached = mnew AssetCache();
		cached->asset = newAset;
		cached->referencesCount = 0;

		mCachedAssets.Add(cached);

		return AssetRef(newAset, &cached->referencesCount);
	}

}
