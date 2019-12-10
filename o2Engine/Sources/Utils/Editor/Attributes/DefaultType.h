#pragma once
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

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
    AddAttribute(new DefaultTypeAttribute(&TypeOf(type)))
}
