#pragma once

#include "Utils/Reflection/Attribute.h"

namespace o2
{
	template<typename _type>
	class AnimAttribute: public IAttribute
	{
	public:

		IAttribute* Clone() const { return new AnimAttribute(*this); }
	};

#define ANIMATABLE(TYPE) .AddAttribute<AnimAttribute<TYPE>>()
}