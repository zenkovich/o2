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

		// Initializes reflection
		static void Initialize();

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

		// Initializes type
		template<typename _type>
		static void InitializeType();

		// Initializes fundamental type
		template<typename _type>
		static void InitializeFundamentalType();

		// Initializes enum
		template<typename _type>
		static void InitializeEnum(const Dictionary<int, String>& defs);

		// Initializes fundamental types
		static void InitializeFundamentalTypes();

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
	};

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
	void Reflection::InitializeType()
	{
		_type* sample = nullptr;

		_type::InitializeType(sample);
		_type::type.mId = mInstance->mLastGivenTypeId++;
		_type::type.mTypeAgent = new Type::TypeCreator<_type>();
		_type::type.mSize = sizeof(_type);

		mInstance->mTypes.Add(&_type::type);
	}

	template<typename _type>
	void Reflection::InitializeFundamentalType()
	{
		FundamentalType<_type>::type.SetName(typeid(_type).name());
		FundamentalType<_type>::type.mId = mInstance->mLastGivenTypeId++;
		FundamentalType<_type>::type.mTypeAgent = new Type::TypeCreator<_type>();
		FundamentalType<_type>::type.mSize = sizeof(_type);

		mInstance->mTypes.Add(&FundamentalType<_type>::type);
	}

	template<typename _type>
	void Reflection::InitializeEnum(const Dictionary<int, String>& defs)
	{
		mInstance->mEnums.Add(typeid(_type).name(), defs);
	}
}
