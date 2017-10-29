#pragma once

#include <functional>
#include "Utils/Containers/Pair.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/StringDef.h"

// Reflection access macros
#define o2Reflection Reflection::Instance()

namespace o2
{
	class Type;
	class EnumType;
	class PropertyType;
	class VectorType;
	class DictionaryType;

	template<typename _return_type>
	class StringPointerAccessorType;

	typedef UInt TypeId;

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
		static const Type* GetType(TypeId id);

		// Returns type by name
		static const Type* GetType(const String& name);

		// Returns enum value from string
		template<typename _type>
		static _type GetEnumValue(const String& name);

		// Returns enum name from value
		template<typename _type>
		static String GetEnumName(_type value);

		// Returns is types was initialized
		static bool IsTypesInitialized();

	public:
		template<typename _type>
		static Type* InitializeType(const char* name);

		template<typename _type>
		static Type* InitializeFundamentalType(const char* name);

		// Initializes enum
		template<typename _type>
		static EnumType* InitializeEnum(const char* name, std::function<Dictionary<int, String>()> func);

		// Initializes pointer type
		static const Type* InitializePointerType(const Type* type);

		// Initializes property type 
		template<typename _value_type>
		static const PropertyType* InitializePropertyType();

		// Initializes vector type
		template<typename _element_type>
		static const VectorType* InitializeVectorType();

		// Initializes dictionary type
		template<typename _key_type, typename _value_type>
		static const DictionaryType* InitializeDictionaryType();

		// Initializes accessor type
		template<typename _return_type>
		static const StringPointerAccessorType<_return_type>* InitializeAccessorType();

	protected:
		static Reflection* mInstance;        // Reflection instance

		bool               mTypesInitialized = false;

		Vector<Type*>      mTypes;           // All registered types
		UInt               mLastGivenTypeId; // Last given type index

	protected:
		// Constructor. Initializes dummy type
		Reflection();

		// Destructor. Destroys types
		~Reflection();

		// Initializes fundamental types
		static void InitializeFundamentalTypes();

		friend class Type;
	};
}

#define BREAK_ON_REFLECTION_STAGE if (!Reflection::IsTypesInitialized()) return

#define REG_TYPE(CLASS) \
	o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)

#define REG_FUNDAMENTAL_TYPE(TYPE) \
	o2::Type* o2::FundamentalTypeContainer<TYPE>::type = o2::Reflection::InitializeFundamentalType<TYPE>(#TYPE)

#define ENUM_META(NAME)                                                                                  \
    o2::EnumType* o2::EnumTypeContainer<NAME>::type = o2::Reflection::InitializeEnum<NAME>(#NAME, []() { \
    typedef NAME EnumName;                                                                               \
    o2::Dictionary<int, o2::String> res;    

#define ENUM_META_(NAME, U)                                                                              \
    o2::EnumType* o2::EnumTypeContainer<NAME>::type = o2::Reflection::InitializeEnum<NAME>(#NAME, []() { \
    typedef NAME EnumName;                                                                               \
    o2::Dictionary<int, o2::String> res;                                        

#define ENUM_ENTRY(NAME) \
    res.Add((int)EnumName::NAME, #NAME)

#define END_ENUM_META \
    return res; });

#include "Utils/Reflection/Type.h"

namespace o2
{
	template<typename _type>
	_type Reflection::GetEnumValue(const String& name)
	{
		EnumType* type = (EnumType*)(&TypeOf(_type));
		auto& entries = type->GetEntries();

		if (entries.ContainsValue(name))
			return (_type)(entries.FindValue(name).Key());

		return (_type)0;
	}

	template<typename _type>
	String Reflection::GetEnumName(_type value)
	{
		String res;

		EnumType* type = (EnumType*)(&TypeOf(_type));
		auto& entries = type->GetEntries();

		entries.TryGetValue((int)value, res);

		return res;
	}

	template<typename _type>
	Type* Reflection::InitializeType(const char* name)
	{
		Type* res = new Type(name, new TypeSampleCreator<_type>(), sizeof(_type));

		res->mInitializeFunc = &_type::InitializeType;
		res->mId = Reflection::Instance().mLastGivenTypeId++;

		Reflection::Instance().mTypes.Add(res);

		//printf("Reflection::InitializeType(%s): instance:%x - %i\n", name, mInstance, Reflection::Instance().mTypes.Count());

		return res;
	}

	template<typename _type>
	Type* Reflection::InitializeFundamentalType(const char* name)
	{
		Type* res = new FundamentalType<_type>(name);

		res->mInitializeFunc = &FundamentalType<_type>::InitializeType;
		res->mId = Reflection::Instance().mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	EnumType* Reflection::InitializeEnum(const char* name, std::function<Dictionary<int, String>()> func)
	{
		EnumType* res = new EnumType(name, new TypeSampleCreator<_type>(), sizeof(_type));

		res->mInitializeFunc = nullptr;
		res->mId = Reflection::Instance().mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);
		res->mEntries.Add(func());

		return res;
	}

	template<typename _value_type>
	const PropertyType* Reflection::InitializePropertyType()
	{
		String typeName = "o2::Property<" + TypeOf(_value_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return (PropertyType*)fnd;

		TPropertyType<_value_type>* newType = new TPropertyType<_value_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _element_type>
	const VectorType* Reflection::InitializeVectorType()
	{
		String typeName = "o2::Vector<" + TypeOf(_element_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return (VectorType*)fnd;

		TVectorType<_element_type>* newType = new TVectorType<_element_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _key_type, typename _value_type>
	const DictionaryType* Reflection::InitializeDictionaryType()
	{
		String typeName = "o2::Dictionary<" + TypeOf(_key_type).GetName() + ", " + TypeOf(_value_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return (DictionaryType*)fnd;

		_key_type* x = nullptr;
		_value_type* y = nullptr;
		DictionaryType* newType = new DictionaryType(x, y);
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _return_type>
	const StringPointerAccessorType<_return_type>* Reflection::InitializeAccessorType()
	{
		const Type* type = &TypeOf(_return_type);
		String typeName = "o2::Accessor<" + type->mName + "*, const o2::String&>";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return (StringPointerAccessorType<_return_type>*)fnd;

		StringPointerAccessorType<_return_type>* newType = new StringPointerAccessorType<_return_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

}
