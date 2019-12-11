#pragma once
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Attributes.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class InvokeOnChangeAttribute : public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("INVOKE_ON_CHANGE");
		ATTRIBUTE_SHORT_DEFINITION("INVOKE_ON_CHANGE_ATTRIBUTE");

	public:
		String methodName;

	public:
		InvokeOnChangeAttribute() { }
		InvokeOnChangeAttribute(const String& methodName):methodName(methodName) { }
	};

#define INVOKE_ON_CHANGE_ATTRIBUTE(methodName) \
    AddAttribute(new InvokeOnChangeAttribute(#methodName))
}
