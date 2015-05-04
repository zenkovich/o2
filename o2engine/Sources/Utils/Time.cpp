#include "Time.h"

#include <Windows.h>

namespace o2
{
	DECLARE_SINGLETON(Time);

	Time::Time():
		mApplicationTime(0), mLocalTime(0), mCurrentFrame(0), mDeltaTime(0), mFPS(0), mFPSSum(0),
		mFramesSum(0), mLastFPSCheckingTime(0)
	{
	}

	Time::~Time()
	{
	}

	void Time::Update(float dt)
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

	float Time::GetApplicationTime()
	{
		return mInstance->mApplicationTime;
	}

	float Time::GetLocalTime()
	{
		return mInstance->mLocalTime;
	}

	void Time::ResetLocalTime()
	{
		mInstance->mLocalTime = 0;
	}

	void Time::SetLocalTime(float time)
	{
		mInstance->mLocalTime = time;
	}

	int Time::GetCurrentFrame()
	{
		return (int)mInstance->mCurrentFrame;
	}

	float Time::GetDeltaTime()
	{
		return mInstance->mDeltaTime;
	}

	float Time::GetFPS()
	{
		return mInstance->mFPS;
	}

	WideTime Time::CurrentTime()
	{
		SYSTEMTIME tm;
		GetSystemTime(&tm);
		
		return WideTime(tm.wSecond, tm.wMinute, tm.wHour, tm.wDay, tm.wMonth, tm.wYear);
	}

	bool WideTime::operator!=(const WideTime& wt) const
	{
		return !(*this == wt);
	}

	bool WideTime::operator==(const WideTime& wt) const
	{
		return mSecond == wt.mSecond && mMinute == wt.mMinute && mHour == wt.mHour && mDay == wt.mDay && mMonth == wt.mMonth &&
			mYear == wt.mYear;
	}

	WideTime::WideTime(int seconds /*= 0*/, int minutes /*= 0*/, int hours /*= 0*/, int days /*= 0*/, int months /*= 0*/, int years /*= 0*/):
		mYear(years), mMonth(months), mDay(days), mHour(hours), mMinute(minutes), mSecond(seconds)
	{
	}
}