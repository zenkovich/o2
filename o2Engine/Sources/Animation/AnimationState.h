#pragma once

#include "Animation/Animation.h"
#include "Animation/AnimationMask.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Serializable.h"

namespace o2
{
	class Animatable;

	// ---------------
	// Animation state
	// ---------------
	class AnimationState: public ISerializable
	{
	public:
		String        name;       // State name @SERIALIZABLE
		Animation     animation;  // Animation @SERIALIZABLE
		AnimationMask mask;       // Animation mask @SERIALIZABLE
		float         weight;     // State weight @SERIALIZABLE
		float         workWeight; // State working weight, using for blendings

		// Default constructor
		AnimationState();

		// Constructor with name
		AnimationState(const String& name);

		SERIALIZABLE(AnimationState);

	protected:
		Animatable* mOwner; // Animatable owner

		friend class Animatable;
		friend class ANimation;

		template<typename _type>
		friend class AnimatedValue;
	};
	typedef Vector<AnimationState*> AnimationStatesVec;
}