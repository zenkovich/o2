#pragma once
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class NoHeaderAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("NO_HEADER");
		ATTRIBUTE_SHORT_DEFINITION("NO_HEADER_ATTRIBUTE");
	};

#define NO_HEADER_ATTRIBUTE() \
    AddAttribute(new NoHeaderAttribute())
}
