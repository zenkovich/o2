#include "Timer.h"

namespace o2
{
	Timer::Timer()
	{
		Reset();
	}

	Timer::~Timer()
	{
	}

	void Timer::Reset()
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mStartTime);
		mLastElapsedTime = mStartTime.QuadPart;
	}

	float Timer::GetTime()
	{
		LARGE_INTEGER curTime;
		QueryPerformanceCounter(&curTime);

		float res = (float)(curTime.QuadPart - (float)mStartTime.QuadPart)/(float)mFrequency.QuadPart;
		mLastElapsedTime = curTime.QuadPart;

		return res;
	}

	float Timer::GetDeltaTime()
	{
		LARGE_INTEGER curTime;
		QueryPerformanceCounter(&curTime);

		float res = (float)(curTime.QuadPart - (float)mLastElapsedTime)/(float)mFrequency.QuadPart;
		mLastElapsedTime = curTime.QuadPart;

		return res;
	}
}