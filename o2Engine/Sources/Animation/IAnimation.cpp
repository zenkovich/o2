#include "stdafx.h"
#include "IAnimation.h"

#include "Utils/Math/Math.h"

namespace o2
{
	IAnimation::IAnimation():
		mTime(0), mDuration(0), mBeginTime(0), mEndTime(0), mDirection(1.0f), mSpeed(1.0f), mLoop(Loop::None),
		mPlaying(false), mInDurationTime(0)
	{}

	IAnimation::IAnimation(const IAnimation& other) :
		mTime(other.mTime), mDuration(other.mDuration), mBeginTime(other.mBeginTime), mEndTime(other.mEndTime),
		mDirection(other.mDirection), mSpeed(other.mSpeed), mLoop(other.mLoop),
		mPlaying(other.mPlaying), mInDurationTime(other.mInDurationTime),
		playing(this), reversed(this), speed(this), time(this), relTime(this),
		beginBound(this), endBound(this), loop(this), duration(this)
	{}

	IAnimation::~IAnimation()
	{}

	IAnimation& IAnimation::operator=(const IAnimation& other)
	{
		mTime = other.mTime;
		mDuration = other.mDuration;
		mBeginTime = other.mBeginTime;
		mEndTime = other.mEndTime;
		mDirection = other.mDirection;
		mSpeed = other.mSpeed;
		mLoop = other.mLoop;
		mPlaying = other.mPlaying;
		mInDurationTime = other.mInDurationTime;

		return *this;
	}

	void IAnimation::Update(float dt)
	{
		if (!mPlaying)
			return;

		mTime += mDirection*dt*mSpeed;

		float lastInDurationTime = mInDurationTime;

		UpdateTime();

		float eventCheckBeg = Math::Min(lastInDurationTime, mInDurationTime);
		float eventCheckEnd = Math::Max(lastInDurationTime, mInDurationTime);
		for (auto& kv : mTimeEvents)
		{
			if (kv.first > eventCheckBeg && kv.first <= eventCheckEnd)
				kv.second.Invoke();
		}
	}

	void IAnimation::UpdateTime()
	{
		if (mLoop == Loop::None)
		{
			if (mTime > mEndTime)
			{
				mTime = mEndTime;
				mInDurationTime = mTime;
				mPlaying = false;
				Evaluate();

				onStopEvent();
				onPlayedEvent();
			}
			else if (mTime < mBeginTime)
			{
				mTime = mBeginTime;
				mInDurationTime = mTime;
				mPlaying = false;
				Evaluate();

				onStopEvent();
				onPlayedEvent();
			}
			else
			{
				mInDurationTime = mTime;
				Evaluate();
			}
		}
		else if (mLoop == Loop::Repeat)
		{
			float x;
			if (mTime > 0)
				mInDurationTime = modff(mTime/mDuration, &x)*mDuration;
			else
				mInDurationTime = (1.0f - modff(-mTime/mDuration, &x))*mDuration;

			Evaluate();
		}
		else //if (mLoop == Loop::PingPong)
		{
			float x;
			if (mTime > 0)
			{
				mInDurationTime = modff(mTime/mDuration, &x)*mDuration;
				if ((int)x%2 == 1)
					mInDurationTime = mDuration - mInDurationTime;
			}
			else
			{
				mInDurationTime = (1.0f - modff(-mTime/mDuration, &x))*mDuration;
				if ((int)x%2 == 0)
					mInDurationTime = mDuration - mInDurationTime;
			}

			Evaluate();
		}

		onUpdate(mTime);
	}

	void IAnimation::Play()
	{
		if (mPlaying)
			return;

		mPlaying = true;
		onPlayEvent();
		Evaluate();
	}

	void IAnimation::PlayInBounds(float beginTime, float endTime)
	{
		mBeginTime = beginTime;
		mEndTime = endTime;
		Play();
	}

	void IAnimation::PlayBackInBounds(float beginTime, float endTime)
	{
		mBeginTime = beginTime;
		mEndTime = endTime;
		PlayBack();
	}

	void IAnimation::TogglePlay()
	{
		SetReverse(!IsReversed());
		Play();
	}

