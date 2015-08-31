#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

namespace o2
{
	class TimeStuff;

	// ----------
	// Time stamp
	// ----------
	struct TimeStamp: public ISerializable
	{
		int mYear;
		int mMonth;
		int mDay;
		int mHour;
		int mMinute;
		int mSecond;

		TimeStamp(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

		bool operator==(const TimeStamp& wt) const;
		bool operator!=(const TimeStamp& wt) const;

		SERIALIZABLE_IMPL(TimeStamp);

		IOBJECT(TimeStamp)
		{
			SRLZ_FIELD(mYear);
			SRLZ_FIELD(mMonth);
			SRLZ_FIELD(mDay);
			SRLZ_FIELD(mHour);
			SRLZ_FIELD(mMinute);
			SRLZ_FIELD(mSecond);
		}
	};

	// Time stuff access macros
#define o2Time TimeStuff::Instance()

	// Time stuff singleton pointer
	static const TimeStuff* Time = nullptr;

	// -------------
	// Timing system
	// -------------
	class TimeStuff: public Singleton<TimeStuff>
	{
		friend class IApplication;
		friend class BaseApplication;

		float  mApplicationTime;     // Application working time, seconds
		float  mLocalTime;           // Local timer working, seconds. Controlling by user
		UInt64 mCurrentFrame;        // Current frame index
		float  mDeltaTime;           // Frame delta time
		float  mFPS;                 // Average frames per second

		float  mFPSSum;              // Summary of fps
		float  mFramesSum;           // Frames summary
		float  mLastFPSCheckingTime; // Last average fps checking time

	protected:
		// Default constructor
		TimeStuff();

		// Update parameters
		void Update(float dt);

	public:
		// Destructor
		~TimeStuff();

		// Returns application working time, in seconds
		float GetApplicationTime() const;

		// Returns local time, in seconds
		float GetLocalTime() const;

		// Resetting local time to zero
		void ResetLocalTime() const;

		// Resetting local time to value
		void SetLocalTime(float time) const;

		// Returns current frame index
		int GetCurrentFrame() const;

		// Returns frames delta time
		float GetDeltaTime() const;

		// Returns average frames per second for 0.3 seconds
		float GetFPS() const;

		// Return current system time
		TimeStamp CurrentTime() const;
	};
}