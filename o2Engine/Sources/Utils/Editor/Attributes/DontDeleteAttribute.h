#pragma once
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class DontDeleteAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("DONT_DELETE");
		ATTRIBUTE_SHORT_DEFINITION("DONT_DELETE_ATTRIBUTE");
	};

#define DONT_DELETE_ATTRIBUTE() \
    AddAttribute(new DontDeleteAttribute())
}
