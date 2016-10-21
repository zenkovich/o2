#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetInfo.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/ITree.h"

namespace o2
{
	class LogStream;

	// --------------------------------
	// Asset tree structure information
	// --------------------------------
	class AssetTree
	{
	public:
		// ----------------------
		// Asset information node
		// ----------------------
		struct AssetNode: public ITreeNode<AssetNode>, public AssetInfo
		{ 
			Asset::IMetaInfo* mMeta; // Asset meta
			TimeStamp         mTime; // Asset edited time

		public:
			// Default constructor
			AssetNode();

			// Constructor
			AssetNode(const String& path, UID id, Type::Id type);

			// Destructor
			~AssetNode();

			SERIALIZABLE(AssetNode);
		};
		typedef Vector<AssetNode*> AssetsVec;

	public:
		String     mPath;       // Assets information root path
		LogStream* mLog;        // Log stream
		AssetsVec  mRootAssets; // Root path assets
		AssetsVec  mAllAssets;  // All assets

	public:
		// Default constructor
		AssetTree();

		// Destructor
		~AssetTree();

		// Builds tree for folder
		void BuildTree(const String& path);

		// Rebuilds tree for current folder
		void RebuildTree();

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

	protected:
		// Loads assets nodes from folder
		void LoadFolder(FolderInfo& folder, AssetNode* parentAsset);

		// Loads and returns asset by path
		AssetNode* LoadAsset(const String& path, AssetNode* parent, const TimeStamp& time);
	};
}
