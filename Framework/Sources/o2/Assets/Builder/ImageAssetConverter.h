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
		void ConvertAsset(const AssetsTree::AssetInfo& node);

		// Removes image
		void RemoveAsset(const AssetsTree::AssetInfo& node);

		// Moves image to new path
		void MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo);

		IOBJECT(ImageAssetConverter);
	};
}

CLASS_BASES_META(o2::ImageAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);
}
END_META;
CLASS_FIELDS_META(o2::ImageAssetConverter)
{
}
END_META;
CLASS_METHODS_META(o2::ImageAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetsTree::AssetInfo&, const AssetsTree::AssetInfo&);
}
END_META;
