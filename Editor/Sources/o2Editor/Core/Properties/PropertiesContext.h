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
		PropertiesContext* parent = nullptr; // Parent context

		Vector<Pair<IObject*, IObject*>>       targets;    // Last set targets
		Map<const FieldInfo*, IPropertyField*> properties; // Property info to property field map

		Spoiler* privatePropertiesSpoiler = nullptr; // Private properties spoiler

		bool builtWithPrivateProperties = false; // Is properties was built with hidden properties

	public:
		// Sets targets objects and updates targets in properties
		void Set(const Vector<Pair<IObject*, IObject*>>& targets);

		// Refreshes properties
		void Refresh();

		// Returns is properties was built with hidden properties
		bool IsBuiltWIthPrivateProperties() const;

		// Returns property info to property field map
		const Map<const FieldInfo*, IPropertyField*>& GetProperties() const;

		// Searches target with suitable type on contexts stack
		template<typename _type>
		_type* FindOnStack() const;

		friend class Properties;
	};

	template<typename _type>
	_type* PropertiesContext::FindOnStack() const
	{
		auto contextIt = parent;
		while (contextIt)
		{
			if (!contextIt->targets.IsEmpty())
			{
				if (auto typed = dynamic_cast<_type*>(contextIt->targets[0].first))
					return typed;
			}

			contextIt = contextIt->parent;
		}

		return nullptr;
	}

}
