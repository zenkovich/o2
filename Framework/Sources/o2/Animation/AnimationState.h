#pragma once

#include "o2/Animation/AnimationMask.h"
#include "o2/Animation/AnimationPlayer.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"

namespace o2
{
	class AnimationComponent;

	// ---------------
	// Animation state
	// ---------------
	class AnimationState: public ISerializable
	{
	public:
		String        name; // State name @SERIALIZABLE
		AnimationMask mask; // Animation mask @SERIALIZABLE

		float blend = 1.0f; // State blending coefficient in 0..1 Used for blending

		AnimationPlayer player; // Animation player

	public:
		// Default constructor
		AnimationState() = default;

		// Constructor with name
		AnimationState(const String& name);

		// Sets state weight
		void SetWeight(float weight);

		// Returns state weight
		float GetWeight() const;

		// Sets animation
		void SetAnimation(const AnimationAssetRef& animationAsset);

		// Returns animation
		const AnimationAssetRef& GetAnimation() const;

		SERIALIZABLE(AnimationState);

	protected:
		AnimationComponent* mOwner = nullptr; // Animation state owner component
		AnimationAssetRef   mAnimation;       // Animation @SERIALIZABLE
		float               mWeight = 1.0f;   // State weight @SERIALIZABLE

		friend class AnimationComponent;
		friend class AnimationClip;

		template<typename _type>
		friend class AnimationTrack;
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
	PUBLIC_FIELD(mask).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(blend);
	PUBLIC_FIELD(player);
	PROTECTED_FIELD(mOwner);
	PROTECTED_FIELD(mAnimation).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mWeight).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationState)
{

	PUBLIC_FUNCTION(void, SetWeight, float);
	PUBLIC_FUNCTION(float, GetWeight);
	PUBLIC_FUNCTION(void, SetAnimation, const AnimationAssetRef&);
	PUBLIC_FUNCTION(const AnimationAssetRef&, GetAnimation);
}
END_META;
