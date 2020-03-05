#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class FieldInfo;
	class IObject;
	class Spoiler;
}

namespace Editor
{
	class IPropertyField;

	class PropertiesContext
	{
	public:
		Vector<Pair<IObject*, IObject*>>       targets;    // Last set targets
		Map<const FieldInfo*, IPropertyField*> properties; // Property info to property field map

		Spoiler* privatePropertiesSpoiler = nullptr; // Private properties spoiler

		bool builtWithPrivateProperties = false; // Is properties was built with hidden properties

	public:
		void Set(const Vector<Pair<IObject*, IObject*>>& targets);

		// Returns is properties was built with hidden properties
		bool IsBuiltWIthPrivateProperties() const;

		// Returns property info to property field map
		const Map<const FieldInfo*, IPropertyField*>& GetProperties() const;

		friend class Properties;
	};
}
