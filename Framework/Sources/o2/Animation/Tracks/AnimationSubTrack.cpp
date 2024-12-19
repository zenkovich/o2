#include "o2/stdafx.h"
#include "AnimationSubTrack.h"

#include "o2/Animation/AnimationState.h"
#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
    AnimationSubTrack::AnimationSubTrack()
    {}

    AnimationSubTrack::AnimationSubTrack(const AnimationSubTrack& other):
		IAnimationTrack(other), mBeginTime(other.mBeginTime), mDuration(other.mDuration),
		mSubTrackBeginOffset(other.mSubTrackBeginOffset), mSubTrackEndOffset(other.mSubTrackEndOffset)
	{}

    AnimationSubTrack& AnimationSubTrack::operator=(const AnimationSubTrack& other)
    {
        IAnimationTrack::operator=(other);

		mBeginTime = other.mBeginTime;
		mDuration = other.mDuration;
		mSubTrackBeginOffset = other.mSubTrackBeginOffset;
		mSubTrackEndOffset = other.mSubTrackEndOffset;

        onKeysChanged();

        return *this;
    }

    float AnimationSubTrack::GetDuration() const
    {
        return mDuration;
    }

    Ref<IAnimationTrack::IPlayer> AnimationSubTrack::CreatePlayer() const
    {
        return mmake<Player>();
    }

	void AnimationSubTrack::SetBeginTime(float beginTime)
	{
		mBeginTime = beginTime;

		onKeysChanged();
	}

	float AnimationSubTrack::GetBeginTime() const
	{
		return mBeginTime;
	}

	void AnimationSubTrack::SetSubTrackDuration(float duration)
	{
		mDuration = duration;

		onKeysChanged();

	}

	float AnimationSubTrack::GetSubTrackDuration() const
	{
		return mDuration;
	}

	void AnimationSubTrack::SetSubTrackBeginOffset(float offset)
	{
		mSubTrackBeginOffset = offset;

		onKeysChanged();
	}

	float AnimationSubTrack::GetSubTrackBeginOffset() const
	{
		return mSubTrackBeginOffset;
	}

	void AnimationSubTrack::SetSubTrackEndOffset(float offset)
	{
		mSubTrackEndOffset = offset;

		onKeysChanged();
	}

	float AnimationSubTrack::GetSubTrackEndOffset() const
	{
		return mSubTrackEndOffset;
	}

	void AnimationSubTrack::OnDeserialized(const DataValue& node)
    {}

    AnimationSubTrack::Player::Player():
        IPlayer()
    {}

    AnimationSubTrack::Player::~Player()
    {}

    void AnimationSubTrack::Player::SetTarget(IAnimation* value)
    {
        mTarget = value;
        mTargetDelegate.Clear();
    }

    void AnimationSubTrack::Player::SetTarget(IAnimation* value, const Function<void()>& changeEvent)
    {
        mTarget = value;
        mTargetDelegate = changeEvent;
    }

    void AnimationSubTrack::Player::SetTargetDelegate(const Function<void()>& changeEvent)
    {
        mTargetDelegate = changeEvent;
    }

    void AnimationSubTrack::Player::SetTrack(const Ref<AnimationSubTrack>& track)
    {
        mTrack = track;
        IPlayer::SetTrack(track);
    }

    const Ref<AnimationSubTrack>& AnimationSubTrack::Player::GetTrackT() const
    {
        return mTrack;
    }

    void AnimationSubTrack::Player::SetTargetVoid(void* target)
    {
        SetTarget((IAnimation*)target);
    }

    void AnimationSubTrack::Player::SetTargetVoid(void* target, const Function<void()>& changeEvent)
    {
        SetTarget((IAnimation*)target, changeEvent);
    }

	void AnimationSubTrack::Player::SetTargetProxy(const Ref<IAbstractValueProxy>& targetProxy)
    {}

    void AnimationSubTrack::Player::SetTrack(const Ref<IAnimationTrack>& track)
    {
        SetTrack(DynamicCast<AnimationSubTrack>(track));
    }

    Ref<IAnimationTrack> AnimationSubTrack::Player::GetTrack() const
    {
        return mTrack;
    }

    void AnimationSubTrack::Player::Evaluate()
    {
        if (!mTrack)
            return;

        if (mTarget)
            mTarget->SetTime(mTime);

        mTargetDelegate();
    }

    void AnimationSubTrack::Player::RegMixer(const Ref<AnimationState>& state, const String& path)
    {
        state->mOwner.Lock()->RegSubTrack(Ref(this), path, state);
    }
}
// --- META ---

DECLARE_CLASS(o2::AnimationSubTrack, o2__AnimationSubTrack);

DECLARE_CLASS(o2::AnimationSubTrack::Player, o2__AnimationSubTrack__Player);
// --- END META ---
