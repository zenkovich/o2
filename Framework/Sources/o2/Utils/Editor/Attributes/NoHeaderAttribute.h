#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

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
