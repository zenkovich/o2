#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
    class ExpandedByDefaultAttribute : public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("EXPANDED_BY_DEFAULT");
        ATTRIBUTE_SHORT_DEFINITION("EXPANDED_BY_DEFAULT_ATTRIBUTE");
    };

#define EXPANDED_BY_DEFAULT_ATTRIBUTE() \
    template AddAttribute<o2::ExpandedByDefaultAttribute>()
}
