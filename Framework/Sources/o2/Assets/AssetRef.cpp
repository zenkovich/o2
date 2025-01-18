#include "o2/stdafx.h"
#include "AssetRef.h"

#include "o2/Assets/Assets.h"

namespace o2
{
    const Type& BaseAssetRef::GetAssetType() const
    {
        return TypeOf(Asset);
    }
}

DECLARE_TEMPLATE_CLASS(o2::AssetRef<o2::Asset>);
// --- META ---

DECLARE_CLASS(o2::BaseAssetRef, o2__BaseAssetRef);
// --- END META ---
