#pragma once

#include "Utils/Types/Containers/Map.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/Containers/Pair.h"

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
		void Set(const Vector<IObject*>& targets);
		void Set(const Vector<Pair<IObject*, IObject*>>& targets);

	protected:
		Map<const FieldInfo*, IPropertyField*> mProperties; // Property info to property field map

		Spoiler* mPrivatePropertiesSpoiler = nullptr; // Private properties spoiler

		bool mBuildWithPrivateProperties = false; // Is properties was built with hidden properties
	};
}
