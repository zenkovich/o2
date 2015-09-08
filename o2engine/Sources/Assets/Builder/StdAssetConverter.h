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
		void ConvertAsset(const String& path);

		// Removes asset
		void RemoveAsset(const String& path);

		// Moves asset to new path
		void MoveAsset(const String& pathFrom, const String& pathTo);

		IOBJECT(StdAssetConverter)
		{
			BASE_CLASS(IAssetConverter);
		}
	};
}
