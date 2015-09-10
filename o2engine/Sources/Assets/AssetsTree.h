#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetInfo.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Tree.h"

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
			Ptr<Asset::IMetaInfo> mMeta; // Asset meta
			TimeStamp             mTime; // Asset edited time

		public:
			// Default constructor
			AssetNode();

			// Constructor
			AssetNode(const String& path, AssetId id, Type::Id type);
		};
		typedef Vector<Ptr<AssetNode>> AssetsVec;

	public:
		String         mPath;       // Assets information root path
		Ptr<LogStream> mLog;        // Log stream
		AssetsVec      mRootAssets; // Root path assets
		AssetsVec      mAllAssets;  // All assets

	public:
		// Default constructor
		AssetTree();

		// Destructor
		~AssetTree();

		// Builds tree for folder
		void BuildTree(const String& path, Ptr<LogStream> log = nullptr);

		// Returns asset by path. (nullptr if not asset with path)
		Ptr<AssetNode> FindAsset(const String& path) const;

		// Returns asset by id. (nullptr if not asset with id)
		Ptr<AssetNode> FindAsset(AssetId id) const;

		// Returns asset info by path
		AssetInfo FindAssetInfo(const String& path) const;

		// Returns asset info by id
		AssetInfo FindAssetInfo(AssetId id) const;

		// Adds asset node information into structure
		Ptr<AssetNode> AddAsset(Ptr<AssetNode> asset);

		// Removes asset node information from structure
		void RemoveAsset(Ptr<AssetNode> asset, bool release = true);

		// Clears all information
		void Clear();

	protected:
		// Loads assets nodes from folder
		void LoadFolder(FolderInfo& folder, Ptr<AssetNode> parentAsset);

		// Loads and returns asset by path
		Ptr<AssetNode> LoadAsset(const String& path, Ptr<AssetNode> parent, const TimeStamp& time);
	};
}
