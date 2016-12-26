#pragma once

#include "Utils/IObject.h"
#include "Utils/Reflection/Attribute.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class EditorPropertyAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("EDITOR_PROPERTY");
		ATTRIBUTE_SHORT_DEFINITION("EDITOR_PROPERTY_ATTRIBUTE()");
	};

#define EDITOR_PROPERTY_ATTRIBUTE() \
    AddAttribute(new EditorPropertyAttribute())
}