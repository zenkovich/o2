#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/Containers/Pair.h"

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
		void Set(const Vector<Pair<IObject*, IObject*>>& targets);

		// Returns is properties was built with hidden properties
		bool IsBuiltWIthPrivateProperties() const;

		// Returns property info to property field map
		const Map<const FieldInfo*, IPropertyField*>& GetProperties() const;

	protected:
		Vector<Pair<IObject*, IObject*>>       mTargets;    // Last setted targets
		Map<const FieldInfo*, IPropertyField*> mProperties; // Property info to property field map

		Spoiler* mPrivatePropertiesSpoiler = nullptr; // Private properties spoiler

		bool mBuildWithPrivateProperties = false; // Is properties was built with hidden properties

		friend class Properties;
	};
}
