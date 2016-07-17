#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class FieldInfo;
	class IObject;
}

namespace Editor
{
	class IPropertyField;

	struct FieldPropertiesInfo
	{
		Dictionary<FieldInfo*, IPropertyField*> properties;

		void Set(const Vector<IObject*>& targets);
	};
}