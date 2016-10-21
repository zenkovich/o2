#pragma once

#include "IAssetConverter.h"

namespace o2
{
	// ----------------
	// Folder converter
	// ----------------
	class FolderAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts folder by path
		void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes folder by path
		void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves folder to new path
		void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		// Post processing folders
		Vector<UID> AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(FolderAssetConverter);

	protected:
		Vector<String> mRemovedFolders; // Removes folders
	};
}