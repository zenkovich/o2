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
		// Types registrator
		// ----------------
		struct Registrator
		{
			Registrator();
		};

		// Returns array of all registered types
		static const Vector<Type*>& GetTypes();

		// Returns a copy of type sample
		static IObject* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static Type* GetType(Type::Id id);

		// Initializes all engine types
		static void InitializeTypes();

		// Initializes type
		template<typename _type>
		static void InitializeType(const String& name);
	};

	template<typename _type>
	void Types::InitializeType(const String& name)
	{
		_type* sample = new _type();
		_type::InitializeType(sample);
		Type::Initialize(_type::type, name, mLastGivenTypeId++, sample);
		Types::mTypes.Add(&_type::type);
	}

#define INIT_TYPE(TYPE) InitializeType<TYPE>(#TYPE)
}
