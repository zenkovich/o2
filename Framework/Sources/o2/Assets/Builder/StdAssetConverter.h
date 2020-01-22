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
		Vector<const Type*> GetProcessingAssetsTypes() const;

		// Copies asset
		void ConvertAsset(const AssetsTree::AssetInfo& node);

		// Removes asset
		void RemoveAsset(const AssetsTree::AssetInfo& node);

		// Moves asset to new path
		void MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo);

		IOBJECT(StdAssetConverter);
	};
}

CLASS_BASES_META(o2::StdAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);
}
END_META;
CLASS_FIELDS_META(o2::StdAssetConverter)
{
}
END_META;
CLASS_METHODS_META(o2::StdAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetsTree::AssetInfo&, const AssetsTree::AssetInfo&);
}
END_META;
