//@CODETOOLIGNORE

#pragma once

#include "o2/Utils/Delegates.h"
#include "o2/Utils/Reflection/Attributes.h"
#include "o2/Utils/Reflection/TypeSerializer.h"
#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/StringDef.h"

#define TypeOf(TYPE) GetTypeOf<TYPE>()

namespace o2
{
	class FieldInfo;
	class FunctionInfo;
	class IAbstractValueProxy;
	class IObject;
	class StaticFunctionInfo;
	class Type;

	template<typename _type>
	class IValueProxy;

	template<typename _key_type, typename _value_type>
	class Map;

	template<typename _type, typename _getter>
	const Type& GetTypeOf();

	template<typename _element_type>
	struct VectorCountFieldSerializer;

	typedef UInt TypeId;

	// ---------------
	// Type of a value
	// ---------------
	class Type
	{
	public:
		enum class Usage
		{
			Regular, Object, Vector, Map, StringAccessor, Enumeration, Pointer, Property
		};

		struct BaseType
		{
			const Type* type;
			void*(*dynamicCastUpFunc)(void*);
			void*(*dynamicCastDownFunc)(void*);

			bool operator==(const BaseType& other) const { return type == other.type; }
		};

	public:
		// Default constructor
		Type(const String& name, int size, ITypeSerializer* serializer);

		// Destructor
		virtual ~Type();

		// Check equals operator
		bool operator==(const Type& other) const;

		// Check not equals operator
		bool operator!=(const Type& other) const;

		// Returns name of type
		const String& GetName() const;

		// Returns id of type
		TypeId ID() const;

		// Returns size of type in bytes
		int GetSize() const;

		// Is this type based on other
		bool IsBasedOn(const Type& other) const;

		// Returns pointer of type (type -> type*)
		virtual const Type* GetPointerType() const = 0;

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns vector of base types
		const Vector<BaseType>& GetBaseTypes() const;

		// Returns fields informations array
		const Vector<FieldInfo>& GetFields() const;

		// Returns fields informations array with all base types
		Vector<const FieldInfo*> GetFieldsWithBaseClasses() const;

		// Returns functions informations array
		const Vector<FunctionInfo*>& GetFunctions() const;

		// Returns static functions informations array
		const Vector<StaticFunctionInfo*>& GetStaticFunctions() const;

		// Returns functions informations array with all base types
		Vector<FunctionInfo*> GetFunctionsWithBaseClasses() const;

		// Returns functions informations array with all base types
		Vector<StaticFunctionInfo*> GetStaticFunctionsWithBaseClasses() const;

		// Returns field information by name
		const FieldInfo* GetField(const String& name) const;

		// Returns function info by name
		const FunctionInfo* GetFunction(const String& name) const;

		// Returns static function info by name
		const StaticFunctionInfo* GetStaticFunction(const String& name) const;

		// Invokes function with name
		template<typename _res_type, typename ... _args>
		_res_type Invoke(const String& name, void* object, _args ... args) const;

		// Invokes function with name
		template<typename _res_type, typename ... _args>
		_res_type InvokeStatic(const String& name, _args ... args) const;

		// Returns derived types
		Vector<const Type*> GetDerivedTypes(bool deep = true) const;

		// Creates sample copy and returns him
		virtual void* CreateSample() const = 0;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const;

		// Returns abstract value proxy for object value
		virtual IAbstractValueProxy* GetValueProxy(void* object) const = 0;

		// Serializes value from ptr
		virtual void Serialize(void* ptr, DataValue& data) const;

		// Deserializes value from ptr
		virtual void Deserialize(void* ptr, const DataValue& data) const;

		// Returns is values getted from object A and object B equals
		virtual bool IsValueEquals(void* objectA, void* objectB) const;

		// Copies value getter from objectB to value from objectA
		virtual void CopyValue(void* objectA, void* objectB) const;

		// Returns type serializer
		ITypeSerializer* GetSerializer() const;

	public:
		// --------------------
		// Dummy type container
		// --------------------
		struct Dummy { static Type* type; };

	protected:
		TypeId mId;   // Id of type
		String mName; // Name of object type
		int    mSize; // Size of type in bytes

		Vector<BaseType> mBaseTypes; // Base types ids with offset 

		Vector<FieldInfo>           mFields;          // Fields information
		Vector<FunctionInfo*>       mFunctions;       // Functions informations
		Vector<StaticFunctionInfo*> mStaticFunctions; // Functions informations

		mutable Type* mPtrType = nullptr; // Pointer type from this

		ITypeSerializer* mSerializer = nullptr; // Value serializer

		friend class FieldInfo;
		friend class FunctionInfo;
		friend class PointerType;
		friend class Reflection;
		friend class TypeInitializer;
		friend class VectorType;

