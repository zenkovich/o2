#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	// ------------------------------
	// Types in application container
	// ------------------------------
	class Types
	{
	public:
		static Ptr<Types> instance;

	public:
		// Returns array of all registered types
		static const Vector<Ptr<Type>>& GetTypes();

		// Returns a copy of type sample
		static IObject* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static Ptr<Type> GetType(Type::Id id);

		// Initializes all engine types
		static void InitializeTypes();

		// Initializes type
		template<typename _type>
		static void InitializeType(const String& name);

	protected:
		Vector<Ptr<Type>> mTypes;           // All registered types
		UInt              mLastGivenTypeId; // Last given type index
	};

	template<typename _type>
	void Types::InitializeType(const String& name)
	{
		_type::type = mnew Type();

		Ptr<_type> sample = mnew _type();

		_type::InitializeType(sample);
		Type::Initialize(*_type::type, name, instance->mLastGivenTypeId++, sample);

		instance->mTypes.Add(_type::type);
	}

#define INIT_TYPE(TYPE) Types::InitializeType<TYPE>(#TYPE)
}
