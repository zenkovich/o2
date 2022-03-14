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
	FIELD().PUBLIC().NAME(log);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(assetsPath);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(builtAssetsPath);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(rootAssets);
	FIELD().PUBLIC().NAME(allAssets);
	FIELD().PUBLIC().NAME(allAssetsByPath);
	FIELD().PUBLIC().NAME(allAssetsByUID);
}
END_META;
CLASS_METHODS_META(o2::AssetsTree)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, Build, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, Build, const FolderInfo&);
	FUNCTION().PUBLIC().SIGNATURE(void, Rebuild);
	FUNCTION().PUBLIC().SIGNATURE(void, SortAssets);
	FUNCTION().PUBLIC().SIGNATURE(void, SortAssetsInverse);
	FUNCTION().PUBLIC().SIGNATURE(AssetInfo*, Find, const String&);
	FUNCTION().PUBLIC().SIGNATURE(AssetInfo*, Find, const UID&);
	FUNCTION().PUBLIC().SIGNATURE(AssetInfo*, AddAsset, AssetInfo*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAsset, AssetInfo*, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, Clear);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadFolder, const FolderInfo&, AssetInfo*);
	FUNCTION().PROTECTED().SIGNATURE(AssetInfo*, LoadAssetNode, const String&, AssetInfo*, const TimeStamp&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