		template<typename _type, typename _accessor_type>
		friend class TStringPointerAccessorType;
	};

	// -------------------
	// Type specialization
	// -------------------
	template<typename _type>
	class TType: public Type
	{
	public:
		// Default constructor
		TType(const String& name, int size);

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	// --------------------------------------------------------------------
	// Type of objects, derived from IObject. Can be casted to/from IObject
	// --------------------------------------------------------------------
	class ObjectType: public Type
	{
	public:
		// Constructor
		ObjectType(const String& name, int size, void*(*castFromFunc)(void*), void*(*castToFunc)(void*), ITypeSerializer* serializer);

		// Returns type usage
		Usage GetUsage() const override;

		// Dynamically casts from IObject* to type pointer
		void* DynamicCastFromIObject(IObject* object) const;

		// Dynamically casts from IObject* to type pointer
		IObject* DynamicCastToIObject(void* object) const;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

	protected:
		void*(*mCastFromFunc)(void*); // Dynamic cast function from IObject
		void*(*mCastToFunc)(void*); // Dynamic cast function from IObject
	};

	// -----------------------
	// Specialized object type
	// -----------------------
	template<typename _type>
	class TObjectType: public ObjectType
	{
	public:
		// Default constructor
		TObjectType(const String& name, int size, void*(*castFromFunc)(void*), void*(*castToFunc)(void*));

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	// ----------------
	// Fundamental type
	// ----------------
	template<typename _type>
	class FundamentalType: public TType<_type>
	{
	public:
		// Default constructor
		FundamentalType(const String& name);
	};

	// ------------
	// Pointer type
	// ------------
	class PointerType: public Type
	{
	public:
		// Constructor
		PointerType(const Type* unptrType, ITypeSerializer* serializer);

		// Returns type usage
		Usage GetUsage() const override;

		// Returns unpointed type
		const Type* GetUnpointedType() const;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

	protected:
		const Type* mUnptrType;
	};

	// ------------------------
	// Specialized pointer type
	// ------------------------
	template<typename _type>
	class TPointerType: public PointerType
	{
	public:
		// Default constructor
		TPointerType(const Type* unptrType);

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	// -------------
	// Property type
	// -------------
	class PropertyType: public Type
	{
	public:
		// Default constructor
		PropertyType(const String& name, int size, ITypeSerializer* serializer);

		// Returns type usage
		Usage GetUsage() const;

		// Returns type of value
		const Type* GetValueType() const;

		// Returns value from property casted to void*
		virtual void* GetValueAsPtr(void* propertyPtr) const = 0;

	protected:
		const Type* mValueType;
	};

	// -------------------------
	// Specialized Property type
	// -------------------------
	template<typename _value_type, typename _property_type>
	class TPropertyType: public PropertyType
	{
		struct RealFieldGetter;
		struct NullFieldGetter;

	public:
		// Constructor
		TPropertyType();

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

		// Returns value from property casted to void*
		void* GetValueAsPtr(void* propertyPtr) const override;

		// Returns value from property
		_value_type GetValue(void* propertyPtr) const;
	};

	// ----------------------
	// Type of Vector<> value
	// ----------------------
	class VectorType: public Type
	{
	public:
		// Default constructor
		VectorType(const String& name, int size, ITypeSerializer* serializer);

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns type of vector element
		const Type* GetElementType() const;

		// Returns size of vector by pointer
		virtual int GetObjectVectorSize(void* object) const = 0;

		// Sets size of vector by pointer
		virtual void SetObjectVectorSize(void* object, int size) const = 0;

		// Returns element's pointer by index
		virtual void* GetObjectVectorElementPtr(void* object, int idx) const = 0;

		// Returns element's value proxy by index
		virtual IAbstractValueProxy* GetObjectVectorElementProxy(void* object, int idx) const = 0;

		// Removes element at idx in vector
		virtual void RemoveObjectVectorElement(void* object, int idx) const = 0;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

		// Returns element field info. Element field info is same for all elements
		FieldInfo* GetElementFieldInfo() const;

		// Returns count field info. This field info can serialize and deserialize size of vector with storing and restoring elements
		FieldInfo* GetCountFieldInfo() const;

	protected:
		const Type* mElementType;
		FieldInfo*  mElementFieldInfo;
		FieldInfo*  mCountFieldInfo;

		template<typename _element_type>
		friend struct VectorCountFieldSerializer;
	};

	// -----------------------
	// Specialized vector type
	// -----------------------
	template<typename _element_type>
	class TVectorType: public VectorType
	{
	public:
		// Default constructor
		TVectorType();

		// Returns size of vector by pointer
		int GetObjectVectorSize(void* object) const override;

		// Sets size of vector by pointer
		void SetObjectVectorSize(void* object, int size) const override;

