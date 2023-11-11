#include "o2/stdafx.h"
#include "AnimationMask.h"

namespace o2
{
    float AnimationMask::GetNodeWeight(const String& node) const
    {
        auto fnd = weights.find(node);
        if (fnd != weights.End())
            return fnd->second;

        return 1.0f;
    }
}
// --- META ---

DECLARE_CLASS(o2::AnimationMask, o2__AnimationMask);
// --- END META ---
