#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	// --------------------------
	// Animation tree weight mask
	// --------------------------
	class AnimationMask: public ISerializable
	{
	public:
		Map<String, float> weights; // Masked nodes weights @SERIALIZABLE

	public:
		// Returns node masked weight. 1.0f is default
		float GetNodeWeight(const String& node) const;

		SERIALIZABLE(AnimationMask);
	};
}
// --- META ---

CLASS_BASES_META(o2::AnimationMask)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationMask)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(weights);
}
END_META;
CLASS_METHODS_META(o2::AnimationMask)
{

	FUNCTION().PUBLIC().SIGNATURE(float, GetNodeWeight, const String&);
}
END_META;
// --- END META ---
