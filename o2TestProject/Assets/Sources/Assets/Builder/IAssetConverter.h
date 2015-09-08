#pragma once

#include "Utils/IObject.h"
#include "Utils/Memory/MemoryManager.h"
#include "Utils/Reflection/Type.h"

namespace o2
{
	class IAssetConverter: public IObject
	{
	public:
		virtual ~IAssetConverter() {}

		virtual Vector<Type::Id> GetProcessingAssetsTypes() const;
		virtual void ConvertAsset(const String& path);
		virtual void RemoveAsset(const String& path);
		virtual void MoveAsset(const String& pathFrom, const String& pathTo);

		IOBJECT(IAssetConverter) {}
	};
}