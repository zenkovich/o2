#pragma once

#include "Assets/AssetsTree.h"
#include "Utils/IObject.h"
#include "Utils/Memory/MemoryManager.h"

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
	public:
		// Virtual destructor
		virtual ~IAssetConverter() {}

		// Returns vector of processing assets types
		virtual Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts asset by path
		virtual void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes asset by path
		virtual void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves asset to new path
		virtual void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		// Post processing
		virtual Vector<UID> AssetsPostProcess();

		// Resets converter
		virtual void Reset();

		// Sets owner assets builder
		void SetAssetsBuilder(AssetsBuilder* builder);

		IOBJECT(IAssetConverter);

	protected:
		// Assets builder pointer
		AssetsBuilder* mAssetsBuilder = nullptr;
	};
}