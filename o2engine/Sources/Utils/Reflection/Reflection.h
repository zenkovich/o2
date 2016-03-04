#pragma once

#include "Utils/Containers/Pair.h"
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
		static void* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static Type* GetType(Type::Id id);

		// Returns enum value from string
		template<typename _type>
		static _type GetEnum(const String& name);

		// Returns enum name from value
		template<typename _type>
		static String GetEnumName(_type value);

		// Initializes fundamental types
		static void InitializeFundamentalTypes();

		// Initializes type
		template<typename _type>
		static void InitializeType(const String& name);

		// Initializes fundamental type
		template<typename _type>
		static void InitializeFundamentalType();

		// Initializes enum
		template<typename _type>
		static void InitializeEnum(const Dictionary<int, String>& defs);

	protected:
		typedef Dictionary<const char*, Dictionary<int, String>> EnumsDict;

		Vector<Type*> mTypes;           // All registered types
		UInt          mLastGivenTypeId; // Last given type index
		EnumsDict     mEnums;           // Enums
		
	protected:
	};

	template<typename _type>
	_type Reflection::GetEnum(const String& name)
	{
		auto typeName = typeid(_type).name();
		if (instance->mEnums.ContainsKey(typeName))
		{
			return (_type)(instance->mEnums[typeName].FindValue(name).Key());
		}

		return (_type)0;
	}

	template<typename _type>
	String Reflection::GetEnumName(_type value)
	{
		auto typeName = typeid(_type).name();
		if (instance->mEnums.ContainsKey(typeName))
		{
			return instance->mEnums[typeName][(int)value];
		}

		return "unknown";
	}

	template<typename _type>
	void Reflection::InitializeType(const String& name)
	{
		_type* sample = nullptr;

		_type::InitializeType(sample);
		_type::type.mName = name;
		_type::type.mId = instance->mLastGivenTypeId++;
		_type::type.mTypeAgent = new Type::TypeAgent<_type>();

		instance->mTypes.Add(&_type::type);
	}

	template<typename _type>
	void Reflection::InitializeFundamentalType()
	{
		FundamentalType<_type>::type.mName = typeid(_type).name();
		FundamentalType<_type>::type.mId = instance->mLastGivenTypeId++;
		FundamentalType<_type>::type.mTypeAgent = new Type::TypeAgent<_type>();

		instance->mTypes.Add(&FundamentalType<_type>::type);
	}

	template<typename _type>
	void Reflection::InitializeEnum(const Dictionary<int, String>& defs)
	{
		instance->mEnums.Add(typeid(_type).name(), defs);
	}
}
