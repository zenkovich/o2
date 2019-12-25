#pragma once

#include <functional>
#include "Utils/Types/Containers/Pair.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/Containers/Map.h"
#include "Utils/Types/StringDef.h"

// Reflection access macros
#define o2Reflection Reflection::Instance()

namespace o2
{
	class Type;
	class EnumType;
	class PropertyType;
	class VectorType;
	class MapType;

	template<typename _return_type, typename _accessor_type>
	class TStringPointerAccessorType;

	class ReflectionInitializationTypeProcessor;
	class FieldInfo;
	class FunctionInfo;

	class IObject;

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
		static EnumType* InitializeEnum(const char* name, std::function<Map<int, String>()> func);

		// Initializes pointer type
		template<typename _type>
		static const Type* InitializePointerType(const Type* type);

		// Initializes property type 
		template<typename _value_type, typename _property_type>
		static const PropertyType* InitializePropertyType();

		// Initializes vector type
		template<typename _element_type>
		static const VectorType* InitializeVectorType();

		// Initializes dictionary type
		template<typename _key_type, typename _value_type>
		static const MapType* InitializeMapType();

		// Initializes accessor type
		template<typename _return_type, typename _accessor_type>
		static const TStringPointerAccessorType<_return_type, _accessor_type>* InitializeAccessorType();

		// Type dynamic casting function template
		template<typename _source_type, typename _target_type>
		static void* CastFunc(void* obj) { return dynamic_cast<_target_type*>((_source_type*)obj); }

		// Fake type casting function
		static void* NoCastFunc(void* obj) { return obj; }

	protected:
		typedef void(*TypeInitializingFunc)(void*, ReflectionInitializationTypeProcessor&);
		typedef Vector<TypeInitializingFunc> TypeInitializingFuncsVec;

		static Reflection*       mInstance;              // Reflection instance

		Vector<Type*>            mTypes;                 // All registered types
		UInt                     mLastGivenTypeId;       // Last given type index

		TypeInitializingFuncsVec mInitializingFunctions; // List of types initializations functions

		bool                     mTypesInitialized = false;

	protected:
		// Constructor. Initializes dummy type
		Reflection();

		// Destructor. Destroys types
		~Reflection();

		// Initializes fundamental types
		static void InitializeFundamentalTypes();

		friend class Type;
	};

	class ReflectionInitializationTypeProcessor
	{
	public:
		template<typename _object_type>
		void Start(_object_type* object, Type* type);

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type);

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type);

		template<typename _object_type>
		void StartMethods(_object_type* object, Type* type);

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name);

		template<typename _object_type, typename _field_type>
		FieldInfo& Field(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*), _field_type& field, ProtectSection protection);

		template<typename _object_type, typename _res_type, typename ... _args>
		FunctionInfo* Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...), ProtectSection protection);

		template<typename _object_type, typename _res_type, typename ... _args>
		FunctionInfo* Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...) const, ProtectSection protection);

		template<typename _object_type, typename _res_type, typename ... _args>
		StaticFunctionInfo* StaticMethod(_object_type* object, Type* type, const char* name, _res_type(*pointer)(_args ...), ProtectSection protection);
	};
}

