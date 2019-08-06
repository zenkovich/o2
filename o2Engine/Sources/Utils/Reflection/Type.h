//@CODETOOLIGNORE

#pragma once

#include "Utils/Delegates.h"
#include "Utils/Reflection/Attribute.h"
#include "Utils/Reflection/IFieldSerializer.h"
#include "Utils/Reflection/SearchPassedObject.h"
#include "Utils/Types/CommonTypes.h"
#include "Utils/Types/Containers/Dictionary.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/StringDef.h"

#define TypeOf(TYPE) GetTypeOf<TYPE>()

namespace o2
{
	class FieldInfo;
	class FunctionInfo;
	class IAbstractValueProxy;
	class IObject;
	class Type;

	template<typename _type>
	class IValueProxy;

	template<typename _key_type, typename _value_type>
	class Dictionary;

	template<typename _type, typename _getter>
	const Type& GetTypeOf();

	template<typename T, typename X>
	struct GetTypeHelper;

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
			Regular, Object, Vector, Dictionary, StringAccessor, Enumeration, Pointer, Property
		};

		struct BaseType
		{
			const Type* type;
			void*(*dynamicCastUpFunc)(void*);
			void*(*dynamicCastDownFunc)(void*);

			bool operator==(const BaseType& other) const { return type == other.type; }
		};

		typedef Vector<FieldInfo*> FieldInfosVec;
		typedef Vector<FunctionInfo*> FunctionsInfosVec;
		typedef Vector<Type*> TypesVec;
		typedef Vector<BaseType> BaseTypesVec;

	public:
		// Default constructor
		Type(const String& name, int size);

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
		const BaseTypesVec& GetBaseTypes() const;

		// Returns fields informations array
		const FieldInfosVec& GetFields() const;

		// Returns fields informations array with all base types
		FieldInfosVec GetFieldsWithBaseClasses() const;

		// Returns functions informations array
		const FunctionsInfosVec& GetFunctions() const;

		// Returns functions informations array with all base types
		FunctionsInfosVec GetFunctionsWithBaseClasses() const;

		// Returns field information by name
		FieldInfo* GetField(const String& name) const;

		// Returns function info by name
		const FunctionInfo* GetFunction(const String& name) const;

		// Invokes function with name
		template<typename _res_type, typename ... _args>
		_res_type Invoke(const String& name, void* object, _args ... args);

		// Returns derived types
		Vector<const Type*> GetDerivedTypes(bool deep = true) const;

		// Creates sample copy and returns him
		virtual void* CreateSample() const = 0;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		// Returns abstract value proxy for object value
		virtual IAbstractValueProxy* GetValueProxy(void* object) const = 0;

	public:
		// --------------------
		// Dummy type container
		// --------------------
		struct Dummy { static Type* type; };

	protected:
		TypeId            mId;                // Id of type
		String            mName;              // Name of object type
		BaseTypesVec      mBaseTypes;         // Base types ids with offset 
		FieldInfosVec     mFields;            // Fields information
		FunctionsInfosVec mFunctions;         // Functions informations
		mutable Type*     mPtrType = nullptr; // Pointer type from this
		int               mSize;              // Size of type in bytes

		friend class FieldInfo;
		friend class FunctionInfo;
		friend class PointerType;
		friend class Reflection;
		friend class TypeInitializer;
		friend class VectorType;

		template<typename _type, typename _accessor_type>
		friend class StringPointerAccessorType;
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
		ObjectType(const String& name, int size, void*(*castFromFunc)(void*), void*(*castToFunc)(void*));

		// Returns type usage
		Usage GetUsage() const override;

		// Dynamically casts from IObject* to type pointer
		void* DynamicCastFromIObject(IObject* object) const;

		// Dynamically casts from IObject* to type pointer
		IObject* DynamicCastToIObject(void* object) const;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const override;

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
		PointerType(const Type* unptrType);

		// Returns type usage
		Usage GetUsage() const override;

		// Returns unpointed type
		const Type* GetUnpointedType() const;

		// Returns filed pointer by path
		void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const override;

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
		PropertyType(const String& name, int size);

		// Returns type usage
		Usage GetUsage() const;

		// Returns type of value
		const Type* GetValueType() const;

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
		void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const override;

	private:
		struct RealFieldGetter
		{
			static void* GetFieldPtr(const TPropertyType<_value_type, _property_type>& type, void* object, const String& path, FieldInfo*& fieldInfo);
		};

		struct NullFieldGetter
		{
			static void* GetFieldPtr(const TPropertyType<_value_type, _property_type>& type, void* object, const String& path, FieldInfo*& fieldInfo);
		};
	};

	// ----------------------
	// Type of Vector<> value
	// ----------------------
	class VectorType: public Type
	{
	public:
		// Default constructor
		VectorType(const String& name, int size);

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

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

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
		int GetObjectVectorSize(void* object) const;

		// Sets size of vector by pointer
		void SetObjectVectorSize(void* object, int size) const;

		// Returns element's pointer by index
		void* GetObjectVectorElementPtr(void* object, int idx) const;

		// Returns element's value proxy by index
		IAbstractValueProxy* GetObjectVectorElementProxy(void* object, int idx) const;

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;
	};

	template<typename _element_type>
	struct VectorCountFieldSerializer : public IFieldSerializer
	{
		VectorCountFieldSerializer() { }
		void Serialize(void* object, DataNode& data) const;
		void Deserialize(void* object, DataNode& data) const;
		bool Equals(void* objectA, void* objectB) const;
		void Copy(void* objectA, void* objectB) const;
		IFieldSerializer* Clone() const;
	};

	// --------------------------
	// Type of Dictionary<> value
	// --------------------------
	class DictionaryType: public Type
	{
	public:
		// Default constructor
		DictionaryType(const Type* keyType, const Type* valueType, int size);

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns type of dictionary key
		const Type* GetKeyType() const;

		// Returns type of dictionary value
		const Type* GetValueType() const;

		// Returns size of dictionary by pointer
		int GetObjectDictionarySize(void* object) const;

		// Sets size of dictionary by pointer
		void SetObjectDictionarySize(void* object, int size) const;

		// Returns element's key pointer by index
		void* GetObjectDictionaryKeyPtr(void* object, int idx) const;

		// Returns element's value pointer by index
		void* GetObjectDictionaryValuePtr(void* object, int idx) const;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

	protected:
		const Type* mKeyType;
		const Type* mValueType;

		SharedLambda<int(void*)>        mGetDictionaryObjectSizeFunc;
		SharedLambda<void(void*, int)>  mSetDictionaryObjectSizeFunc;
		SharedLambda<void*(void*, int)> mGetObjectDictionaryKeyPtrFunc;
		SharedLambda<void*(void*, int)> mGetObjectDictionaryValuePtrFunc;
	};

	template<typename _key_type, typename _value_type>
	class TDictionaryType: public DictionaryType
	{
	public:
		// Default constructor
		TDictionaryType();

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
	template<typename _return_type, typename _accessor_type>
	class StringPointerAccessorType: public Type
	{
	public:
		StringPointerAccessorType();

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		// Creates sample copy and returns him
		void* CreateSample() const override;

		// Returns abstract value proxy for object value
		IAbstractValueProxy* GetValueProxy(void* object) const override;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const override;

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
		EnumType(const String& name, int size);

		// Returns type usage
		Usage GetUsage() const;

		// Returns enum entries
		const Dictionary<int, String>& GetEntries() const;

	protected:
		Dictionary<int, String> mEntries;

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

		template<typename _type_, typename _getter>
		friend const Type& o2::GetTypeOf();

		template<typename T>
		friend struct RegularTypeGetter;

		template<typename T, typename X>
		friend struct o2::GetTypeHelper;

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
		static const Dictionary<int, String>& GetEntries() { return type->GetEntries(); }

	protected:
		static EnumType* type;

		template<typename _type_, typename _getter>
		friend const Type& o2::GetTypeOf();

		template<typename T>
		friend struct RegularTypeGetter;

		template<typename T, typename X>
		friend struct o2::GetTypeHelper;

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
		MethodInfo* Method(_object_type* object, Type* type, const char* name, _res_type(_object_type::*pointer)(_args ...) const, ProtectSection protection) {}
	};
}