		// Returns element's pointer by index
		void* GetObjectVectorElementPtr(void* object, int idx) const override;

		// Returns element's value proxy by index
		IAbstractValueProxy* GetObjectVectorElementProxy(void* object, int idx) const override;

		// Removes element at idx in vector
		void RemoveObjectVectorElement(void* object, int idx) const override;

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	template<typename _element_type>
	struct VectorCountFieldSerializer : public ITypeSerializer
	{
		VectorCountFieldSerializer() { }
		void Serialize(void* object, DataValue& data) const;
		void Deserialize(void* object, DataValue& data) const;
		bool Equals(void* objectA, void* objectB) const;
		void Copy(void* objectA, void* objectB) const;
		ITypeSerializer* Clone() const;
	};

	// --------------------------
	// Type of Dictionary<> value
	// --------------------------
	class MapType: public Type
	{
	public:
		// Default constructor
		MapType(const Type* keyType, const Type* valueType, int size, ITypeSerializer* serializer);

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns type of dictionary key
		const Type* GetKeyType() const;

		// Returns type of dictionary value
		const Type* GetValueType() const;

		// Returns size of dictionary by pointer
		int GetObjectDictionarySize(void* object) const;

		// Returns element's key pointer by index
		void* GetObjectDictionaryKeyPtr(void* object, int idx) const;

		// Returns element's value pointer by index
		void* GetObjectDictionaryValuePtr(void* object, int idx) const;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

	protected:
		const Type* mKeyType;
		const Type* mValueType;

		SharedLambda<int(void*)>        mGetDictionaryObjectSizeFunc;
		SharedLambda<void*(void*, int)> mGetObjectDictionaryKeyPtrFunc;
		SharedLambda<void*(void*, int)> mGetObjectDictionaryValuePtrFunc;
	};

	template<typename _key_type, typename _value_type>
	class TMapType: public MapType
	{
	public:
		// Default constructor
		TMapType();

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	// -------------------------------------------
	// Accessor<_return_type*, const String&> type
	// -------------------------------------------
	class StringPointerAccessorType : public Type
	{
	public:
		// Default constructor
		StringPointerAccessorType(const String& name, int size, ITypeSerializer* serializer);

		// Returns type usage
		Usage GetUsage() const override;

		// Returns value pointer by key
		virtual void* GetValue(void* object, const String& key) const = 0;

		// Returns all values with keys
		virtual Map<String, void*> GetAllValues(void* object) const = 0;

		// Returns type of return values
		virtual const Type* GetReturnType() const = 0;
	};

	template<typename _return_type, typename _accessor_type>
	class TStringPointerAccessorType: public StringPointerAccessorType
	{
	public:
		TStringPointerAccessorType();

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const override;

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;

		// Returns value pointer by key
		void* GetValue(void* object, const String& key) const override;

		// Returns all values with keys
		Map<String, void*> GetAllValues(void* object) const override;

		// Returns type of return values
		const Type* GetReturnType() const override;

	protected:
		const Type* mReturnType;
	};

	// ----------------
	// Enumeration type
	// ----------------
	class EnumType: public Type
	{
	public:
		// Constructor
		EnumType(const String& name, int size, ITypeSerializer* serializer);

		// Returns type usage
		Usage GetUsage() const;

		// Returns enum entries
		const Map<int, String>& GetEntries() const;

	protected:
		Map<int, String> mEntries;

		friend class Reflection;
	};

	// ---------------------
	// Specialized Enum type
	// ---------------------
	template<typename _type>
	class TEnumType: public EnumType
	{
	public:
		// Constructor
		TEnumType(const String& name, int size);

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	// --------------------------
	// Fundamental type container
	// --------------------------
	template<typename _type>
	class FundamentalTypeContainer
	{
	protected:
		static Type* type;

		template<typename _type_>
		friend const Type& o2::GetTypeOf();

		friend class Reflection;

	public:
		template<typename _processor_type>
		static void InitializeType(_type* object, _processor_type& processor);
	};

	// --------------------------
	// Enumeration type container
	// --------------------------
	template<typename _type>
	class EnumTypeContainer
	{
	public:
		static const Map<int, String>& GetEntries() { return type->GetEntries(); }

	protected:
		static EnumType* type;

		template<typename _type_>
		friend const Type& o2::GetTypeOf();

		friend class Reflection;
	};

	// ----------------
	// Type initializer
	// ----------------
	class TypeInitializer
	{
	public:
		// Adds basic type
		template<typename _this_type, typename _base_type, typename X = typename std::conditional<std::is_base_of<IObject, _base_type>::value, _base_type, Type::Dummy>::type>
		static void AddBaseType(Type*& type);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, void*(*pointerGetter)(void*), _type& value, ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...), ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const, ProtectSection section);

