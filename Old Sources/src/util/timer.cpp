#include "timer.h"

OPEN_O2_NAMESPACE

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

float Timer::GetElapsedTime()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)(curTime.QuadPart - (float)mLastElapsedTime)/(float)mFrequency.QuadPart;
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

CLOSE_O2_NAMESPACE