#pragma once

#include "Utils/IObject.h"
#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Reflection/Type.h"

namespace o2
{
	class AssetsBuilder;

	// --------------------------------------
	// Asset converter interface
	// Converts assets with specialized types
	//---------------------------------------
	class IAssetConverter: public IObject
	{
	protected:
		// Assets builder pointer
		Ptr<AssetsBuilder> mAssetsBuilder;

	public:
		// Virtual destructor
		virtual ~IAssetConverter() {}

		// Returns vector of processing assets types
		virtual Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts asset by path
		virtual void ConvertAsset(const String& path);

		// Removes asset by path
		virtual void RemoveAsset(const String& path);

		// Moves asset to new path
		virtual void MoveAsset(const String& pathFrom, const String& pathTo);

		// Post processing
		virtual void AssetsPostProcess();

		// Resets converter
		virtual void Reset();

		// Sets owner assets builder
		void SetAssetsBuilder(Ptr<AssetsBuilder> builder);

		IOBJECT(IAssetConverter) {}
	};
}