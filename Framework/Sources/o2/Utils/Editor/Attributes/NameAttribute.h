#pragma once
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class NameAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("NAME");
		ATTRIBUTE_SHORT_DEFINITION("NAME_ATTRIBUTE");

	public:
		String name;

	public:
		NameAttribute() {}
		NameAttribute(const char* name) :name(name) {}
	};

#define NAME_ATTRIBUTE(NAME) \
    template AddAttribute<o2::NameAttribute>(NAME)
}
