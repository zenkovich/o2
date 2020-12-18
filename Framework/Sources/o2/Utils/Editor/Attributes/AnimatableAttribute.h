#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	class AnimatableAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("ANIMATABLE");
		ATTRIBUTE_SHORT_DEFINITION("ANIMATABLE_ATTRIBUTE");
	};

#define ANIMATABLE_ATTRIBUTE() \
    AddAttribute<AnimatableAttribute>()
}
