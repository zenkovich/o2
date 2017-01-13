#pragma once

#include "Assets/AssetInfo.h"
#include "Assets/AssetsTree.h"
#include "Utils/Containers/Vector.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"
#include "Utils/Singleton.h"

// Assets system access macros
#define  o2Assets o2::Assets::Instance()

namespace o2
{
	class Asset;
	class LogStream;
	class AssetsBuilder;

	template<typename _asset_type>
	class AssetRef;

	// ----------------
	// Assets utilities
	// ----------------
	class Assets: public Singleton<Assets>
	{
	public:
		typedef Dictionary<String, const Type*> TypesExtsDict;

	public:
		Getter<String>                         assetsPath;        // Assets path getter
		Function<void(const Vector<UID>&)> onAssetsRebuilded; // Assets rebuilding event

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
		const TypesExtsDict GetAssetsExtensionsTypes() const;

		// Returns standard asset type
		const Type* GetStdAssetType() const;

		// Returns asset type for extension
		const Type* GetAssetTypeByExtension(const String& extension) const;

		// Creates new asset
		template<typename _asset_type>
		_asset_type* CreateAsset();

		// Loads new asset by path
		template<typename _asset_type>
		_asset_type* LoadAsset(const String& path);

		// Loads new asset by id
		template<typename _asset_type>
		_asset_type* LoadAsset(UID id);

		// Loads asset by info
		Asset* LoadAsset(const AssetInfo& info);

		// Returns asset reference by path
		template<typename _asset_type>
		AssetRef<_asset_type> GetAsset(const String& path);

		// Returns asset reference by id
		template<typename _asset_type>
		AssetRef<_asset_type> GetAsset(UID id);

		// Returns true if asset exist by path
		bool IsAssetExist(const String& path) const;

		// Returns true if asset exist by id
		bool IsAssetExist(UID id) const;

		// Returns true if asset exist
		bool IsAssetExist(const AssetInfo& info) const;

		// Removes asset
		bool RemoveAsset(Asset* asset, bool rebuildAssets = true);

		// Removes asset by path
		bool RemoveAsset(const String& path, bool rebuildAssets = true);

		// Removes asset by id
		bool RemoveAsset(UID id, bool rebuildAssets = true);

		// Removes asset by info
		bool RemoveAsset(const AssetInfo& info, bool rebuildAssets = true);

		// Copies asset
		bool CopyAsset(Asset* asset, const String& dest, bool rebuildAssets = true);

		// Copies asset by path
		bool CopyAsset(const String& path, const String& dest, bool rebuildAssets = true);

		// Copies asset by id
		bool CopyAsset(UID id, const String& dest, bool rebuildAssets = true);

		// Copies asset by info
		bool CopyAsset(const AssetInfo& info, const String& dest, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(Asset* asset, const String& newPath, bool rebuildAssets = true);

		// Moves asset by path to new path
		bool MoveAsset(const String& path, const String& newPath, bool rebuildAssets = true);

		// Moves asset by id to new path
		bool MoveAsset(UID id, const String& newPath, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(const AssetInfo& info, const String& newPath, bool rebuildAssets = true);

		// Moves assets to new path
		bool MoveAssets(const Vector<AssetInfo>& assets, const String& destPath, bool rebuildAssets = true);

		// Renames asset to new path
		bool RenameAsset(Asset* asset, const String& newName, bool rebuildAssets = true);

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
			String path;
			UID    id;
			int    referencesCount;

			~AssetCache();
		};
		typedef Vector<AssetCache*> AssetsCachesVec;

		String          mAssetsFolderPath; // Project assets path
		String          mDataFolderPath;   // Project data (builded assets) path

		AssetTree       mAssetsTree;       // Assets information tree
		LogStream*      mLog;              // Log stream
		AssetsBuilder*  mAssetsBuilder;    // Assets builder
		TypesExtsDict   mAssetsTypes;      // Assets types and extensions dictionary
		const Type*     mStdAssetType;     // Standard asset type

		AssetsCachesVec mCachedAssets;     // Current cached assets

	protected:
		// Loads asset infos
		void LoadAssetsTree();

		// Initializes types extensions dictionary
		void LoadAssetTypes();

		// Initializes properties
		void InitializeProperties();

		// Returns asset cache by path
		AssetCache* FindAssetCache(const String& path);

		// Returns asset cache by id
		AssetCache* FindAssetCache(UID id);

		friend class Asset;
		friend class FolderAsset;
	};

}

#include "Assets/AssetRef.h"

namespace o2
{
	template<typename _asset_type>
	_asset_type* Assets::CreateAsset()
	{
		return mnew _asset_type();
	}

	template<typename _asset_type>
	_asset_type* Assets::LoadAsset(const String& path)
	{
		return mnew _asset_type(path);
	}

	template<typename _asset_type>
	_asset_type* Assets::LoadAsset(UID id)
	{
		return mnew _asset_type(id);
	}

	template<typename _asset_type>
	AssetRef<_asset_type> Assets::GetAsset(const String& path)
	{
		auto cached = FindAssetCache(path);

		if (!cached)
		{
			if (!IsAssetExist(path))
				return AssetRef<_asset_type>();

			_asset_type* asset = mnew _asset_type();
			asset->Load(path);

			cached = mnew AssetCache();
			cached->asset = asset;
			cached->path = path;
			cached->id = asset->GetAssetId();
			cached->referencesCount = 0;

			mCachedAssets.Add(cached);
		}

		return AssetRef<_asset_type>((_asset_type*)cached->asset, &cached->referencesCount);
	}

	template<typename _asset_type>
	AssetRef<_asset_type> Assets::GetAsset(UID id)
	{
		auto cached = FindAssetCache(id);

		if (!cached)
		{
			if (!IsAssetExist(id))
				return AssetRef<_asset_type>();

			_asset_type* asset = mnew _asset_type();
			asset->Load(id);

			cached = mnew AssetCache();
			cached->asset = asset;
			cached->path = asset->GetPath();
			cached->id = id;
			cached->referencesCount = 0;

			mCachedAssets.Add(cached);
		}

		return AssetRef<_asset_type>((_asset_type*)cached->asset, &cached->referencesCount);
	}

}