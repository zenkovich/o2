#pragma once

#include "Utils/Serialization.h"

namespace o2
{
	// ----------
	// Time stamp
	// ----------
	struct TimeStamp: public ISerializable
	{
		int mYear;	 //  @SERIALIZABLE
		int mMonth;	 //  @SERIALIZABLE
		int mDay;	 //  @SERIALIZABLE
		int mHour;	 //  @SERIALIZABLE
		int mMinute; //  @SERIALIZABLE
		int mSecond; //  @SERIALIZABLE

		TimeStamp(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

		bool operator==(const TimeStamp& wt) const;
		bool operator!=(const TimeStamp& wt) const;

		SERIALIZABLE(TimeStamp);
	};
}
