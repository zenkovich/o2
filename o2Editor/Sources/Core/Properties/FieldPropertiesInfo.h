#pragma once

#include "Utils/Types/Containers/Dictionary.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/Containers/Pair.h"

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
		Dictionary<const FieldInfo*, IPropertyField*> properties;

		void Set(const Vector<IObject*>& targets);
		void Set(const Vector<Pair<IObject*, IObject*>>& targets);
	};
}
