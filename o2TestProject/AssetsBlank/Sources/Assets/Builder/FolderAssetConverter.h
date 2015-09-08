#pragma once

#include "IAssetConverter.h"

namespace o2
{
	class FolderAssetConverter: public IAssetConverter
	{
	public:
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		void ConvertAsset(const String& path);
		void RemoveAsset(const String& path);
		void MoveAsset(const String& pathFrom, const String& pathTo);

		IOBJECT(FolderAssetConverter)
		{
			BASE_CLASS(IAssetConverter);
		}
	};
}