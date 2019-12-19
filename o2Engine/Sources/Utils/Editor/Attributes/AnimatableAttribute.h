#pragma once
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class AnimatableAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("ANIMATABLE");
		ATTRIBUTE_SHORT_DEFINITION("ANIMATABLE_ATTRIBUTE");
	};

#define ANIMATABLE_ATTRIBUTE() \
    AddAttribute(new AnimatableAttribute())
}
