#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class ExpandedByDefaultAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("EXPANDED_BY_DEFAULT");
		ATTRIBUTE_SHORT_DEFINITION("EXPANDED_BY_DEFAULT_ATTRIBUTE");
	};

#define EXPANDED_BY_DEFAULT_ATTRIBUTE() \
    AddAttribute(new ExpandedByDefaultAttribute())
}
