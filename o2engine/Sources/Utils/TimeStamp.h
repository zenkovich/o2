#pragma once

#include "Utils/Serialization.h"

namespace o2
{
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
}
