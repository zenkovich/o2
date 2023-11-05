#include "o2/stdafx.h"
#include "WidgetState.h"

#include "o2/Scene/UI/Widget.h"

namespace o2
{
    WidgetState::WidgetState()
    {}

    WidgetState::WidgetState(const WidgetState& state):
        name(state.name), mState(state.mState), 
        offStateAnimationSpeed(state.offStateAnimationSpeed), state(this), animationAsset(this), animationClip(this)
    {
        mAnimation = state.mAnimation;
        player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
        player.relTime = mState ? 1.0f:0.0f;
    }

    WidgetState::~WidgetState()
    {}

    WidgetState::operator bool()
    {
        return GetState();
    }

    bool WidgetState::operator==(const WidgetState& other) const
    {
        return false;
    }

    void WidgetState::SetOwner(Widget* owner, bool errors)
    {
        mOwner = owner;
        player.SetTarget(owner, errors);
        player.relTime = mState ? 1.0f : 0.0f;
    }

    void WidgetState::SetAnimationAsset(const AnimationAssetRef& asset)
    {
        mAnimation = asset;
        player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
    }

    const AnimationAssetRef& WidgetState::GetAnimationAsset() const
    {
        return mAnimation;
    }

    void WidgetState::SetAnimationClip(const AnimationClip& animation)
    {
        if (mAnimation && mAnimation.IsInstance())
            mAnimation->animation = animation;
        else
        {
            mAnimation.SetInstance(mnew AnimationAsset(animation));
            player.SetClip(&mAnimation->animation);
        }
    }

    AnimationClip& WidgetState::GetAnimationClip()
    {
        if (mAnimation)
            return mAnimation->animation;

        static AnimationClip empty;
        return empty;
    }

    void WidgetState::SetState(bool state)
    {
         if (mState == state && !player.IsPlaying())
             return;

        mState = state;

        if (state)
        {
            player.speed = 1.0f;
            player.PlayForward();

            onStateBecomesTrue();
        }
        else
        {
            player.speed = offStateAnimationSpeed;
            player.PlayBack();

            onStateBecomesFalse();
        }
    }

    void WidgetState::SetStateForcible(bool state)
    {
        if (mState == state)
        {
            if ((state && Math::Equals(player.GetRelTime(), 1.0f)) ||
                (!state && Math::Equals(player.GetRelTime(), 0.0f)))
            {
                return;
            }
        }

        mState = state;

        if (mState)
        {
            player.GoToEnd();
            player.Stop();
            onStateBecomesTrue();
            onStateFullyTrue();
        }
        else
        {
            player.GoToBegin();
            player.Stop();
            onStateBecomesFalse();
            onStateFullyFalse();
        }
    }

    bool WidgetState::GetState() const
    {
        return mState;
    }

    void WidgetState::Update(float dt)
    {
        if (player.IsPlaying())
        {
            player.Update(dt);

            if (!player.IsPlaying())
            {
                if (mState) onStateFullyTrue();
                else onStateFullyFalse();
            }
        }
    }

    void WidgetState::OnAnimationChanged()
    {
        player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
    }

    void WidgetState::OnDeserialized(const DataValue& node)
    {
        player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
    }

    void WidgetState::OnDeserializedDelta(const DataValue& node, const IObject& origin)
    {
        OnDeserialized(node);
    }

    WidgetState& WidgetState::operator=(bool state)
    {
        SetState(state);
        return *this;
    }
}
// --- META ---

DECLARE_CLASS(o2::WidgetState);
// --- END META ---
