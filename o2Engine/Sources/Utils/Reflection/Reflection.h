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

		// Initializes all types
		static void InitializeTypes();

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
		static int InitializeEnum(std::function<Dictionary<int, const char*>()> func);

		// Initializes pointer type
		static const Type* InitializePointerType(const Type* type);

	protected:
		typedef Dictionary<const char*, Dictionary<int, const char*>> EnumsDict;

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

#define CONCATENATE_DETAIL(x, y, z) x##y##z
#define CONCATENATE(x, y, z) CONCATENATE_DETAIL(x, y, z)
#define MAKE_UNIQUE(x) CONCATENATE(x, __LINE__, __COUNTER__)

#define ENUM_META(NAME)                                                      \
    int MAKE_UNIQUE(_enum_def) = o2::Reflection::InitializeEnum<NAME>([]() { \
    typedef NAME EnumName;                                                   \
    o2::Dictionary<int, const char*> res;                                        

#define ENUM_ENTRY(NAME) \
    res.Add((int)EnumName::NAME, #NAME)

#define END_ENUM_META \
    return res; });

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

		res->mInitializeFunc = &_type::InitializeType;
		res->mId = Reflection::Instance().mLastGivenTypeId++;

		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	Type* Reflection::InitializeFundamentalType(const char* name)
	{
		Type* res = new Type(name, new Type::SampleCreator<_type>(), sizeof(_type));

		res->mId = Reflection::Instance().mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	int Reflection::InitializeEnum(std::function<Dictionary<int, const char*>()> func)
	{
		Reflection::Instance().mEnums.Add(typeid(_type).name(), func());
		return 0;
	}
}
