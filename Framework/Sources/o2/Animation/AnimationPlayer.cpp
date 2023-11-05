#include "o2/stdafx.h"
#include "AnimationPlayer.h"

#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/AnimationState.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
    AnimationPlayer::AnimationPlayer(IObject* target /*= nullptr*/, AnimationClip* clip /*= nullptr*/):
        mTarget(target), mClip(clip)
    {
        SetTarget(target);
        SetClip(clip);
    }

    AnimationPlayer::~AnimationPlayer()
    {
        for (auto player : mTrackPlayers)
            delete player;

        mTrackPlayers.Clear();

        if (mClip && mClipOwner)
            delete mClip;
    }

    void AnimationPlayer::SetTarget(IObject* target, bool errors /*= true*/)
    {
        mTarget = target;
        BindTracks(errors);
    }

    IObject* AnimationPlayer::GetTarget() const
    {
        return mTarget;
    }

    void AnimationPlayer::SetClip(AnimationClip* clip, bool owner /*= false*/)
    {
        if (mClip)
        {
            mClip->onTrackAdded -= THIS_FUNC(OnClipTrackAdded);
            mClip->onTrackRemove -= THIS_FUNC(OnClipTrackRemove);
            mClip->onDurationChange -= THIS_FUNC(OnClipDurationChanged);
        }

        mClip = clip;
        mClipOwner = owner;

        if (mClip)
        {
            mClip->onTrackAdded += MakeSubscription(this, &thisclass::OnClipTrackAdded, [&]() { mClip = nullptr; });
            mClip->onTrackRemove += THIS_SUBSCRIPTION(OnClipTrackRemove, [&]() { mClip = nullptr; });
            mClip->onDurationChange += THIS_SUBSCRIPTION(OnClipDurationChanged, [&]() { mClip = nullptr; });
        }

        BindTracks(true);
    }

    AnimationClip* AnimationPlayer::GetClip() const
    {
        return mClip;
    }

    const Vector<IAnimationTrack::IPlayer*>& AnimationPlayer::GetTrackPlayers() const
    {
        return mTrackPlayers;
    }

    void AnimationPlayer::BindTracks(bool errors)
    {
        for (auto player : mTrackPlayers)
        {
            onTrackPlayerRemove(player);
            delete player;
        }

        mTrackPlayers.Clear();

        if (!mTarget || !mClip)
            return;

        const ObjectType* type = dynamic_cast<const ObjectType*>(&mTarget->GetType());
        void* castedTarget = type->DynamicCastFromIObject(mTarget);

        for (auto track : mClip->mTracks)
            BindTrack(type, castedTarget, track, errors);

        mLoop = mClip->mLoop;
        mDuration = mClip->GetDuration();
        mBeginTime = 0.0f;
        mEndTime = mDuration;
    }

    void AnimationPlayer::BindTrack(const ObjectType* type, void* castedTarget, IAnimationTrack * track, bool errors)
    {
        const FieldInfo* fieldInfo = nullptr;
        auto targetPtr = type->GetFieldPtr(castedTarget, track->path, fieldInfo);

        if (!fieldInfo)
        {
            if (errors)
                o2Debug.LogWarning("Can't find '" + track->path + "' for animating");
        }
        else
        {
            auto trackPlayer = track->CreatePlayer();
            trackPlayer->SetTrack(track);
            trackPlayer->mOwnerPlayer = this;

            if (fieldInfo->GetType()->GetUsage() == Type::Usage::Property)
                trackPlayer->SetTargetProxyVoid(fieldInfo->GetType()->GetValueProxy(targetPtr));
            else
                trackPlayer->SetTargetVoid(targetPtr);

            mTrackPlayers.Add(trackPlayer);

            onTrackPlayerAdded(trackPlayer);
        }
    }

    void AnimationPlayer::OnClipTrackAdded(IAnimationTrack* track)
    {
        const ObjectType* type = dynamic_cast<const ObjectType*>(&mTarget->GetType());
        void* castedTarget = type->DynamicCastFromIObject(mTarget);

        BindTrack(type, castedTarget, track, false);
    }

    void AnimationPlayer::OnClipTrackRemove(IAnimationTrack* track)
    {
        mTrackPlayers.RemoveFirst([track, this](auto& x) { return x->GetTrack() == track; onTrackPlayerRemove(x); });
    }

    void AnimationPlayer::OnClipDurationChanged(float duration)
    {
        mDuration = duration;
        mEndTime = duration;
    }

    void AnimationPlayer::Evaluate()
    {
        for (auto trackPlayer : mTrackPlayers)
            trackPlayer->ForceSetTime(mInDurationTime, mDuration);
    }
}
// --- META ---

DECLARE_CLASS(o2::AnimationPlayer);
// --- END META ---
