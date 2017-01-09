#pragma once

#include "Utils/IObject.h"
#include "Utils/Reflection/Attribute.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class EditorPropertyAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("EDITOR_PROPERTY");
		ATTRIBUTE_SHORT_DEFINITION("EDITOR_PROPERTY_ATTRIBUTE()");
	};

	class IgnoreEditorPropertyAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("EDITOR_IGNORE");
		ATTRIBUTE_SHORT_DEFINITION("EDITOR_IGNORE_ATTRIBUTE()");
	};

#define EDITOR_IGNORE_ATTRIBUTE() \
    AddAttribute(new IgnoreEditorPropertyAttribute())

#define EDITOR_PROPERTY_ATTRIBUTE() \
    AddAttribute(new EditorPropertyAttribute())
}