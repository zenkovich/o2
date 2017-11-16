#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Serializable.h"
#include "Utils/String.h"

namespace o2
{
	// --------------------------
	// Animation tree weight mask
	// --------------------------
	class AnimationMask: public ISerializable
	{
	public:
		Dictionary<String, float> weights; // Masked nodes weights @SERIALIZABLE

		// Returns node masked weight. 1.0f is default
		float GetNodeWeight(const String& node) const;

		SERIALIZABLE(AnimationMask);
	};
}

CLASS_BASES_META(o2::AnimationMask)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationMask)
{
	PUBLIC_FIELD(weights).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationMask)
{

	PUBLIC_FUNCTION(float, GetNodeWeight, const String&);
}
END_META;
