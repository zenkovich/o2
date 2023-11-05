#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
    class EditorPropertyAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("EDITOR_PROPERTY");
        ATTRIBUTE_SHORT_DEFINITION("EDITOR_PROPERTY_ATTRIBUTE");
    };

    class IgnoreEditorPropertyAttribute: public IAttribute
    {
        ATTRIBUTE_COMMENT_DEFINITION("EDITOR_IGNORE");
        ATTRIBUTE_SHORT_DEFINITION("EDITOR_IGNORE_ATTRIBUTE");
    };

#define EDITOR_IGNORE_ATTRIBUTE() \
    template AddAttribute<o2::IgnoreEditorPropertyAttribute>()

#define EDITOR_PROPERTY_ATTRIBUTE() \
    template AddAttribute<o2::EditorPropertyAttribute>()
}
