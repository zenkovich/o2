#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	// ------------------------------
	// Reflection in application container
	// ------------------------------
	class Reflection
	{
	public:
		static Ptr<Reflection> instance;

	public:
		// Constructor. Initializes dummy type
		Reflection();

		// Destructor. Destroys types
		~Reflection();

		// Returns array of all registered types
		static const Vector<Ptr<Type>>& GetTypes();

		// Returns a copy of type sample
		static IObject* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static Ptr<Type> GetType(Type::Id id);

		// Initializes type
		template<typename _type>
		static void InitializeType(const String& name);

	protected:
		Vector<Ptr<Type>> mTypes;           // All registered types
		UInt              mLastGivenTypeId; // Last given type index
	};

	template<typename _type>
	void Reflection::InitializeType(const String& name)
	{
		Ptr<_type> sample = mnew _type();

		_type::InitializeType(sample);
		_type::type->mName = name;
		_type::type->mId = instance->mLastGivenTypeId++;
		_type::type->mSample = sample;

		instance->mTypes.Add(_type::type);
	}
}
