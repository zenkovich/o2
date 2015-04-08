#include "time_utils.h"

OPEN_O2_NAMESPACE

TimeUtil::TimeUtil():
mApplicationTime(0), mLocalTime(0), mCurrentFrame(0), mDeltaTime(0), mFPS(0), mFPSSum(0),
mFramesSum(0), mLastFPSCheckingTime(0)
{
}

TimeUtil::~TimeUtil()
{
}

void TimeUtil::Update(float dt)
{
	mDeltaTime = dt;
	mApplicationTime += dt;
	mLocalTime += dt;
	mCurrentFrame++;

	mFramesSum += 1.0f;
	mFPSSum += 1.0f/dt;
	if (mApplicationTime - mLastFPSCheckingTime > 0.3f)
	{
		mLastFPSCheckingTime = mApplicationTime;
		mFPS = mFPSSum/mFramesSum;
		mFPSSum = 0;
		mFramesSum = 0;
	}
}

float TimeUtil::GetApplicationTime() const
{
	return mApplicationTime;
}

float TimeUtil::GetLocalTime() const
{
	return mLocalTime;
}

void TimeUtil::ResetLocalTime()
{
	mLocalTime = 0;
}

void TimeUtil::SetLocalTime(float time)
{
	mLocalTime = time;
}

int TimeUtil::GetCurrentFrame() const
{
	return (int)mCurrentFrame;
}

float TimeUtil::GetDeltaTime() const
{
	return mDeltaTime;
}

float TimeUtil::GetFPS() const
{
	return mFPS;
}
CLOSE_O2_NAMESPACE