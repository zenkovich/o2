#include "o2/stdafx.h"
#include "IAssetConverter.h"

#include "o2AssetBuilder/AssetsBuilder.h"

namespace o2
{
    Vector<const Type*> IAssetConverter::GetProcessingAssetsTypes() const
    {
        return Vector<const Type*>();
    }

    void IAssetConverter::ConvertAsset(const AssetInfo& node)
    {}

    void IAssetConverter::RemoveAsset(const AssetInfo& node)
    {}

    void IAssetConverter::MoveAsset(const AssetInfo& nodeFrom, const AssetInfo& nodeTo)
    {}

    Vector<UID> IAssetConverter::AssetsPostProcess()
    {
        return Vector<UID>();
    }

    void IAssetConverter::Reset()
    {}

    void IAssetConverter::SetAssetsBuilder(AssetsBuilder* builder)
    {
        mAssetsBuilder = builder;
    }

}
// --- META ---

DECLARE_CLASS(o2::IAssetConverter);
// --- END META ---
