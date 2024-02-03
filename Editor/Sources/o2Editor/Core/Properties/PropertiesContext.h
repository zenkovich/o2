#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/SmartPtr/Ref.h"
#include "o2/Utils/Types/SmartPtr/WeakRef.h"

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
		Ref<PropertiesContext> parent; // Parent context

		Vector<Pair<Ref<IObject>, Ref<IObject>>>       targets;    // Last set targets
		Map<const FieldInfo*, Ref<IPropertyField>> properties; // Property info to property field map

		WeakRef<Spoiler> privatePropertiesSpoiler; // Private properties spoiler

		bool builtWithPrivateProperties = false; // Is properties was built with hidden properties

	public:
		// Marks context as not actual and required to update values proxies
		void Invalidate();

		// Sets targets objects and updates targets in properties
		void Set(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targets, bool force = false);

		// Refreshes properties
		void Refresh();

		// Returns is properties was built with hidden properties
		bool IsBuiltWIthPrivateProperties() const;

		// Returns property info to property field map
		const Map<const FieldInfo*, Ref<IPropertyField>>& GetProperties() const;

		// Searches target with suitable type on contexts stack
		template<typename _type>
		_type* FindOnStack() const;

		friend class Properties;
	};

	template<typename _type>
	_type* PropertiesContext::FindOnStack() const
	{
		auto contextIt = parent.get();
		while (contextIt)
		{
			if (!contextIt->targets.IsEmpty())
			{
				if (auto typed = DynamicCast<_type>(contextIt->targets[0].first))
					return typed;
			}

			contextIt = contextIt->parent.get();
		}

		return nullptr;
	}
}