#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	class DefaultTypeAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("DEFAULT_TYPE");
		ATTRIBUTE_SHORT_DEFINITION("DEFAULT_TYPE_ATTRIBUTE");

	public:
		const Type* defaultType = nullptr;

	public:
		DefaultTypeAttribute() { }
		DefaultTypeAttribute(const Type* defaultType) :defaultType(defaultType) { }
	};

#define DEFAULT_TYPE_ATTRIBUTE(type) \
	template AddAttribute<o2::DefaultTypeAttribute>(&TypeOf(type))
}
