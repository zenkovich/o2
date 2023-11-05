#pragma once
#include "o2/Utils/Reflection/Attributes.h"

namespace o2
{
    class ScriptableAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("SCRIPTABLE");
        ATTRIBUTE_SHORT_DEFINITION("SCRIPTABLE_ATTRIBUTE");
    };

#define SCRIPTABLE_ATTRIBUTE() \
    template AddAttribute<o2::ScriptableAttribute>()
}

namespace o2
{
    class ScriptableNameAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("SCRIPTABLE_NAME");
        ATTRIBUTE_SHORT_DEFINITION("SCRIPTABLE_NAME_ATTRIBUTE");

    public:
        const char* name = 0;

        ScriptableNameAttribute() {}
        ScriptableNameAttribute(const char* name):name(name) {}
    };

#define SCRIPTABLE_NAME_ATTRIBUTE(NAME) \
    template AddAttribute<o2::ScriptableNameAttribute>(#NAME)
}
