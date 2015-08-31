#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Type.h"

namespace o2
{
	// ------------------------------
	// Types in application container
	// ------------------------------
	class Types
	{
		static Vector<Type*> mTypes;           // All registered types
		static UInt          mLastGivenTypeId; // Last given type index

	public:
		// ----------------
		// Type registrator
		// ----------------
		template<typename _type>
		struct Registrator
		{
			Registrator(Type& type, const String& name);
		};

		// Returns array of all registered types
		static const Vector<Type*>& GetTypes();

		// Returns a copy of type sample
		static IObject* CreateTypeSample(const String& typeName);
	};

	template<typename _type>
	Types::Registrator<_type>::Registrator(Type& type, const String& name)
	{
		_type* sample = new _type();
		_type::InitializeType(sample);
		Type::Initialize(type, name, mLastGivenTypeId++, sample);
		Types::mTypes.Add(&type);
	}

#define FIRST_TYPES_USAGE Vector<Type*> Types::mTypes
}