		// Registers static function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static StaticFunctionInfo* RegStaticFunction(Type* type, const String& name, _res_type(*pointer)(_args ...), ProtectSection section);
	};

	// Here is the sample of type processing class
	class ITypeProcessor
	{
	public:
		class FieldInfo {};
		class MethodInfo {};

		template<typename _object_type>
		void Start(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartMethods(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name) {}

		template<typename _object_type, typename _field_type>
		FieldInfo& Field(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*), _field_type& field, ProtectSection protection) {}

		template<typename _object_type, typename _res_type, typename ... _args>
		MethodInfo* Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...), ProtectSection protection) {}

		template<typename _object_type, typename _res_type, typename ... _args>
		MethodInfo* Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...) const, ProtectSection protection) { }

		template<typename _object_type, typename _res_type, typename ... _args>
		MethodInfo* StaticMethod(_object_type* object, Type* type, const char* name, _res_type(*pointer)(_args ...), ProtectSection protection) { }
	};
}

typedef void*(*GetValuePointerFuncPtr)(void*);

#define DECLARE_CLASS(CLASS)                                                                                   \
    o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)										             
																											   
#define CLASS_BASES_META(CLASS)                                                                                \
    template<typename _type_processor> void CLASS::ProcessBaseTypes(CLASS* object, _type_processor& processor) \
	{                                                                                                          \
        typedef CLASS thisclass;                                                                               \
		processor.template StartBases<CLASS>(object, type);														         
																											   
#define CLASS_FIELDS_META(CLASS)                                                                               \
    template<typename _type_processor> void CLASS::ProcessFields(CLASS* object, _type_processor& processor)    \
	{                                                                                                          \
        typedef CLASS thisclass;                                                                               \
		processor.template StartFields<CLASS>(object, type);													             
																											   
#define CLASS_METHODS_META(CLASS)                                                                              \
    template<typename _type_processor> void CLASS::ProcessMethods(CLASS* object, _type_processor& processor)   \
	{                                                                                                          \
        typedef CLASS thisclass;                                                                               \
		processor.template StartMethods<CLASS>(object, type);

#define META_TEMPLATES(...) \
    template<__VA_ARGS__>

#define DECLARE_CLASS_MANUAL(CLASS) \
    DECLARE_CLASS(CLASS)	

#define CLASS_BASES_META_MANUAL(CLASS) \
    CLASS_BASES_META(CLASS)

#define CLASS_FIELDS_META_MANUAL(CLASS) \
    CLASS_FIELDS_META(CLASS)

#define CLASS_METHODS_META_MANUAL(CLASS) \
    CLASS_METHODS_META(CLASS)

#define META_TEMPLATES_MANUAL(...) \
    META_TEMPLATES(__VA_ARGS__)

#define FUNDAMENTAL_META(NAME) \
    template<>                                                                                                                                \
    template<typename _type_processor> void FundamentalTypeContainer<NAME>::InitializeType(NAME* object, _type_processor& processor) \
	{                                                                                                                                         \
        typedef NAME thisclass;                                                                                                               \
		processor.template StartFields<NAME>(object, type);

#define BASE_CLASS(CLASS) \
    processor.template BaseType<thisclass, CLASS>(object, type, #CLASS)

#define FIELD(NAME, PROTECT_SECTION) \
    processor.template Field<thisclass, decltype(object->NAME)>(object, type, #NAME, (GetValuePointerFuncPtr)([](void* obj) { return (void*)&((thisclass*)obj)->NAME; }), object->NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FIELD(NAME) \
    processor.template Field<thisclass, decltype(object->NAME)>(object, type, #NAME, (GetValuePointerFuncPtr)([](void* obj) { return (void*)&((thisclass*)obj)->NAME; }), object->NAME, ProtectSection::Public)

#define PRIVATE_FIELD(NAME) \
    processor.template Field<thisclass, decltype(object->NAME)>(object, type, #NAME, (GetValuePointerFuncPtr)([](void* obj) { return (void*)&((thisclass*)obj)->NAME; }), object->NAME, ProtectSection::Private)

#define PROTECTED_FIELD(NAME) \
    processor.template Field<thisclass, decltype(object->NAME)>(object, type, #NAME, (GetValuePointerFuncPtr)([](void* obj) { return (void*)&((thisclass*)obj)->NAME; }), object->NAME, ProtectSection::Protected)

#define DEFAULT_VALUE \
	SetDefaultValue

#define ATTRIBUTE(NAME) \
    AddAttribute(new NAME)

#define ATTRIBUTES(...)

#define FUNCTION(PROTECT_SECTION, RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Public)

#define PRIVATE_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Private)

#define PROTECTED_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Protected)

#define STATIC_FUNCTION(PROTECT_SECTION, RETURN_TYPE, NAME, ...) \
    processor.template StaticMethod<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_STATIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template StaticMethod<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Public)

#define PRIVATE_STATIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template StaticMethod<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Private)

#define PROTECTED_STATIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template StaticMethod<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Protected)

#define END_META }

#include "o2/Utils/Property.h"
#include "o2/Utils/Reflection/FieldInfo.h"
#include "o2/Utils/Reflection/FunctionInfo.h"
#include "o2/Utils/Reflection/TypeTraits.h"
#include "o2/Utils/Types/StringImpl.h"
#include "o2/Utils/Types/UID.h"
#include "o2/Utils/ValueProxy.h"

namespace o2
{
	// -------------------
	// Type implementation
	// -------------------

	template<typename _res_type, typename ... _args>
	_res_type Type::Invoke(const String& name, void* object, _args ... args) const
	{
		if (auto func = GetFunction(name))
			return func->Invoke<_res_type, _args ...>(object, args ...);

		return _res_type();
	}

	template<typename _res_type, typename ... _args>
	_res_type Type::InvokeStatic(const String& name, _args ... args) const
	{
		if (auto func = GetStaticFunction(name))
			return func->Invoke<_res_type, _args ...>(args ...);

		return _res_type();
	}

	// --------------------
	// TType implementation
	// --------------------

	template<typename _type>
	TType<_type>::TType(const String& name, int size):
		Type(name, size, mnew TypeSerializer<_type>())
	{}

	template<typename _type>
	void* TType<_type>::CreateSample() const
	{
		return mnew _type();
	}
	template<typename _type>
	IAbstractValueProxy* TType<_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<_type>((_type*)object);
	}

	template<typename _type>
	const Type* TType<_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_type>(this);

		return mPtrType;
	}

	// --------------------------
	// TObjectType implementation
	// --------------------------

	template<typename _type>
	TObjectType<_type>::TObjectType(const String& name, int size, void*(*castFromFunc)(void*), void*(*castToFunc)(void*)):
		ObjectType(name, size, castFromFunc, castToFunc, mnew TypeSerializer<_type>())
	{}

	template<typename _type>
	void* TObjectType<_type>::CreateSample() const
	{
		return mnew _type();
	}

	template<typename _type>
	IAbstractValueProxy* TObjectType<_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<_type>((_type*)object);
	}

	template<typename _type>
	const Type* TObjectType<_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_type>(this);

		return mPtrType;
	}

	// ------------------------------
	// FundamentalType implementation
	// ------------------------------

	template<typename _type>
	FundamentalType<_type>::FundamentalType(const String& name):
		TType<_type>(name, sizeof(_type))
	{}

	// -----------------------------
	// TPointerType<> implementation
	// -----------------------------

	template<typename _type>
	TPointerType<_type>::TPointerType(const Type* unptrType):
		PointerType(unptrType, mnew TypeSerializer<_type*>())
	{}

	template<typename _type>
	void* TPointerType<_type>::CreateSample() const
	{
		return mnew void*();
	}

	template<typename _type>
	IAbstractValueProxy* TPointerType<_type>::GetValueProxy(void* object) const
	{
		auto unptrProxy = dynamic_cast<IValueProxy<_type>*>(GetUnpointedType()->GetValueProxy(*(void**)object));
		return mnew PointerValueProxy<_type*>((_type**)object);
	}

	template<typename _type>
	const Type* TPointerType<_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_type>(this);

		return mPtrType;
	}

	// ------------------------------
	// TPropertyType<> implementation
	// ------------------------------

	template<typename _value_type, typename _property_type>
	TPropertyType<_value_type, _property_type>::TPropertyType():
		PropertyType((String)(typeid(_property_type).name()) + (String)"<" + TypeOf(_value_type).GetName() + ">", sizeof(_value_type), 
					 mnew TypeSerializer<_property_type>())
	{
		mValueType = &GetTypeOf<_value_type>();
	}

	template<typename _value_type, typename _property_type>
	void* TPropertyType<_value_type, _property_type>::CreateSample() const
	{
		return nullptr;
	}
	template<typename _value_type, typename _property_type>
	IAbstractValueProxy* TPropertyType<_value_type, _property_type>::GetValueProxy(void* object) const
	{
		return mnew PropertyValueProxy<_value_type, _property_type>((_property_type*)object);
	}

	template<typename _value_type, typename _property_type>
	const Type* TPropertyType<_value_type, _property_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_property_type>(this);

		return mPtrType;
	}

	template<typename _value_type, typename _property_type>
	void* TPropertyType<_value_type, _property_type>::GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const
	{
		if constexpr (std::is_pointer<_value_type>::value)
		{
			_property_type* prop = (_property_type*)object;
			_value_type valuePtr = prop->Get();
			return mValueType->GetFieldPtr(&valuePtr, path, fieldInfo);
		}

		return nullptr;
	}

	template<typename _value_type, typename _property_type>
	void* TPropertyType<_value_type, _property_type>::GetValueAsPtr(void* propertyPtr) const
	{
		if constexpr (std::is_pointer<_value_type>::value)
		{
			_property_type* prop = (_property_type*)propertyPtr;
			return prop->Get();
		}

		return nullptr;
	}

	template<typename _value_type, typename _property_type>
	_value_type TPropertyType<_value_type, _property_type>::GetValue(void* propertyPtr) const
	{
		_property_type* prop = (_property_type*)propertyPtr;
		return prop->Get();
	}

	// --------------------------
	// TVectorType implementation
	// --------------------------

	template<typename _element_type>
	void* TVectorType<_element_type>::GetObjectVectorElementPtr(void* object, int idx) const
	{
		return &((Vector<_element_type>*)object)->Get(idx);
	}

	template<typename _element_type>
	IAbstractValueProxy* TVectorType<_element_type>::GetObjectVectorElementProxy(void* object, int idx) const
	{
		return mElementType->GetValueProxy(&((Vector<_element_type>*)object)->Get(idx));
	}

	template<typename _element_type>
	void TVectorType<_element_type>::SetObjectVectorSize(void* object, int size) const
	{
		auto vectorObj = ((Vector<_element_type>*)object);
		int oldSize = vectorObj->Count();
		vectorObj->Resize(size);

		for (int i = oldSize; i < size; i++)
			(*vectorObj)[i] = _element_type();
	}

	template<typename _element_type>
	void TVectorType<_element_type>::RemoveObjectVectorElement(void* object, int idx) const
	{
		((Vector<_element_type>*)object)->RemoveAt(idx);
	}

	template<typename _element_type>
	void* TVectorType<_element_type>::CreateSample() const
	{
		return mnew Vector<_element_type>();
	}

	template<typename _element_type>
	IAbstractValueProxy* TVectorType<_element_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<Vector<_element_type>>((Vector<_element_type>*)object);
	}

	template<typename _element_type>
	int TVectorType<_element_type>::GetObjectVectorSize(void* object) const
	{
		return ((Vector<_element_type>*)object)->Count();
	}

	template<typename _element_type>
	TVectorType<_element_type>::TVectorType():
		VectorType((String)"o2::Vector<" + GetTypeOf<_element_type>().GetName() + ">", sizeof(Vector<_element_type>), mnew TypeSerializer<Vector<_element_type>>())
	{
		mElementType = &GetTypeOf<_element_type>();

		mElementFieldInfo = mnew FieldInfo(this, "element", 0, mElementType, ProtectSection::Private);
		mCountFieldInfo = mnew FieldInfo(this, "count", 0, &GetTypeOf<int>(), ProtectSection::Public, mnew VectorCountFieldSerializer<_element_type>());
	}

	template<typename _element_type>
	const Type* TVectorType<_element_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<Vector<_element_type>>(this);

		return mPtrType;
	}

	template<typename _element_type>
	void VectorCountFieldSerializer<_element_type>::Serialize(void* object, DataValue& data) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());

		int size = type.GetObjectVectorSize(object);
		data["Size"].Set(size);

		DataValue& elements = data["Elements"];
		for (int i = 0; i < size; i++)
		{
			void* elementPtr = type.GetObjectVectorElementPtr(object, i);
			type.mElementFieldInfo->Serialize(elementPtr, elements.AddMember("Element" + (String)i));
		}
	}

	template<typename _element_type>
	void VectorCountFieldSerializer<_element_type>::Deserialize(void* object, DataValue& data) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());
		int size = type.GetObjectVectorSize(object);
		int newSize;
		data["Size"].Get(newSize);
		type.SetObjectVectorSize(object, newSize);

		if (auto elementsData = data.GetMember("Elements"))
		{
			for (int i = size; i < newSize; i++)
			{
				if (auto elementData = elementsData->GetMember("Element" + (WString)i))
				{
					void* elementPtr = type.GetObjectVectorElementPtr(object, i);
					type.mElementFieldInfo->Deserialize(elementPtr, *elementData);
				}
			}
		}
	}

	template<typename _element_type>
	bool VectorCountFieldSerializer<_element_type>::Equals(void* objectA, void* objectB) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());
		return type.GetObjectVectorSize(objectA) == type.GetObjectVectorSize(objectB);
	}

	template<typename _element_type>
	void VectorCountFieldSerializer<_element_type>::Copy(void* objectA, void* objectB) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());
		type.SetObjectVectorSize(objectA, type.GetObjectVectorSize(objectB));
	}

	template<typename _element_type>
	ITypeSerializer* VectorCountFieldSerializer<_element_type>::Clone() const
	{
		return mnew VectorCountFieldSerializer();
	}

	// ------------------------------
	// TDictionaryType implementation
	// ----------------------------- -

	template<typename _key_type, typename _value_type>
	TMapType<_key_type, _value_type>::TMapType():
		MapType(&GetTypeOf<_key_type>(), &GetTypeOf<_value_type>(), sizeof(Map<_key_type, _value_type>), mnew TypeSerializer<Map<_key_type, _value_type>>())
	{
		mGetDictionaryObjectSizeFunc = [](void* obj) { return ((Map<_key_type, _value_type>*)obj)->Count(); };

		mGetObjectDictionaryKeyPtrFunc = [](void* obj, int idx) {
			auto it = ((Map<_key_type, _value_type>*)obj)->Begin();
			while (idx >= 0) it++;

			return (void*)(&it->first);
		};

		mGetObjectDictionaryValuePtrFunc = [](void* obj, int idx) {
			auto it = ((Map<_key_type, _value_type>*)obj)->Begin();
			while (idx >= 0) it++;

			return (void*)(&it->second);
		};
	}

	template<typename _key_type, typename _value_type>
	void* TMapType<_key_type, _value_type>::CreateSample() const
	{
		return mnew Map<_key_type, _value_type>();
	}

	template<typename _key_type, typename _value_type>
	IAbstractValueProxy* TMapType<_key_type, _value_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<Map<_key_type, _value_type>>((Map<_key_type, _value_type>*)object);
	}

	template<typename _key_type, typename _value_type>
	const Type* TMapType<_key_type, _value_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<Map<_key_type, _value_type>>(this);

		return mPtrType;
	}

	// ----------------------------------------
	// StringPointerAccessorType implementation
	// ----------------------------------------

	template<typename _return_type, typename _accessor_type>
	TStringPointerAccessorType<_return_type, _accessor_type>::TStringPointerAccessorType():
		StringPointerAccessorType((String)(typeid(_accessor_type).name()) + (String)"<" + TypeOf(_return_type).GetName() + ">",
			 sizeof(_accessor_type), mnew TypeSerializer<_accessor_type>())
	{
		mReturnType = &GetTypeOf<_return_type>();
	}

	template<typename _return_type, typename _accessor_type>
	void* TStringPointerAccessorType<_return_type, _accessor_type>::CreateSample() const
	{
		return nullptr;
	}

	template<typename _return_type, typename _accessor_type>
	IAbstractValueProxy* TStringPointerAccessorType<_return_type, _accessor_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<_accessor_type>((_accessor_type*)object);
	}

	template<typename _return_type, typename _accessor_type>
	void* TStringPointerAccessorType<_return_type, _accessor_type>::GetFieldPtr(void* object, const String& path, const FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		_accessor_type* accessor = (_accessor_type*)object;
		if (_return_type value = accessor->Get(pathPart))
			return mReturnType->GetFieldPtr(&value, path.SubStr(delPos + 1), fieldInfo);

		return nullptr;
	}

	template<typename _return_type, typename _accessor_type>
	const Type* TStringPointerAccessorType<_return_type, _accessor_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_accessor_type>(this);

		return mPtrType;
	}

	template<typename _return_type, typename _accessor_type>
	void* TStringPointerAccessorType<_return_type, _accessor_type>::GetValue(void* object, const String& key) const
	{
		_accessor_type* accessor = (_accessor_type*)object;
		return (void*)(accessor->Get(key));
	}

	template<typename _return_type, typename _accessor_type>
	Map<String, void*> TStringPointerAccessorType<_return_type, _accessor_type>::GetAllValues(void* object) const
	{
		_accessor_type* accessor = (_accessor_type*)object;
		auto all = accessor->GetAll();
		Map<String, void*> res;
		for (auto& kv : all)
			res.Add(kv.first, (void*)(kv.second));

		return res;
	}

	template<typename _return_type, typename _accessor_type>
	const Type* TStringPointerAccessorType<_return_type, _accessor_type>::GetReturnType() const
	{
		return mReturnType;
	}

	// ------------------------
	// TEnumType implementation
	// ------------------------

	template<typename _type>
	TEnumType<_type>::TEnumType(const String& name, int size):
		EnumType(name, size, mnew TypeSerializer<_type>())
	{}

	template<typename _type>
	void* TEnumType<_type>::CreateSample() const
	{
		return mnew _type();
	}

	template<typename _type>
	IAbstractValueProxy* TEnumType<_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<_type>((_type*)object);
	}

	template<typename _type>
	const Type* TEnumType<_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_type>(this);

		return mPtrType;
	}

	// ------------------------------
	// TypeInitializer implementation
	// ------------------------------

	template<typename _this_type, typename _base_type, typename X>
	void TypeInitializer::AddBaseType(Type*& type)
	{
		if (std::is_same<X, Type::Dummy>::value)
			return;

		Type::BaseType baseTypeInfo;
		baseTypeInfo.type = X::type;
		baseTypeInfo.dynamicCastUpFunc = &Reflection::CastFunc<_this_type, _base_type>;
		baseTypeInfo.dynamicCastDownFunc = &Reflection::CastFunc<_base_type, _this_type>;

		type->mBaseTypes.Add(baseTypeInfo);
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, void*(*pointerGetter)(void*), _type& value,
										 ProtectSection section)
	{
		auto valType = &TypeOf(_type);
		type->mFields.emplace_back(FieldInfo(type, name, pointerGetter, valType, section));

		return type->mFields.Last();
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...),
											   ProtectSection section)
	{
		auto retType = &TypeOf(_res_type);

		auto funcInfo = mnew SpecFunctionInfo<_class_type, _res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mIsContant = false;
		funcInfo->mProtectSection = section;
		funcInfo->mOwnerType = type;
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name,
											   _res_type(_class_type::*pointer)(_args ...) const, ProtectSection section)
	{
		auto retType = &TypeOf(_res_type);

		auto funcInfo = mnew SpecConstFunctionInfo<_class_type, _res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mIsContant = true;
		funcInfo->mProtectSection = section;
		funcInfo->mOwnerType = type;
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	StaticFunctionInfo* TypeInitializer::RegStaticFunction(Type* type, const String& name, _res_type(*pointer)(_args ...), ProtectSection section)
	{
		auto retType = &TypeOf(_res_type);

		auto funcInfo = mnew SpecStaticFunctionInfo<_res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mProtectSection = section;
		funcInfo->mOwnerType = type;
		type->mStaticFunctions.Add(funcInfo);

		return funcInfo;
	}

	FUNDAMENTAL_META(RectF)
	{
		PUBLIC_FIELD(left);
		PUBLIC_FIELD(right);
		PUBLIC_FIELD(top);
		PUBLIC_FIELD(bottom);
	}
	END_META;

	FUNDAMENTAL_META(RectI)
	{
		PUBLIC_FIELD(left);
		PUBLIC_FIELD(right);
		PUBLIC_FIELD(top);
		PUBLIC_FIELD(bottom);
	}
	END_META;

	FUNDAMENTAL_META(BorderF)
	{
		PUBLIC_FIELD(left);
		PUBLIC_FIELD(right);
		PUBLIC_FIELD(top);
		PUBLIC_FIELD(bottom);
	}
	END_META;

	FUNDAMENTAL_META(BorderI)
	{
		PUBLIC_FIELD(left);
		PUBLIC_FIELD(right);
		PUBLIC_FIELD(top);
		PUBLIC_FIELD(bottom);
	}
	END_META;

	FUNDAMENTAL_META(Vec2F)
	{
		PUBLIC_FIELD(x);
		PUBLIC_FIELD(y);
	}
	END_META;

	FUNDAMENTAL_META(Vec2I)
	{
		PUBLIC_FIELD(x);
		PUBLIC_FIELD(y);
	}
	END_META;

	FUNDAMENTAL_META(Color4)
	{
		PUBLIC_FIELD(r);
		PUBLIC_FIELD(g);
		PUBLIC_FIELD(b);
		PUBLIC_FIELD(a);
	}
	END_META;

	FUNDAMENTAL_META(int) END_META;
	FUNDAMENTAL_META(bool) END_META;
	FUNDAMENTAL_META(char) END_META;
	FUNDAMENTAL_META(wchar_t) END_META;
	FUNDAMENTAL_META(short int) END_META;
	FUNDAMENTAL_META(long int) END_META;
	FUNDAMENTAL_META(long long int) END_META;
	FUNDAMENTAL_META(unsigned char) END_META;
	FUNDAMENTAL_META(unsigned short int) END_META;
	FUNDAMENTAL_META(unsigned int) END_META;
	FUNDAMENTAL_META(unsigned long int) END_META;
	FUNDAMENTAL_META(unsigned long long int) END_META;
	FUNDAMENTAL_META(float) END_META;
	FUNDAMENTAL_META(double) END_META;
	FUNDAMENTAL_META(long double) END_META;
	FUNDAMENTAL_META(Basis) END_META;
	FUNDAMENTAL_META(Vertex2) END_META;
	FUNDAMENTAL_META(String) END_META;
	FUNDAMENTAL_META(WString) END_META;
	FUNDAMENTAL_META(DataValue) END_META;
	FUNDAMENTAL_META(UID) END_META;
}
