#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
    class PrototypeDeltaSearchAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("DELTA_SEARCH");
        ATTRIBUTE_SHORT_DEFINITION("DELTA_SEARCH_ATTRIBUTE");
    };

    class IgnorePrototypeDeltaSearchAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("IGNORE_DELTA_SEARCH");
        ATTRIBUTE_SHORT_DEFINITION("IGNORE_DELTA_SEARCH_ATTRIBUTE");
    };

#define DELTA_SEARCH_ATTRIBUTE() \
    template AddAttribute<o2::PrototypeDeltaSearchAttribute>()

#define IGNORE_DELTA_SEARCH_ATTRIBUTE() \
    template AddAttribute<o2::IgnorePrototypeDeltaSearchAttribute>()
}
