#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

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
