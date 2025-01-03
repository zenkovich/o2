#pragma once

#include "o2/Animation/AnimationMask.h"
#include "o2/Animation/AnimationPlayer.h"
#include "o2/Assets/AssetRef.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Utils/Basic/ICloneable.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"

namespace o2
{
    FORWARD_CLASS_REF(AnimationComponent);

    // ---------------
    // Animation state
    // ---------------
    class AnimationState: public ISerializable, public RefCounterable, public ICloneableRef
    {
    public:
        String        name;            // State name @SERIALIZABLE
        AnimationMask mask;            // Animation mask @SERIALIZABLE
        bool          autoPlay = true; // True, if state should be played automatically @SERIALIZABLE

        float blend = 1.0f; // State blending coefficient in 0..1 Used for blending

        Ref<AnimationPlayer> player = mmake<AnimationPlayer>(); // Animation player

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
        void SetAnimation(const AssetRef<AnimationAsset>& animationAsset);

        // Returns animation
        const AssetRef<AnimationAsset>& GetAnimation() const;

        SERIALIZABLE(AnimationState);
        CLONEABLE_REF(AnimationState);

    protected:
        WeakRef<AnimationComponent> mOwner; // Animation state owner component

        AssetRef<AnimationAsset> mAnimation; // Animation @SERIALIZABLE @EDITOR_PROPERTY @INVOKE_ON_CHANGE(OnAnimationChanged)

        float mWeight = 1.0f; // State weight @SERIALIZABLE @EDITOR_PROPERTY

    protected:
        // Called when animation changed from editor
        void OnAnimationChanged();

        // Called when player has added new track
        void OnTrackPlayerAdded(const Ref<IAnimationTrack::IPlayer>& trackPlayer);

        // Called when player is removing track
        void OnTrackPlayerRemove(const Ref<IAnimationTrack::IPlayer>& trackPlayer);

        // It is called when object was deserialized; sets animation into player
        void OnDeserialized(const DataValue& node) override;

        friend class AnimationComponent;
        friend class AnimationClip;

        template<typename _type>
        friend class AnimationTrack;

		friend class AnimationSubTrack;
    };
}
// --- META ---

CLASS_BASES_META(o2::AnimationState)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
    BASE_CLASS(o2::ICloneableRef);
}
END_META;
CLASS_FIELDS_META(o2::AnimationState)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(name);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mask);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(autoPlay);
    FIELD().PUBLIC().DEFAULT_VALUE(1.0f).NAME(blend);
    FIELD().PUBLIC().DEFAULT_VALUE(mmake<AnimationPlayer>()).NAME(player);
    FIELD().PROTECTED().NAME(mOwner);
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
    FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, const AssetRef<AnimationAsset>&);
    FUNCTION().PUBLIC().SIGNATURE(const AssetRef<AnimationAsset>&, GetAnimation);
    FUNCTION().PROTECTED().SIGNATURE(void, OnAnimationChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTrackPlayerAdded, const Ref<IAnimationTrack::IPlayer>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTrackPlayerRemove, const Ref<IAnimationTrack::IPlayer>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
