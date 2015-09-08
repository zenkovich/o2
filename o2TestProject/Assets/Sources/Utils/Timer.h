#pragma once

#include <Windows.h>

namespace o2
{
	// ------------------------------------
	// Timer class, using for counting time
	// ------------------------------------
	class Timer
	{
		LONGLONG      mLastElapsedTime;
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mStartTime;

	public:
		// Default constructor
		Timer();

		// Destructor
		~Timer();

		// Resets time
		void Reset();

		// Returns time in seconds from last Reset() call
		float GetTime();

		// Returns time in seconds from last Reset() or GetElapsedTime() call
		float GetDeltaTime();
	};
}