#define DECLARE_FUNDAMENTAL_TYPE(TYPE) \
    template<>                         \
	o2::Type* o2::FundamentalTypeContainer<TYPE>::type = o2::Reflection::InitializeFundamentalType<TYPE>(#TYPE)

#define ENUM_META(NAME)                                                                                  \
    template<>                                                                                           \
    o2::EnumType* o2::EnumTypeContainer<NAME>::type = o2::Reflection::InitializeEnum<NAME>(#NAME, []() { \
    typedef NAME EnumName;                                                                               \
    o2::Map<int, o2::String> res;    

#define ENUM_META_(NAME, U)                                                                              \
    template<>                                                                                           \
    o2::EnumType* o2::EnumTypeContainer<NAME>::type = o2::Reflection::InitializeEnum<NAME>(#NAME, []() { \
    typedef NAME EnumName;                                                                               \
    o2::Map<int, o2::String> res;                                        

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
			return (_type)(entries.FindValue(name).first);

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
		Type* res = mnew TObjectType<_type>(name, sizeof(_type), &CastFunc<IObject, _type>,
											&CastFunc<_type, IObject>);

		Reflection::Instance().mInitializingFunctions.Add((TypeInitializingFunc)&_type::template ProcessType<ReflectionInitializationTypeProcessor>);
		res->mId = Reflection::Instance().mLastGivenTypeId++;

		Reflection::Instance().mTypes.Add(res);

		//printf("Reflection::InitializeType(%s): instance:%x - %i\n", name, mInstance, Reflection::Instance().mTypes.Count());

		return res;
	}

	template<typename _type>
	Type* Reflection::InitializeFundamentalType(const char* name)
	{
		Type* res = mnew FundamentalType<_type>(name);

		Reflection::Instance().mInitializingFunctions.Add((TypeInitializingFunc)&FundamentalTypeContainer<_type>::template InitializeType<ReflectionInitializationTypeProcessor>);
		res->mId = Reflection::Instance().mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);

		return res;
	}

	template<typename _type>
	const Type* Reflection::InitializePointerType(const Type* type)
	{
		if (type->mPtrType)
			return type->mPtrType;

		TPointerType<_type>* newType = mnew TPointerType<_type>(type);
		newType->mId = mInstance->mLastGivenTypeId++;

		type->mPtrType = newType;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _type>
	EnumType* Reflection::InitializeEnum(const char* name, std::function<Map<int, String>()> func)
	{
		EnumType* res = mnew TEnumType<_type>(name, sizeof(_type));

		res->mId = Reflection::Instance().mLastGivenTypeId++;
		Reflection::Instance().mTypes.Add(res);
		res->mEntries.Add(func());

		return res;
	}

	template<typename _value_type, typename _property_type>
	const PropertyType* Reflection::InitializePropertyType()
	{
		String typeName = (String)(typeid(_property_type).name()) + (String)"<" + TypeOf(_value_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return dynamic_cast<PropertyType*>(fnd);

		TPropertyType<_value_type, _property_type>* newType = mnew TPropertyType<_value_type, _property_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _element_type>
	const VectorType* Reflection::InitializeVectorType()
	{
		String typeName = "o2::Vector<" + TypeOf(_element_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return dynamic_cast<VectorType*>(fnd);

		TVectorType<_element_type>* newType = mnew TVectorType<_element_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _key_type, typename _value_type>
	const MapType* Reflection::InitializeMapType()
	{
		String typeName = "o2::Dictionary<" + TypeOf(_key_type).GetName() + ", " + TypeOf(_value_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return dynamic_cast<MapType*>(fnd);

		auto newType = mnew TMapType<_key_type, _value_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _return_type, typename _accessor_type>
	const TStringPointerAccessorType<_return_type, _accessor_type>* Reflection::InitializeAccessorType()
	{
		const Type* type = &TypeOf(_return_type);
		String typeName = (String)(typeid(_accessor_type).name()) + (String)"<" + TypeOf(_return_type).GetName() + ">";

		if (auto fnd = mInstance->mTypes.FindMatch([&](auto x) { return x->mName == typeName; }))
			return dynamic_cast<TStringPointerAccessorType<_return_type, _accessor_type>*>(fnd);

		TStringPointerAccessorType<_return_type, _accessor_type>* newType = mnew TStringPointerAccessorType<_return_type, _accessor_type>();
		newType->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes.Add(newType);

		return newType;
	}

	template<typename _object_type>
	void ReflectionInitializationTypeProcessor::Start(_object_type* object, Type* type)
	{}

	template<typename _object_type>
	void ReflectionInitializationTypeProcessor::StartBases(_object_type* object, Type* type)
	{}

	template<typename _object_type>
	void ReflectionInitializationTypeProcessor::StartFields(_object_type* object, Type* type)
	{}

	template<typename _object_type>
	void ReflectionInitializationTypeProcessor::StartMethods(_object_type* object, Type* type)
	{}

	template<typename _object_type, typename _base_type>
	void ReflectionInitializationTypeProcessor::BaseType(_object_type* object, Type* type, const char* name)
	{
		TypeInitializer::AddBaseType<_object_type, _base_type>(type);
	}

	template<typename _object_type, typename _field_type>
	FieldInfo& ReflectionInitializationTypeProcessor::Field(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*), _field_type& field, ProtectSection protection)
	{
		return TypeInitializer::RegField(type, name, pointerGetter, field, protection);
	}

	template<typename _object_type, typename _res_type, typename ... _args>
	FunctionInfo* ReflectionInitializationTypeProcessor::Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...), ProtectSection protection)
	{
		return TypeInitializer::RegFunction<_object_type, _res_type, _args ...>(type, name, pointer, protection);
	}

	template<typename _object_type, typename _res_type, typename ... _args>
	FunctionInfo* ReflectionInitializationTypeProcessor::Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...) const, ProtectSection protection)
	{
		return TypeInitializer::RegFunction<_object_type, _res_type, _args ...>(type, name, pointer, protection);
	}

	template<typename _object_type, typename _res_type, typename ... _args>
	StaticFunctionInfo* ReflectionInitializationTypeProcessor::StaticMethod(_object_type* object, Type* type, const char* name, _res_type(*pointer)(_args ...), ProtectSection protection)
	{
		return TypeInitializer::RegStaticFunction<_object_type, _res_type, _args ...>(type, name, pointer, protection);
	}

}
