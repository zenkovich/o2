#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

struct WideTime
{
	int mYear;
	int mMonth;
	int mDay;
	int mHour;
	int mMinute;
	int mSecond;

	WideTime(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0)
		:mYear(years), mMonth(months), mDay(days), mHour(hours), mMinute(minutes), mSecond(seconds)
	{
	}

	bool operator==(const WideTime& wt) const
	{
		return mSecond == wt.mSecond && mMinute == wt.mMinute && mHour == wt.mHour && mDay == wt.mDay && mMonth == wt.mMonth &&
			mYear == wt.mYear;
	}

	bool operator!=(const WideTime& wt) const
	{
		return !(*this == wt);
	}
};

class TimeUtil
{
	friend class ApplicationBaseInterface;
	friend class Application;

	float   mApplicationTime;     /* Application working time, seconds. */
	float   mLocalTime;           /* Local timer working, seconds. Controlling by user. */
	uint64  mCurrentFrame;        /* Current frame index. */
	float   mDeltaTime;           /* Frame delta time. */
	float   mFPS;                 /* Average frames per second. */

	float   mFPSSum;              /* Summary of fps. */
	float   mFramesSum;           /* Frames summary. */
	float   mLastFPSCheckingTime; /* Last average fps checking time. */

public:
	/* ctor. */
	TimeUtil();

	/* dtor. */
	~TimeUtil();

	/* Update parameters. */
	void Update(float dt);

	/* Returns application working time, in seconds. */
	float GetApplicationTime() const;

	/* Returns local time, in seconds. */
	float GetLocalTime() const;

	/* Resetting local time to zero. */
	void ResetLocalTime();

	/* Resetting local time to value. */
	void SetLocalTime(float time);

	/* Returns current frame index. */
	int GetCurrentFrame() const;

	/* Returns frames delta time. */
	float GetDeltaTime() const;

	/* Returns average frames per second for 0.3 seconds. */
	float GetFPS() const;
};

CLOSE_O2_NAMESPACE