#pragma once

#include "o2/Animation/Animation.h"
#include "o2/Animation/AnimationMask.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class AnimationComponent;

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

	public:
		// Default constructor
		AnimationState();

		// Constructor with name
		AnimationState(const String& name);

		SERIALIZABLE(AnimationState);

	protected:
		AnimationComponent* mOwner; // Animatable owner

		friend class AnimationComponent;
		friend class Animation;

		template<typename _type>
		friend class AnimatedValue;
	};
}

CLASS_BASES_META(o2::AnimationState)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationState)
{
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(animation).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mask).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(weight).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(workWeight);
	PROTECTED_FIELD(mOwner);
}
END_META;
CLASS_METHODS_META(o2::AnimationState)
{
}
END_META;