typedef void*(*GetValuePointerFuncPtr)(void*);

#define DECLARE_CLASS(CLASS)                                                                                            \
    o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)		

#define DECLARE_CLASS_MANUAL(CLASS)                                                                                     \
    o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)											            

#define CLASS_BASES_META(CLASS)                                                                                         \
    template<typename _type_processor> void CLASS::ProcessBaseTypes(CLASS* object, _type_processor& processor)          \
	{                                                                                                                   \
        typedef CLASS thisclass;                                                                                        \
		processor.template StartBases<CLASS>(object, type);															            

#define CLASS_FIELDS_META(CLASS)                                                                                        \
    template<typename _type_processor> void CLASS::ProcessFields(CLASS* object, _type_processor& processor)             \
	{                                                                                                                   \
        typedef CLASS thisclass;                                                                                        \
		processor.template StartFields<CLASS>(object, type);															            

#define CLASS_METHODS_META(CLASS)                                                                                       \
    template<typename _type_processor> void CLASS::ProcessMethods(CLASS* object, _type_processor& processor)            \
	{                                                                                                                   \
        typedef CLASS thisclass;                                                                                        \
		processor.template StartMethods<CLASS>(object, type);

#define META_TEMPLATES(...) \
    template<__VA_ARGS__>

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


#define ATTRIBUTE(NAME) \
    AddAttribute(new NAME)

#define ATTRIBUTES(...)

#define ATTRIBUTE_COMMENT_DEFINITION(X)

#define ATTRIBUTE_SHORT_DEFINITION(X)

#define FUNCTION(PROTECT_SECTION, RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Public)

#define PRIVATE_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Private)

#define PROTECTED_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.template Method<thisclass, RETURN_TYPE, ##__VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Protected)

#define END_META }

#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Reflection/FunctionInfo.h"
#include "Utils/Reflection/TypeTraits.h"
#include "Utils/Types/StringImpl.h"
#include "Utils/Types/UID.h"
#include "Utils/ValueProxy.h"

namespace o2
{
	// -------------------
	// Type implementation
	// -------------------

	template<typename _res_type, typename ... _args>
	_res_type Type::Invoke(const String& name, void* object, _args ... args)
	{
		const FunctionInfo* func = GetFunction(name);
		if (func)
			return func->Invoke(object, args ...);

		return _res_type();
	}

	// --------------------
	// TType implementation
	// --------------------

	template<typename _type>
	TType<_type>::TType(const String& name, int size):
		Type(name, size)
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
		ObjectType(name, size, castFromFunc, castToFunc)
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
		PointerType(unptrType)
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
		PropertyType((String)(typeid(_property_type).name()) + (String)"<" + TypeOf(_value_type).GetName() + ">", sizeof(_value_type))
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
	void* TPropertyType<_value_type, _property_type>::NullFieldGetter::GetFieldPtr(const TPropertyType<_value_type, _property_type>& type, void* object, const String& path, FieldInfo*& fieldInfo)
	{
		return nullptr;
	}

	template<typename _value_type, typename _property_type>
	void* TPropertyType<_value_type, _property_type>::RealFieldGetter::GetFieldPtr(const TPropertyType<_value_type, _property_type>& type, void* object, const String& path, FieldInfo*& fieldInfo)
	{
		_property_type* prop = (_property_type*)object;
		_value_type valuePtr = prop->Get();
		return type.mValueType->GetFieldPtr(&valuePtr, path, fieldInfo);
	}

	template<typename _value_type, typename _property_type>
	void* TPropertyType<_value_type, _property_type>::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		typedef typename std::conditional<std::is_pointer<_value_type>::value, RealFieldGetter, NullFieldGetter>::type getter;

		return getter::GetFieldPtr(*this, object, path, fieldInfo);
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
		VectorType((String)"o2::Vector<" + GetTypeOf<_element_type>().GetName() + ">", sizeof(Vector<_element_type>))
	{
		mElementType = &GetTypeOf<_element_type>();

		typedef typename std::conditional<DataNode::IsSupport<_element_type>::value,
			                              FieldInfo::FieldSerializer<_element_type>,
			                              IFieldSerializer>::type serializerType;

		mElementFieldInfo = mnew FieldInfo("element", 0, mElementType, ProtectSection::Private, mnew serializerType());
		mCountFieldInfo = mnew FieldInfo("count", 0, &GetTypeOf<int>(), ProtectSection::Public, mnew VectorCountFieldSerializer<_element_type>());
	}

	template<typename _element_type>
	const Type* TVectorType<_element_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<Vector<_element_type>>(this);

		return mPtrType;
	}

	template<typename _element_type>
	void VectorCountFieldSerializer<_element_type>::Serialize(void* object, DataNode& data) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());

		int size = type.GetObjectVectorSize(object);
		data["Size"].SetValue(size);

		DataNode& elements = data["Elements"];
		for (int i = 0; i < size; i++)
		{
			void* elementPtr = type.GetObjectVectorElementPtr(object, i);
			type.mElementFieldInfo->Serialize(elementPtr, *elements.AddNode("Element" + (String)i));
		}
	}

	template<typename _element_type>
	void VectorCountFieldSerializer<_element_type>::Deserialize(void* object, DataNode& data) const
	{
		const VectorType& type = (const VectorType&)(GetTypeOf<Vector<_element_type>>());
		int size = type.GetObjectVectorSize(object);
		int newSize;
		data["Size"].GetValue(newSize);
		type.SetObjectVectorSize(object, newSize);

		if (auto elementsData = data.GetNode("Elements"))
		{
			for (int i = size; i < newSize; i++)
			{
				if (auto elementData = elementsData->GetNode("Element" + (WString)i))
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
	IFieldSerializer* VectorCountFieldSerializer<_element_type>::Clone() const
	{
		return mnew VectorCountFieldSerializer();
	}

	// ------------------------------
	// TDictionaryType implementation
	// ----------------------------- -

	template<typename _key_type, typename _value_type>
	TDictionaryType<_key_type, _value_type>::TDictionaryType():
		DictionaryType(&GetTypeOf<_key_type>(), &GetTypeOf<_value_type>(), sizeof(Dictionary<_key_type, _value_type>))
	{
		mGetDictionaryObjectSizeFunc = [](void* obj) { return ((Dictionary<_key_type, _value_type>*)obj)->Count(); };
		mSetDictionaryObjectSizeFunc = [](void* obj, int size) { ((Dictionary<_key_type, _value_type>*)obj)->Resize(size); };

		mGetObjectDictionaryKeyPtrFunc = [](void* obj, int idx) {
			return &(((Dictionary<_key_type, _value_type>*)obj)->Begin() + idx).Key();
		};

		mGetObjectDictionaryValuePtrFunc = [](void* obj, int idx) {
			return &(((Dictionary<_key_type, _value_type>*)obj)->Begin() + idx).Value();
		};
	}

	template<typename _key_type, typename _value_type>
	void* TDictionaryType<_key_type, _value_type>::CreateSample() const
	{
		return mnew Dictionary<_key_type, _value_type>();
	}

	template<typename _key_type, typename _value_type>
	IAbstractValueProxy* TDictionaryType<_key_type, _value_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<Dictionary<_key_type, _value_type>>((Dictionary<_key_type, _value_type>*)object);
	}

	template<typename _key_type, typename _value_type>
	const Type* TDictionaryType<_key_type, _value_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<Dictionary<_key_type, _value_type>>(this);

		return mPtrType;
	}

	// ----------------------------------------
	// StringPointerAccessorType implementation
	// ----------------------------------------

	template<typename _return_type, typename _accessor_type>
	StringPointerAccessorType<_return_type, _accessor_type>::StringPointerAccessorType():
		Type((String)(typeid(_accessor_type).name()) + (String)"<" + TypeOf(_return_type).GetName() + ">",
			 sizeof(_accessor_type))
	{
		mReturnType = &GetTypeOf<_return_type>();
	}

	template<typename _return_type, typename _accessor_type>
	Type::Usage StringPointerAccessorType<_return_type, _accessor_type>::GetUsage() const
	{
		return Usage::StringAccessor;
	}

	template<typename _return_type, typename _accessor_type>
	void* StringPointerAccessorType<_return_type, _accessor_type>::CreateSample() const
	{
		return nullptr;
	}

	template<typename _return_type, typename _accessor_type>
	IAbstractValueProxy* StringPointerAccessorType<_return_type, _accessor_type>::GetValueProxy(void* object) const
	{
		return mnew PointerValueProxy<_accessor_type>((_accessor_type*)object);
	}

	template<typename _return_type, typename _accessor_type>
	void* StringPointerAccessorType<_return_type, _accessor_type>::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		_accessor_type* accessor = (_accessor_type*)object;

		auto allFromAccessor = accessor->GetAll();
		for (auto kv : allFromAccessor)
		{
			if (kv.Key() == pathPart)
			{
				_return_type value = kv.Value();
				return mReturnType->GetFieldPtr(&value, path.SubStr(delPos + 1), fieldInfo);
			}
		}

		return nullptr;
	}

	template<typename _return_type, typename _accessor_type>
	const Type* StringPointerAccessorType<_return_type, _accessor_type>::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType<_accessor_type>(this);

		return mPtrType;
	}

	// ------------------------
	// TEnumType implementation
	// ------------------------

	template<typename _type>
	TEnumType<_type>::TEnumType(const String& name, int size):
		EnumType(name, size)
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

		typedef typename std::conditional<DataNode::IsSupport<_type>::value,
			FieldInfo::FieldSerializer<_type>,
			IFieldSerializer>::type serializerType;

		auto fieldInfo = mnew FieldInfo(name, pointerGetter, valType, section, mnew serializerType());
		fieldInfo->mOwnerType = type;
		type->mFields.Add(fieldInfo);

		return *fieldInfo;
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
	FUNDAMENTAL_META(DataNode) END_META;
	FUNDAMENTAL_META(UID) END_META;
}
