#include "Time.h"

#include <Windows.h>

#include "Utils/Reflection/Types.h"

namespace o2
{
	//TimeStuff& Time = TimeStuff::Instance();

	DECLARE_SINGLETON(TimeStuff);
	IOBJECT_CPP(TimeStamp);

	TimeStuff::TimeStuff():
		mApplicationTime(0), mLocalTime(0), mCurrentFrame(0), mDeltaTime(0), mFPS(0), mFPSSum(0),
		mFramesSum(0), mLastFPSCheckingTime(0)
	{
		Time = this;
	}

	TimeStuff::~TimeStuff()
	{
	}

	void TimeStuff::Update(float dt)
	{
		if (dt < FLT_EPSILON)
			return;

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

	float TimeStuff::GetApplicationTime() const
	{
		return mInstance->mApplicationTime;
	}

	float TimeStuff::GetLocalTime() const
	{
		return mInstance->mLocalTime;
	}

	void TimeStuff::ResetLocalTime() const
	{
		mInstance->mLocalTime = 0;
	}

	void TimeStuff::SetLocalTime(float time) const
	{
		mInstance->mLocalTime = time;
	}

	int TimeStuff::GetCurrentFrame() const
	{
		return (int)mInstance->mCurrentFrame;
	}

	float TimeStuff::GetDeltaTime() const
	{
		return mInstance->mDeltaTime;
	}

	float TimeStuff::GetFPS() const
	{
		return mInstance->mFPS;
	}

	TimeStamp TimeStuff::CurrentTime() const
	{
		SYSTEMTIME tm;
		GetSystemTime(&tm);
		
		return TimeStamp(tm.wSecond, tm.wMinute, tm.wHour, tm.wDay, tm.wMonth, tm.wYear);
	}

	bool TimeStamp::operator!=(const TimeStamp& wt) const
	{
		return !(*this == wt);
	}

	bool TimeStamp::operator==(const TimeStamp& wt) const
	{
		return mSecond == wt.mSecond && mMinute == wt.mMinute && mHour == wt.mHour && mDay == wt.mDay && mMonth == wt.mMonth &&
			mYear == wt.mYear;
	}

	TimeStamp::TimeStamp(int seconds /*= 0*/, int minutes /*= 0*/, int hours /*= 0*/, int days /*= 0*/, int months /*= 0*/, int years /*= 0*/):
		mYear(years), mMonth(months), mDay(days), mHour(hours), mMinute(minutes), mSecond(seconds)
	{
	}

}