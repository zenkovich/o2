#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Utils/Basic/ITree.h"
#include "o2/Utils/FileSystem/FileInfo.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	class LogStream;

	// --------------------------------
	// Asset tree structure information
	// --------------------------------
	class AssetsTree: public ISerializable, public RefCounterable
	{
	public:
		Ref<LogStream> log; // Log stream

		String assetsPath;      // Assets path @SERIALIZABLE
		String builtAssetsPath; // Built assets path @SERIALIZABLE

		Vector<Ref<AssetInfo>>      rootAssets;      // Root path assets @SERIALIZABLE
		Vector<Ref<AssetInfo>>      allAssets;       // All assets
		Map<String, Ref<AssetInfo>> allAssetsByPath; // All assets by path
		Map<UID, Ref<AssetInfo>>    allAssetsByUID;  // All assets by UID

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
		Ref<AssetInfo> Find(const String& path) const;

		// Returns asset by id. (nullptr if not asset with id)
		Ref<AssetInfo> Find(const UID& id) const;

		// Adds asset node information into structure
		Ref<AssetInfo> AddAsset(const Ref<AssetInfo>& asset);

		// Removes asset node information from structure
		void RemoveAsset(const Ref<AssetInfo>& asset);

		// Clears all information
		void Clear();

		SERIALIZABLE(AssetsTree);

	protected:
		// Loads assets nodes from folder
		void LoadFolder(const FolderInfo& folder, Ref<AssetInfo> parentAsset);

		// Loads and returns asset by path
		Ref<AssetInfo> LoadAssetNode(const String& path, const Ref<AssetInfo>& parent, const TimeStamp& time);

		// Called when deserializing node, combine all nodes in mAllNodes
		void OnDeserialized(const DataValue& node) override;
	};
}

CLASS_BASES_META(o2::AssetsTree)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::RefCounterable);
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
	FUNCTION().PUBLIC().SIGNATURE(Ref<AssetInfo>, Find, const String&);
	FUNCTION().PUBLIC().SIGNATURE(Ref<AssetInfo>, Find, const UID&);
	FUNCTION().PUBLIC().SIGNATURE(Ref<AssetInfo>, AddAsset, const Ref<AssetInfo>&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAsset, const Ref<AssetInfo>&);
	FUNCTION().PUBLIC().SIGNATURE(void, Clear);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadFolder, const FolderInfo&, Ref<AssetInfo>);
	FUNCTION().PROTECTED().SIGNATURE(Ref<AssetInfo>, LoadAssetNode, const String&, const Ref<AssetInfo>&, const TimeStamp&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
