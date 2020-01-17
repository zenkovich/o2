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
		// ----------------------
		// Asset information node
		// ----------------------
		struct AssetNode: public AssetInfo
		{ 
			AssetsTree* tree = nullptr; // Owner asset tree

			Asset::IMetaInfo*  meta; // Asset meta
			TimeStamp          time; // Asset edited time
							  
			AssetNode*         parent = nullptr; // Parent node
			Vector<AssetNode*> children;         // Children nodes @SERIALIZABLE

		public:
			// Default constructor
			AssetNode();

			// Constructor
			AssetNode(const String& path, UID id, const Type* type);

			// Destructor
			~AssetNode();

			// Adds new child node and returns him
			AssetNode* AddChild(AssetNode* node);

			// Remove child node and releases him if needs
			bool RemoveChild(AssetNode* node, bool release = true);

			// Sets parent node
			void SetParent(AssetNode* parent);

			SERIALIZABLE(AssetNode);

		private:
			// It is called when deserializing node, sets parent for children
			void OnDeserialized(const DataNode& node) override;

			// Sets asset tree, adds in tree's allAssets map
			void SetTree(AssetsTree* tree);

			friend class AssetsTree;
		};

	public:
		String     path; // Assets information root path @SERIALIZABLE
		LogStream* log;  // Log stream

		Vector<AssetNode*>      rootAssets;      // Root path assets @SERIALIZABLE
		Vector<AssetNode*>      allAssets;       // All assets
		Map<String, AssetNode*> allAssetsByPath; // All assets by path
		Map<UID, AssetNode*>    allAssetsByUID;  // All assets by UID

	public:
		// Default constructor
		AssetsTree();

		// Destructor
		~AssetsTree();

		// Builds tree for folder
		void BuildTree(const String& path);

		// Rebuilds tree for current folder
		void RebuildTree();

		// Sorts all assets by path depth
		void Sort();

		// Sorts inverted all assets by depth
		void SortInverse();

		// Returns asset by path. (nullptr if not asset with path)
		AssetNode* FindAsset(const String& path) const;

		// Returns asset by id. (nullptr if not asset with id)
		AssetNode* FindAsset(UID id) const;

		// Returns asset info by path
		AssetInfo FindAssetInfo(const String& path) const;

		// Returns asset info by id
		AssetInfo FindAssetInfo(UID id) const;

		// Adds asset node information into structure
		AssetNode* AddAsset(AssetNode* asset);

		// Removes asset node information from structure
		void RemoveAsset(AssetNode* asset, bool release = true);

		// Clears all information
		void Clear();

		SERIALIZABLE(AssetsTree);

	protected:
		// Loads assets nodes from folder
		void LoadFolder(FolderInfo& folder, AssetNode* parentAsset);

		// Loads and returns asset by path
		AssetNode* LoadAssetNode(const String& path, AssetNode* parent, const TimeStamp& time);

		// It is called when deserializing node, combine all nodes in mAllNodes
		void OnDeserialized(const DataNode& node) override;
	};
}

CLASS_BASES_META(o2::AssetsTree)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetsTree)
{
	PUBLIC_FIELD(path).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(log);
	PUBLIC_FIELD(rootAssets).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(allAssets);
	PUBLIC_FIELD(allAssetsByPath);
	PUBLIC_FIELD(allAssetsByUID);
}
END_META;
CLASS_METHODS_META(o2::AssetsTree)
{

	PUBLIC_FUNCTION(void, BuildTree, const String&);
	PUBLIC_FUNCTION(void, RebuildTree);
	PUBLIC_FUNCTION(void, Sort);
	PUBLIC_FUNCTION(void, SortInverse);
	PUBLIC_FUNCTION(AssetNode*, FindAsset, const String&);
	PUBLIC_FUNCTION(AssetNode*, FindAsset, UID);
	PUBLIC_FUNCTION(AssetInfo, FindAssetInfo, const String&);
	PUBLIC_FUNCTION(AssetInfo, FindAssetInfo, UID);
	PUBLIC_FUNCTION(AssetNode*, AddAsset, AssetNode*);
	PUBLIC_FUNCTION(void, RemoveAsset, AssetNode*, bool);
	PUBLIC_FUNCTION(void, Clear);
	PROTECTED_FUNCTION(void, LoadFolder, FolderInfo&, AssetNode*);
	PROTECTED_FUNCTION(AssetNode*, LoadAssetNode, const String&, AssetNode*, const TimeStamp&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;

CLASS_BASES_META(o2::AssetsTree::AssetNode)
{
	BASE_CLASS(o2::AssetInfo);
}
END_META;
CLASS_FIELDS_META(o2::AssetsTree::AssetNode)
{
	PUBLIC_FIELD(tree);
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(time);
	PUBLIC_FIELD(parent);
	PUBLIC_FIELD(children).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AssetsTree::AssetNode)
{

	PUBLIC_FUNCTION(AssetNode*, AddChild, AssetNode*);
	PUBLIC_FUNCTION(bool, RemoveChild, AssetNode*, bool);
	PUBLIC_FUNCTION(void, SetParent, AssetNode*);
	PRIVATE_FUNCTION(void, OnDeserialized, const DataNode&);
	PRIVATE_FUNCTION(void, SetTree, AssetsTree*);
}
END_META;
