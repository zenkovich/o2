#pragma once

#include "IAssetConverter.h"

namespace o2
{
	// -----------------------------------------------------------------
	// Standard assets converter. Copying file and meta without changing
	// -----------------------------------------------------------------
	class StdAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Copies asset
		void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes asset
		void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves asset to new path
		void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		IOBJECT(StdAssetConverter);
	};
}
