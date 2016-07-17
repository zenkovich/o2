#include "AnimationTask.h"

namespace o2
{

	AnimationTask::AnimationTask(const Animation& animation, float delay /*= 0.0f*/):
		mRemainingDelay(delay), mAnimation(animation)
	{
		if (Math::Equals(delay, 0.0f))
			mAnimation.RewindAndPlay();
	}

	void AnimationTask::Update(float dt)
	{
		if (mRemainingDelay > 0.0f)
		{
			mRemainingDelay -= dt;
			if (mRemainingDelay < FLT_EPSILON)
				mAnimation.RewindAndPlay();

			return;
		}

		mAnimation.Update(dt);
	}

	bool AnimationTask::IsDone() const
	{
		return mRemainingDelay < FLT_EPSILON && !mAnimation.IsPlaying();
	}
}