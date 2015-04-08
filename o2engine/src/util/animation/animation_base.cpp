#include "animation_base.h"

#include <math.h>
#include "util/math/math.h"

OPEN_O2_NAMESPACE

IAnimation::IAnimation():
mBeginPlayingCallback(NULL), mEndPlayingCallback(NULL), mTime(0), mPlaying(false), mPlayed(false), mBeginPlaying(0),
mEndPlaying(0), mDuration(0), mAnimationSpeed(1), mLoop(LoopType::None), mDirectionSign(1), mLastFrameTime(0),
mAbsoluteTime(0)
{
}

IAnimation::~IAnimation()
{
}

void IAnimation::Play(bool restart /*= false*/, bool stopAtEnd /*= true*/)
{
	Play(restart ? 0.0f:mAbsoluteTime, stopAtEnd ? mDuration:FLT_MAX);
}

void IAnimation::Play(float begin, float end)
{
	mBeginPlaying = begin;
	mEndPlaying = end;

	mPlaying = true;
	mPlayed = false;
	mAbsoluteTime = mBeginPlaying;

	mDirectionSign = Sign(mEndPlaying - mBeginPlaying);

	UpdateTime();
	mLastFrameTime = mTime;

	Evaluate();
	onChangedEvent.Invoke();
	CheckBeginCallback();
}

void IAnimation::Update(float dt)
{
	if (!mPlaying)
		return;

	mAbsoluteTime += mDirectionSign*mAnimationSpeed*dt;

	float endDiff = (mAbsoluteTime - mEndPlaying)*mDirectionSign*Sign(mAnimationSpeed);
	if (endDiff > 0)
	{
		mAbsoluteTime = mEndPlaying;
		mPlaying = false;
	}

	UpdateTime();
	Evaluate();
	onChangedEvent.Invoke();

	if (mTime > mLastFrameTime)
		CheckTimeCallbacks(mLastFrameTime, mTime);
	else
		CheckTimeCallbacks(mTime, mLastFrameTime);

	if (!mPlaying)
		CheckEndCallback();
}

void IAnimation::UpdateTime()
{
	mLastFrameTime = mTime;

	if (mLoop == LoopType::None)
	{
		mTime = Clamp(mAbsoluteTime, 0.0f, mDuration);
	}
	else if (mLoop == LoopType::Repeat)
	{
		float x;
		if (mAbsoluteTime > 0)
		{
			mTime = modff(mAbsoluteTime/mDuration, &x)*mDuration;
		}
		else
		{
			mTime = (1.0f - modff(-mAbsoluteTime/mDuration, &x))*mDuration;
		}
	}
	else //if (mLoop == LT_TOGGLE)
	{
		float x;
		if (mAbsoluteTime > 0)
		{
			mTime = modff(mAbsoluteTime/mDuration, &x)*mDuration;
			if ((int)x%2 == 1)
				mTime = mDuration - mTime;
		}
		else
		{
			mTime = (1.0f - modff(-mAbsoluteTime/mDuration, &x))*mDuration;
			if ((int)x%2 == 0)
				mTime = mDuration - mTime;
		}
	}
}

void IAnimation::Stop()
{
	mPlaying = false;
}

float IAnimation::GetTime() const
{
	return mAbsoluteTime;
}

float IAnimation::GetLocalTime() const
{
	return mTime;
}

void IAnimation::SetTime(float time)
{
	mAbsoluteTime = time;
	UpdateTime();
	mLastFrameTime = mTime;
	Evaluate();
	onChangedEvent.Invoke();
}

void IAnimation::SetLoop(LoopType type /*= LT_REPEAT*/)
{
	mLoop = type;
}

void IAnimation::SetSpeed(float speed /*= 1*/)
{
	mAnimationSpeed = speed;
	mDirectionSign = Sign(mEndPlaying - mBeginPlaying);
}

float IAnimation::GetSpeed() const
{
	return mAnimationSpeed;
}

float IAnimation::GetDuration() const
{
	return mDuration;
}

float IAnimation::GetBeginTime() const
{
	return mBeginPlaying;
}

float IAnimation::GetEndTime() const
{
	return mEndPlaying;
}

bool IAnimation::IsPlaying() const
{
	return mPlaying;
}

bool IAnimation::IsPlayed() const
{
	return mPlayed;
}

void IAnimation::SetPlaying(bool playing)
{
	if (playing == IsPlaying())
		return;

	if (playing)
		Play(mBeginPlaying, mEndPlaying);
	else
		Stop();
}

void IAnimation::SetBeginPlayingCallback(ICallback* cb)
{
	mBeginPlayingCallback = cb;
}

void IAnimation::SetEndPlayingCallback(ICallback* cb)
{
	mEndPlayingCallback = cb;
}

void IAnimation::SetTimeCallback(float time, ICallback* cb)
{
	mTimedCallbacks.push_back(TimeCallback(cb, time));
}

void IAnimation::CheckBeginCallback()
{
	if (mBeginPlayingCallback)
		mBeginPlayingCallback->Invoke();
}

void IAnimation::CheckEndCallback()
{
	if (mEndPlayingCallback)
		mEndPlayingCallback->Invoke();
}

void IAnimation::CheckTimeCallbacks(float begin, float end)
{
	for (TimeCallbacksVec::iterator it = mTimedCallbacks.begin(); it != mTimedCallbacks.end(); ++it)
	{
		if (!(it->mTime < begin || it->mTime > end))
			it->mCallback->Invoke();
	}
}

CLOSE_O2_NAMESPACE