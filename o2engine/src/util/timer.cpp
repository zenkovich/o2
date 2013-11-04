#include "timer.h"

OPEN_O2_NAMESPACE

cTimer::cTimer()
{
	reset();
}

cTimer::~cTimer()
{

}

void cTimer::reset()
{	
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartTime);
	mLastElapsedTime = mStartTime.QuadPart;
}

float cTimer::getTime()
{	
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)(curTime.QuadPart - (float)mStartTime.QuadPart)/(float)mFrequency.QuadPart;
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

float cTimer::getElapsedTime()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)(curTime.QuadPart - (float)mLastElapsedTime)/(float)mFrequency.QuadPart;
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

CLOSE_O2_NAMESPACE