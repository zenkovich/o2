#include "IAssetConverter.h"

namespace o2
{
	IOBJECT_CPP(IAssetConverter);

	Vector<Type::Id> IAssetConverter::GetProcessingAssetsTypes() const
	{
		return Vector<Type::Id>();
	}

	void IAssetConverter::ConvertAsset(const String& path)
	{
	}

	void IAssetConverter::RemoveAsset(const String& path)
	{
	}

	void IAssetConverter::MoveAsset(const String& pathFrom, const String& pathTo)
	{
	}

}