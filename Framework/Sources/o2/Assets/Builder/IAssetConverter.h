#pragma once

#include "o2/Assets/AssetsTree.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Memory/MemoryManager.h"

#include "o2/Utils/Reflection/Type.h"

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
		virtual Vector<const Type*> GetProcessingAssetsTypes() const;

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

CLASS_BASES_META(o2::IAssetConverter)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::IAssetConverter)
{
	PROTECTED_FIELD(mAssetsBuilder);
}
END_META;
CLASS_METHODS_META(o2::IAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetTree::AssetNode&, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
	PUBLIC_FUNCTION(void, SetAssetsBuilder, AssetsBuilder*);
}
END_META;
