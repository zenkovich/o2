#pragma once
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class GroupAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("GROUP");
		ATTRIBUTE_SHORT_DEFINITION("GROUP_ATTRIBUTE");

	public:
		String name;

	public:
		GroupAttribute() {}
		GroupAttribute(const char* name) :name(name) {}
	};

#define GROUP_ATTRIBUTE(NAME) \
    template AddAttribute<o2::GroupAttribute>(NAME)
}
