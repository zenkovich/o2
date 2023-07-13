#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Utils/FileSystem/FileInfo.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Vector.h"

// Assets system access macros
#define  o2Assets o2::Assets::Instance()

namespace o2
{
	class LogStream;
	class AssetRef;

	// ----------------
	// Assets utilities
	// ----------------
	class Assets : public Singleton<Assets>
	{
	public:
		PROPERTIES(Assets);
		GETTER(String, assetsPath, GetAssetsPath); // Assets path getter

	public:
		Function<void(const Vector<UID>&)> onAssetsRebuilt; // Assets rebuilding event

	public:
		// Default constructor
		Assets();

		// Destructor
		~Assets();

		// Returns assets path
		String GetAssetsPath() const;

		// Returns data path
		String GetBuiltAssetsPath() const;

		// Returns asset path by asset id
		const String& GetAssetPath(const UID& id) const;

		// Returns asset id by path
		const UID& GetAssetId(const String& path) const;

		// Returns asset info by id
		const AssetInfo& GetAssetInfo(const UID& id) const;

		// Returns asset info by path
		const AssetInfo& GetAssetInfo(const String& path) const;

		// Returns assets types and extensions dictionary. Key - extension, value - type of asset
		static const Map<String, const Type*> GetAssetsExtensionsTypes();

		// Returns standard asset type
		static const Type* GetStdAssetType();

		// Returns asset type for extension
		static const Type* GetAssetTypeByExtension(const String& extension);

		// Returns asset reference by path
		AssetRef GetAssetRef(const String& path);

		// Returns asset reference by id
		AssetRef GetAssetRef(const UID& id);

		// Creates asset type _asset_type
		template<typename _asset_type, typename ... _args>
		AssetRef CreateAsset(_args ... args);

		// Returns true if asset exist by path
		bool IsAssetExist(const String& path) const;

		// Returns true if asset exist by id
		bool IsAssetExist(const UID& id) const;

		// Removes asset
		bool RemoveAsset(const AssetRef& asset);

		// Removes asset by path
		bool RemoveAsset(const String& path);

		// Removes asset by id
		bool RemoveAsset(const UID& id);

		// Copies asset
		bool CopyAsset(const AssetRef& asset, const String& dest);

		// Copies asset by path
		bool CopyAsset(const String& path, const String& dest);

		// Copies asset by id
		bool CopyAsset(const UID& id, const String& dest);

		// Moves asset to new path
		bool MoveAsset(const AssetRef& asset, const String& newPath);

		// Moves asset by path to new path
		bool MoveAsset(const String& path, const String& newPath);

		// Moves asset by id to new path
		bool MoveAsset(const UID& id, const String& newPath);

		// Moves assets to new path
		bool MoveAssets(const Vector<UID>& assets, const String& destPath);

		// Renames asset to new path
		bool RenameAsset(const AssetRef& asset, const String& newName);

		// Renames asset by path to new path
		bool RenameAsset(const String& path, const String& newName);

		// Renames asset by id to new path
		bool RenameAsset(const UID& id, const String& newName);

		// Returns all assets trees
		const Vector<AssetsTree*>& GetAssetsTrees() const;

		// Returns main tree
		const AssetsTree& GetAssetsTree() const;

		// Checks assets with zero references and removes them
		void CheckAssetsUnload();

		// Makes unique asset name from first path variant
		String MakeUniqueAssetName(const String& path);

	protected:
		struct AssetCache
		{
			Asset* asset;
			int    referencesCount;

			~AssetCache();
		};

	protected:
		AssetsTree*         mMainAssetsTree; // Main assets tree
		Vector<AssetsTree*> mAssetsTrees;    // Assets trees
		LogStream*          mLog;            // Log stream

		Vector<AssetCache*>      mCachedAssets;       // Current cached assets
		Map<String, AssetCache*> mCachedAssetsByPath; // Current cached assets by path
		Map<UID, AssetCache*>    mCachedAssetsByUID;  // Current cached assets by uid

	protected:
		// Loads asset infos
		void LoadAssetsTree();

		// Returns asset cache by path
		AssetCache* FindAssetCache(const String& path);

		// Returns asset cache by id
		AssetCache* FindAssetCache(const UID& id);

		// Clears assets cache
		void ClearAssetsCache();

		// Adds asset to cache
		AssetCache* AddAssetCache(Asset* asset);

		// Removes asset from cache by UID and path
		void RemoveAssetCache(Asset* asset);

		// Updates asset chached path and id
		AssetCache* UpdateAssetCache(Asset* asset, const String& oldPath, const UID& oldUID);

		// Removes asset by info
		bool RemoveAsset(const AssetInfo& info);

		// Copies asset by info
		bool CopyAsset(const AssetInfo& info, const String& dest);

		// Moves asset to new path
		bool MoveAsset(const AssetInfo& info, const String& newPath);

		// Renames asset to new path
		bool RenameAsset(const AssetInfo& info, const String& newName);

		friend class Asset;
		friend class AssetRef;
		friend class FolderAsset;
	};
}

#include "o2/Assets/AssetRef.h"

namespace o2
{
	template<typename _asset_type, typename ... _args>
	AssetRef Assets::CreateAsset(_args ... args)
	{
		_asset_type* newAset = mnew _asset_type(args ...);

		auto cached = mnew AssetCache();
		cached->asset = newAset;
		cached->referencesCount = 0;

		mCachedAssets.Add(cached);
		mCachedAssetsByUID[cached->asset->GetUID()] = cached;

		return AssetRef(newAset, &cached->referencesCount);
	}

}
