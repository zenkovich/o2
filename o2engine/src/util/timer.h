#ifndef TIMER_H
#define TIMER_H

#include "public.h"

#include <Windows.h>

OPEN_O2_NAMESPACE

/** Timer class, Containing elapsed time from last getElapsedTime() call and elapsed time from reset() call. */
class cTimer
{
	LONGLONG      mLastElapsedTime;
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartTime;

public:
	cTimer();
	~cTimer();
	
	/** Resetting time. */
	void  reset();

	/** Returns time in seconds from last reset() call. */
	float getTime();

	/** return time in seconds from last reset() or getElapsedTime() call. */
	float getElapsedTime();
};

CLOSE_O2_NAMESPACE

#endif //TIMER_H