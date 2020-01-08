#include "o2/stdafx.h"
#include "AnimationMask.h"

namespace o2
{
	float AnimationMask::GetNodeWeight(const String& node) const
	{
		if (weights.ContainsKey(node))
			return weights.Get(node);

		return 1.0f;
	}
}

DECLARE_CLASS(o2::AnimationMask);
