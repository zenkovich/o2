#pragma once

namespace o2
{
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
}