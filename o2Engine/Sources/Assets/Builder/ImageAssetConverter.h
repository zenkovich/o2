#pragma once

#include "IAssetConverter.h"

namespace o2
{
	// ---------------------
	// Image asset converter
	// ---------------------
	class ImageAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<const Type*> GetProcessingAssetsTypes() const;

		// Converts image
		void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes image
		void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves image to new path
		void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		IOBJECT(ImageAssetConverter);
	};
}
