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
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts image
		void ConvertAsset(const String& path);

		// Removes image
		void RemoveAsset(const String& path);

		// Moves image to new path
		void MoveAsset(const String& pathFrom, const String& pathTo);

		IOBJECT(ImageAssetConverter)
		{
			BASE_CLASS(IAssetConverter);
		}
	};
}
