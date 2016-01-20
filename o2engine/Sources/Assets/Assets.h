#pragma once

#include "Assets/AssetInfo.h"
#include "Assets/AssetsTree.h"
#include "Utils/Containers/Vector.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

// Assets system access macros
#define  o2Assets o2::Assets::Instance()

namespace o2
{
	class Asset;
	class LogStream;
	class AssetsBuilder;

	// ----------------
	// Assets utilities
	// ----------------
	class Assets: public Singleton<Assets>
	{
	public:
		typedef Dictionary<String, Type*> TypesExtsDict;

	public:
		Getter<String> assetsPath; // Assets path getter

		// Default constructor
		Assets();

		// Destructor
		~Assets();

		// Returns assets path
		String GetAssetsPath() const;

		// Returns data path
		String GetDataPath() const;

		// Returns asset path by asset id
		String GetAssetPath(AssetId id) const;

		// Returns asset id by path
		AssetId GetAssetId(const String& path) const;

		// Returns asset info by id
		AssetInfo GetAssetInfo(AssetId id) const;

		// Returns asset info by path
		AssetInfo GetAssetInfo(const String& path) const;

		// Returns assets types and extensions dictionary. Key - extension, value - type of asset
		const TypesExtsDict GetAssetsExtensionsTypes() const;

		// Returns standard asset type
		Type* GetStdAssetType() const;

		// Returns asset type for extension
		Type* GetAssetTypeByExtension(const String& extension) const;

		// Creates new asset
		template<typename _asset_type>
		_asset_type* CreateAsset();

		// Loads new asset by path
		template<typename _asset_type>
		_asset_type* LoadAsset(const String& path);

		// Loads new asset by id
		template<typename _asset_type>
		_asset_type* LoadAsset(AssetId id);

		// Loads asset by info
		Asset* LoadAsset(const AssetInfo& info);

		// Returns true if asset exist by path
		bool IsAssetExist(const String& path) const;

		// Returns true if asset exist by id
		bool IsAssetExist(AssetId id) const;

		// Returns true if asset exist
		bool IsAssetExist(const AssetInfo& info) const;

		// Removes asset
		bool RemoveAsset(Asset* asset, bool rebuildAssets = true);

		// Removes asset by path
		bool RemoveAsset(const String& path, bool rebuildAssets = true);

		// Removes asset by id
		bool RemoveAsset(AssetId id, bool rebuildAssets = true);

		// Removes asset by info
		bool RemoveAsset(const AssetInfo& info, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(Asset* asset, const String& newPath, bool rebuildAssets = true);

		// Moves asset by path to new path
		bool MoveAsset(const String& path, const String& newPath, bool rebuildAssets = true);

		// Moves asset by id to new path
		bool MoveAsset(AssetId id, const String& newPath, bool rebuildAssets = true);

		// Moves asset to new path
		bool MoveAsset(const AssetInfo& info, const String& newPath, bool rebuildAssets = true);

		// Rebuilds all assets
		void RebuildAssets(bool forcible = false);
		
		// Returns random asset id
		static AssetId GetRandomAssetId();

	protected:
		AssetTree      mAssetsTree;    // Assets information tree
		LogStream*     mLog;           // Log stream
		AssetsBuilder* mAssetsBuilder; // Assets builder
		TypesExtsDict  mAssetsTypes;   // Assets types and extensions dictionary
		Type*          mStdAssetType;  // Standard asset type

	protected:
		// Loads asset infos
		void LoadAssetsTree();

		// Initializes types extensions dictionary
		void LoadAssetTypes();

		// Initializes properties
		void InitializeProperties();

		friend class Asset;
		friend class FolderAsset;
	};


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
	_asset_type* Assets::LoadAsset(AssetId id)
	{
		return mnew _asset_type(id);
	}
}