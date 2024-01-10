#pragma once

#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
    // ----------
    // Time stamp
    // ----------
    struct TimeStamp: public ISerializable
    {
        int mYear;   // @SERIALIZABLE
        int mMonth;  // @SERIALIZABLE
        int mDay;    // @SERIALIZABLE
        int mHour;   // @SERIALIZABLE
        int mMinute; // @SERIALIZABLE
        int mSecond; // @SERIALIZABLE

    public:
        TimeStamp(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

        bool operator==(const TimeStamp& wt) const;
        bool operator!=(const TimeStamp& wt) const;

        SERIALIZABLE(TimeStamp);
    };
}
// --- META ---

CLASS_BASES_META(o2::TimeStamp)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::TimeStamp)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mYear);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mMonth);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mDay);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mHour);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mMinute);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mSecond);
}
END_META;
CLASS_METHODS_META(o2::TimeStamp)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(int, int, int, int, int, int);
}
END_META;
// --- END META ---
