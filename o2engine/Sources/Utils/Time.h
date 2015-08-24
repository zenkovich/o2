#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

namespace o2
{
	struct WideTime: public ISerializable
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

		SERIALIZABLE_IMPL(WideTime);

		FIELDS()
		{
			SERIALIZABLE_FIELD(mYear);
			SERIALIZABLE_FIELD(mMonth);
			SERIALIZABLE_FIELD(mDay);
			SERIALIZABLE_FIELD(mHour);
			SERIALIZABLE_FIELD(mMinute);
			SERIALIZABLE_FIELD(mSecond);
		}
	};

	class Time: public Singleton<Time>
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

		/** Update parameters. */
		void Update(float dt);

	public:
		/** dtor. */
		~Time();

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