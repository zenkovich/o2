#pragma once

#include "Animation/Animation.h"
#include "Animation/AnimationMask.h"
#include "Utils/Containers/Vector.h"

namespace o2
{
	class Animatable;

	// ---------------
	// Animation state
	// ---------------
	class AnimationState
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

	protected:
		Ptr<Animatable> mOwner; // Animatable owner

		friend class Animatable;
		friend class ANimation;

		template<typename _type>
		friend class AnimatedValue;
	};
	typedef Vector<Ptr<AnimationState>> AnimationStatesVec;
}