#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Type.h"

namespace o2
{
	// ------------------------------
	// Reflection in application container
	// ------------------------------
	class Reflection
	{
	public:
		static Reflection* instance;

	public:
		// Constructor. Initializes dummy type
		Reflection();

		// Destructor. Destroys types
		~Reflection();

		// Returns array of all registered types
		static const Vector<Type*>& GetTypes();

		// Returns a copy of type sample
		static IObject* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static Type* GetType(Type::Id id);

		// Initializes type
		template<typename _type>
		static void InitializeType(const String& name);

	protected:
		Vector<Type*> mTypes;           // All registered types
		UInt          mLastGivenTypeId; // Last given type index
	};

	template<typename _type>
	void Reflection::InitializeType(const String& name)
	{
		_type* sample = nullptr;

		_type::InitializeType(sample);
		_type::type.mName = name;
		_type::type.mId = instance->mLastGivenTypeId++;
		_type::type.mTypeCreator = new Type::TypeCreator<_type>();

		instance->mTypes.Add(&_type::type);
	}
}
