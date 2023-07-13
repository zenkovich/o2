#pragma once

#include "o2/Animation/AnimationMask.h"
#include "o2/Animation/AnimationPlayer.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Assets/AssetRef.h"

namespace o2
{
	class AnimationComponent;

	// ---------------
	// Animation state
	// ---------------
	class AnimationState: public ISerializable
	{
	public:
		String        name;            // State name @SERIALIZABLE
		AnimationMask mask;            // Animation mask @SERIALIZABLE
		bool          autoPlay = true; // True, if state should be played automatically @SERIALIZABLE

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
		AnimationAssetRef   mAnimation;        // Animation @SERIALIZABLE @EDITOR_PROPERTY @INVOKE_ON_CHANGE(OnAnimationChanged)
		float               mWeight = 1.0f;   // State weight @SERIALIZABLE @EDITOR_PROPERTY

	protected:
		// Called when animation changed from editor
		void OnAnimationChanged();

		// Called when player has added new track
		void OnTrackPlayerAdded(IAnimationTrack::IPlayer* trackPlayer);

		// Called when player is removing track
		void OnTrackPlayerRemove(IAnimationTrack::IPlayer* trackPlayer);

		// It is called when object was deserialized; sets animation into player
		void OnDeserialized(const DataValue& node) override;

		friend class AnimationComponent;
		friend class AnimationClip;

		template<typename _type>
		friend class AnimationTrack;
	};
}
// --- META ---

CLASS_BASES_META(o2::AnimationState)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationState)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(name);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mask);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(autoPlay);
	FIELD().PUBLIC().DEFAULT_VALUE(1.0f).NAME(blend);
	FIELD().PUBLIC().NAME(player);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwner);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().INVOKE_ON_CHANGE_ATTRIBUTE(OnAnimationChanged).SERIALIZABLE_ATTRIBUTE().NAME(mAnimation);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mWeight);
}
END_META;
CLASS_METHODS_META(o2::AnimationState)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWeight, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWeight);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, const AnimationAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(const AnimationAssetRef&, GetAnimation);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAnimationChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTrackPlayerAdded, IAnimationTrack::IPlayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTrackPlayerRemove, IAnimationTrack::IPlayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
