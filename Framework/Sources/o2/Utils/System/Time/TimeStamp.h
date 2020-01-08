#pragma once

#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// ----------
	// Time stamp
	// ----------
	struct TimeStamp: public ISerializable
	{
		int mYear;	 // @SERIALIZABLE
		int mMonth;	 // @SERIALIZABLE
		int mDay;	 // @SERIALIZABLE
		int mHour;	 // @SERIALIZABLE
		int mMinute; // @SERIALIZABLE
		int mSecond; // @SERIALIZABLE

		TimeStamp(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

		bool operator==(const TimeStamp& wt) const;
		bool operator!=(const TimeStamp& wt) const;

		SERIALIZABLE(TimeStamp);
	};
}

CLASS_BASES_META(o2::TimeStamp)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::TimeStamp)
{
	PUBLIC_FIELD(mYear).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMonth).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDay).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mHour).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMinute).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSecond).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::TimeStamp)
{
}
END_META;
