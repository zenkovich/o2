#pragma once

#include "Utils/Singleton.h"
#include "Utils/CommonTypes.h"
#include "Utils/Memory/IObject.h"

namespace o2
{
	struct WideTime: public IObject
	{
		int mYear;
		int mMonth;
		int mDay;
		int mHour;
		int mMinute;
		int mSecond;

		WideTime(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

		bool operator==(const WideTime& wt) const;
		bool operator!=(const WideTime& wt) const;
	};

	class Time: public Singleton<Time>, public IObject
	{
		friend class IApplication;
		friend class Application;

		float  mApplicationTime;     /** Application working time, seconds. */
		float  mLocalTime;           /** Local timer working, seconds. Controlling by user. */
		UInt64 mCurrentFrame;        /** Current frame index. */
		float  mDeltaTime;           /** Frame delta time. */
		float  mFPS;                 /** Average frames per second. */

		float  mFPSSum;              /** Summary of fps. */
		float  mFramesSum;           /** Frames summary. */
		float  mLastFPSCheckingTime; /** Last average fps checking time. */

	protected:
		/** ctor. */
		Time();

		/** dtor. */
		~Time();

		/** Update parameters. */
		void Update(float dt);

	public:
		/** Returns application working time, in seconds. */
		static float GetApplicationTime();

		/** Returns local time, in seconds. */
		static float GetLocalTime();

		/** Resetting local time to zero. */
		static void ResetLocalTime();

		/** Resetting local time to value. */
		static void SetLocalTime(float time);

		/** Returns current frame index. */
		static int GetCurrentFrame();

		/** Returns frames delta time. */
		static float GetDeltaTime();

		/** Returns average frames per second for 0.3 seconds. */
		static float GetFPS();

		/** Return current system time. */
		static WideTime CurrentTime();
	};
}