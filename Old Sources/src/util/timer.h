#pragma once

#include "public.h"

#include <Windows.h>

OPEN_O2_NAMESPACE

/* Timer class, Containing elapsed time from last getElapsedTime() call and elapsed time from reset() call. */
class Timer
{
	LONGLONG      mLastElapsedTime;
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartTime;

public:
	/* ctor. */
	Timer();

	/* dtor. */
	~Timer();

	/* Resetting time. */
	void  Reset();

	/* Returns time in seconds from last reset() call. */
	float GetTime();

	/* return time in seconds from last reset() or getElapsedTime() call. */
	float GetElapsedTime();
};

CLOSE_O2_NAMESPACE