	void IAnimation::RewindAndPlay()
	{
		GoToBegin();
		Play();
	}

	void IAnimation::Stop()
	{
		if (!mPlaying)
			return;

		Evaluate();
		mPlaying = false;
	}

	void IAnimation::SetBeginBound(float time)
	{
		SetBounds(time, mEndTime);
	}

	float IAnimation::GetBeginBound() const
	{
		return mBeginTime;
	}

	void IAnimation::SetEndBound(float time)
	{
		SetBounds(mBeginTime, time);
	}

	float IAnimation::GetEndBound() const
	{
		return mEndTime;
	}

	void IAnimation::SetBounds(float beginTime, float endTime)
	{
		mBeginTime = Math::Min(beginTime, endTime);
		mEndTime = Math::Max(beginTime, endTime);
		SetReverse(beginTime > endTime);
		mTime = Math::Clamp(mTime, mBeginTime, mEndTime);
		Evaluate();
	}

	void IAnimation::ResetBounds()
	{
		mBeginTime = 0.0f;
		mEndTime = mDuration;
	}

	void IAnimation::SetPlaying(bool playing)
	{
		mPlaying = playing;
		Evaluate();
	}

	bool IAnimation::IsPlaying() const
	{
		return mPlaying;
	}

	void IAnimation::SetTime(float time)
	{
		mTime = time;
		UpdateTime();
	}

	float IAnimation::GetTime() const
	{
		return mTime;
	}

	float IAnimation::GetLoopTime() const
	{
		float duration = GetDuration();

		switch (mLoop) 
		{
			case Loop::None:
			return Math::Clamp(mTime, 0.0f, duration);

			case Loop::PingPong:
			return Math::Mod(mTime, duration)*((int)(mTime / duration) % 2 == 0 ? 1.0f : -1.0f);

			case Loop::Repeat:
			return Math::Mod(mTime, duration);
		}

		return mTime;
	}

	float IAnimation::GetDuration() const
	{
		return mEndTime - mBeginTime;
	}

	void IAnimation::SetRelTime(float relTime)
	{
		SetTime(Math::Lerp(mBeginTime, mEndTime, relTime));
	}

	float IAnimation::GetRelTime() const
	{
		return mTime/(mEndTime - mBeginTime);
	}

	void IAnimation::GoToBegin()
	{
		SetTime(mBeginTime);
	}

	void IAnimation::GoToEnd()
	{
		SetTime(mEndTime);
	}

	void IAnimation::PlayForward()
	{
		SetReverse(false);
		Play();
	}

	void IAnimation::PlayBack()
	{
		SetReverse(true);
		Play();
	}

	void IAnimation::SetReverse(bool reverse)
	{
		mDirection = reverse ? -1.0f : 1.0f;
	}

	bool IAnimation::IsReversed() const
	{
		return mDirection < 0.0f;
	}

	void IAnimation::SetSpeed(float speed)
	{
		mSpeed = speed;
	}

	float IAnimation::GetSpeed() const
	{
		return mSpeed;
	}

	void IAnimation::SetLoop(Loop loop /*= Loop::Repeat*/)
	{
		mLoop = loop;
	}

	Loop IAnimation::GetLoop() const
	{
		return mLoop;
	}

	void IAnimation::AddTimeEvent(float time, const Function<void()> eventFunc)
	{
		mTimeEvents.Add(time, eventFunc);
	}

	void IAnimation::RemoveTimeEvent(float time)
	{
		mTimeEvents.Remove(time);
	}

	void IAnimation::RemoveTimeEvent(const Function<void()>& eventFunc)
	{
		mTimeEvents.RemoveAll([=](float t, const Function<void()>& f) { return f == eventFunc; });
	}

	void IAnimation::RemoveAllTimeEvents()
	{
		mTimeEvents.Clear();
	}

	void IAnimation::Evaluate()
	{}
}

DECLARE_CLASS(o2::IAnimation);

ENUM_META_(o2::Loop, Loop)
{
	ENUM_ENTRY(None);
	ENUM_ENTRY(PingPong);
	ENUM_ENTRY(Repeat);
}
END_ENUM_META;
