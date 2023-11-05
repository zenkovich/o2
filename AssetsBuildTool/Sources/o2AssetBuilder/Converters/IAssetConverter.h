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
        virtual void ConvertAsset(const AssetInfo& node);

        // Removes asset by path
        virtual void RemoveAsset(const AssetInfo& node);

        // Moves asset to new path
        virtual void MoveAsset(const AssetInfo& nodeFrom, const AssetInfo& nodeTo);

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
// --- META ---

CLASS_BASES_META(o2::IAssetConverter)
{
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::IAssetConverter)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAssetsBuilder);
}
END_META;
CLASS_METHODS_META(o2::IAssetConverter)
{

    FUNCTION().PUBLIC().SIGNATURE(Vector<const Type*>, GetProcessingAssetsTypes);
    FUNCTION().PUBLIC().SIGNATURE(void, ConvertAsset, const AssetInfo&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAsset, const AssetInfo&);
    FUNCTION().PUBLIC().SIGNATURE(void, MoveAsset, const AssetInfo&, const AssetInfo&);
    FUNCTION().PUBLIC().SIGNATURE(Vector<UID>, AssetsPostProcess);
    FUNCTION().PUBLIC().SIGNATURE(void, Reset);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAssetsBuilder, AssetsBuilder*);
}
END_META;
// --- END META ---
