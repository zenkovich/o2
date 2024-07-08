#pragma once
#include "o2/Utils/Reflection/Attributes.h"

namespace o2
{
    class RangeAttribute : public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("RANGE");
        ATTRIBUTE_SHORT_DEFINITION("RANGE_ATTRIBUTE");

	public:
		float minRange = 0;
		float maxRange = 1;

        RangeAttribute() {}
        RangeAttribute(float minRange, float maxRange):minRange(minRange), maxRange(maxRange) {}
    };

#define RANGE_ATTRIBUTE(MIN_VALUE, MAX_VALUE) \
    template AddAttribute<o2::RangeAttribute>(MIN_VALUE, MAX_VALUE)
}
