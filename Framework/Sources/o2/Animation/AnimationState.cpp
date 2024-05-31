#include "o2/stdafx.h"
#include "AnimationState.h"

#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
    FORWARD_REF_IMPL(AnimationComponent);

    AnimationState::AnimationState(const String& name):
        name(name)
    {
        player->onTrackPlayerAdded = [&](auto track) { OnTrackPlayerAdded(track); };
        player->onTrackPlayerRemove = [&](auto track) { OnTrackPlayerRemove(track); };
    }

    void AnimationState::SetWeight(float weight)
    {
        mWeight = weight;
    }

    float AnimationState::GetWeight() const
    {
        return mWeight;
    }

    void AnimationState::SetAnimation(const AssetRef<AnimationAsset>& animationAsset)
    {
        mAnimation = animationAsset;
        player->SetClip(mAnimation ? mAnimation->animation : nullptr);
    }

    const AssetRef<AnimationAsset>& AnimationState::GetAnimation() const
    {
        return mAnimation;
    }

    void AnimationState::OnAnimationChanged()
    {
        player->SetClip(mAnimation ? mAnimation->animation : nullptr);
    }

    void AnimationState::OnTrackPlayerAdded(const Ref<IAnimationTrack::IPlayer>& trackPlayer)
    {
        if (mOwner)
            mOwner.Lock()->OnStateAnimationTrackAdded(Ref(this), trackPlayer);
    }

    void AnimationState::OnTrackPlayerRemove(const Ref<IAnimationTrack::IPlayer>& trackPlayer)
    {
        if (mOwner)
            mOwner.Lock()->OnStateAnimationTrackRemoved(Ref(this), trackPlayer);
    }

    void AnimationState::OnDeserialized(const DataValue& node)
    {
        player->SetClip(mAnimation ? mAnimation->animation : nullptr);
    }

}
// --- META ---

DECLARE_CLASS(o2::AnimationState, o2__AnimationState);
// --- END META ---
