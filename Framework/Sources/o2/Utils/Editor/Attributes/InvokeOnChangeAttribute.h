#pragma once
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/Reflection.h"

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
	template AddAttribute<o2::InvokeOnChangeAttribute>(#methodName)
}
