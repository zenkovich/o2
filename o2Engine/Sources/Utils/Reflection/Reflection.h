#pragma once

#include "Utils/Containers/Pair.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Reflection/Type.h"

// Reflection access macros
#define o2Reflection Reflection::Instance()

namespace o2
{
	// ------------------------------
	// Reflection in application container
	// ------------------------------
	class Reflection
	{
	public:
		// Returns reflection instance
		static Reflection& Instance();

		// Returns array of all registered types
		static const Vector<Type*>& GetTypes();

		// Returns a copy of type sample
		static void* CreateTypeSample(const String& typeName);

		// Returns type by type id
		static const Type* GetType(Type::Id id);

		// Returns type by name
		static const Type* GetType(const String& name);

		// Returns enum value from string
		template<typename _type>
		static _type GetEnumValue(const String& name);

		// Returns enum name from value
		template<typename _type>
		static String GetEnumName(_type value);

	public:
		template<typename _type>
		static Type* InitializeType(const char* name);

		template<typename _type>
		static Type* InitializeFundamentalType(const char* name);

		// Initializes enum
		template<typename _type>
		static void InitializeEnum(const Dictionary<int, String>& defs);

		// Initializes pointer type
		static const Type* InitializePointerType(const Type* type);

	protected:
		typedef Dictionary<const char*, Dictionary<int, String>> EnumsDict;

		static Reflection* mInstance;        // Reflection instance

		Vector<Type*>      mTypes;           // All registered types
		UInt               mLastGivenTypeId; // Last given type index
		EnumsDict          mEnums;           // Enums
		
	protected:
		// Constructor. Initializes dummy type
		Reflection();

		// Destructor. Destroys types
		~Reflection();

		// Initializes fundamental types
		static void InitializeFundamentalTypes();

		friend class Type;
	};

#define REG_TYPE(CLASS) \
	o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)

#define REG_FUNDAMENTAL_TYPE(TYPE) \
	o2::Type* o2::FundamentalType<TYPE>::type = o2::Reflection::InitializeFundamentalType<TYPE>(#TYPE)

	template<typename _type>
	_type Reflection::GetEnumValue(const String& name)
	{
		auto typeName = typeid(_type).name();
		if (mInstance->mEnums.ContainsKey(typeName))
		{
			return (_type)(mInstance->mEnums[typeName].FindValue(name).Key());
		}

		return (_type)0;
	}

	template<typename _type>
	String Reflection::GetEnumName(_type value)
	{
		auto typeName = typeid(_type).name();
		if (mInstance->mEnums.ContainsKey(typeName))
		{
			return mInstance->mEnums[typeName][(int)value];
		}

		return "unknown";
	}

	template<typename _type>
	Type* Reflection::InitializeType(const char* name)
	{
		Type* res = new Type(name, new Type::SampleCreator<_type>(), sizeof(_type));

		_type::InitializeType(res);
		res->mId = mInstance->mLastGivenTypeId++;

		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	Type* Reflection::InitializeFundamentalType(const char* name)
	{
		Type* res = new Type(name, new Type::SampleCreator<_type>(), sizeof(_type));

		res->mId = mInstance->mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	void Reflection::InitializeEnum(const Dictionary<int, String>& defs)
	{
		mInstance->mEnums.Add(typeid(_type).name(), defs);
	}
}
