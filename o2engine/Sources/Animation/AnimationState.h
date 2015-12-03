#pragma once

#include "Animation/Animation.h"
#include "Animation/AnimationMask.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Serialization.h"

namespace o2
{
	class Animatable;

	// ---------------
	// Animation state
	// ---------------
	class AnimationState: public ISerializable
	{
	public:
		String        name;       // State name
		Animation     animation;  // Animation
		AnimationMask mask;       // Animation mask
		float         weight;     // State weight
		float         workWeight; // State working weight, using for blendings

		// Default constructor
		AnimationState();

		// Constructor with name
		AnimationState(const String& name);

		SERIALIZABLE_IMPL(AnimationState);

		IOBJECT(AnimationState)
		{
			SRLZ_FIELD(name);
			SRLZ_FIELD(animation);
			SRLZ_FIELD(mask);
			SRLZ_FIELD(weight);
			FIELD(workWeight);
		}

	protected:
		Ptr<Animatable> mOwner; // Animatable owner

		friend class Animatable;
		friend class ANimation;

		template<typename _type>
		friend class AnimatedValue;
	};
	typedef Vector<Ptr<AnimationState>> AnimationStatesVec;
}