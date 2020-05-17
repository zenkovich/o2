#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Utils/FileSystem/FileInfo.h"
#include "o2/Utils/Basic/ITree.h"

namespace o2
{
	class LogStream;

	// --------------------------------
	// Asset tree structure information
	// --------------------------------
	class AssetsTree: public ISerializable
	{
	public:
		LogStream* log;  // Log stream

		String assetsPath;      // Assets path @SERIALIZABLE
		String builtAssetsPath; // Built assets path @SERIALIZABLE

		Vector<AssetInfo*>      rootAssets;      // Root path assets @SERIALIZABLE
		Vector<AssetInfo*>      allAssets;       // All assets
		Map<String, AssetInfo*> allAssetsByPath; // All assets by path
		Map<UID, AssetInfo*>    allAssetsByUID;  // All assets by UID

	public:
		// Default constructor
		AssetsTree();

		// Destructor
		~AssetsTree();

		// Builds tree for folder
		void Build(const String& path);

		// Builds tree by folder info
		void Build(const FolderInfo& folderInfo);

		// Rebuilds tree for current folder
		void Rebuild();

		// Sorts all assets by path depth
		void SortAssets();

		// Sorts inverted all assets by depth
		void SortAssetsInverse();

		// Returns asset by path. (nullptr if not asset with path)
		AssetInfo* Find(const String& path) const;

		// Returns asset by id. (nullptr if not asset with id)
		AssetInfo* Find(const UID& id) const;

		// Adds asset node information into structure
		AssetInfo* AddAsset(AssetInfo* asset);

		// Removes asset node information from structure
		void RemoveAsset(AssetInfo* asset, bool release = true);

		// Clears all information
		void Clear();

		SERIALIZABLE(AssetsTree);

	protected:
		// Loads assets nodes from folder
		void LoadFolder(const FolderInfo& folder, AssetInfo* parentAsset);

		// Loads and returns asset by path
		AssetInfo* LoadAssetNode(const String& path, AssetInfo* parent, const TimeStamp& time);

		// It is called when deserializing node, combine all nodes in mAllNodes
		void OnDeserialized(const DataValue& node) override;
	};
}

CLASS_BASES_META(o2::AssetsTree)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetsTree)
{
	PUBLIC_FIELD(log);
	PUBLIC_FIELD(assetsPath).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(builtAssetsPath).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(rootAssets).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(allAssets);
	PUBLIC_FIELD(allAssetsByPath);
	PUBLIC_FIELD(allAssetsByUID);
}
END_META;
CLASS_METHODS_META(o2::AssetsTree)
{

	PUBLIC_FUNCTION(void, Build, const String&);
	PUBLIC_FUNCTION(void, Build, const FolderInfo&);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(void, SortAssets);
	PUBLIC_FUNCTION(void, SortAssetsInverse);
	PUBLIC_FUNCTION(AssetInfo*, Find, const String&);
	PUBLIC_FUNCTION(AssetInfo*, Find, const UID&);
	PUBLIC_FUNCTION(AssetInfo*, AddAsset, AssetInfo*);
	PUBLIC_FUNCTION(void, RemoveAsset, AssetInfo*, bool);
	PUBLIC_FUNCTION(void, Clear);
	PROTECTED_FUNCTION(void, LoadFolder, const FolderInfo&, AssetInfo*);
	PROTECTED_FUNCTION(AssetInfo*, LoadAssetNode, const String&, AssetInfo*, const TimeStamp&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;
