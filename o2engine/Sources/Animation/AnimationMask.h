#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Serialization.h"
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
