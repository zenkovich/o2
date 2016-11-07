#pragma once

#include "Utils/Delegates.h"
#include "Utils/Math/Basis.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Reflection/FunctionInfo.h"
#include "Utils/Reflection/TypeTraits.h"
#include "Utils/String.h"
#include "Utils/UID.h"

#define TypeOf(TYPE) GetTypeOf<TYPE>()

namespace o2
{
	class IObject;

	// -----------
	// Object type
	// -----------
	class Type
	{
	public:
		enum class Usage
		{
			Regular, Vector, Dictionary, StringAccessor
		};

		typedef UInt Id;
		typedef Vector<FieldInfo*> FieldInfosVec;
		typedef Vector<FunctionInfo*> FunctionsInfosVec;
		typedef Vector<Type*> TypesVec;

		struct ISampleCreator;

	public:
		// Default constructor
		Type(const String& name, ISampleCreator* creator, int size);

		// Destructor
		virtual ~Type();

		// Check equals operator
		bool operator==(const Type& other) const;

		// Check not equals operator
		bool operator!=(const Type& other) const;

		// Returns name of type
		const String& Name() const;

		// Returns id of type
		Id ID() const;

		// Returns size of type in bytes
		int Size() const;

		// Is this type based on other
		bool IsBasedOn(const Type& other) const;

		// Returns pointer of type (type -> type*)
		const Type* GetPointerType() const;

		// Returns type with removed pointer (type* -> type)
		const Type* GetUnpointedType() const;

		// Is type pointer
		bool IsPointer() const;

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns vector of base types
		const TypesVec& BaseTypes() const;

		// Returns fields informations array
		const FieldInfosVec& Fields() const;

		// Returns fields informations array with all base types
		FieldInfosVec AllFields() const;

		// Returns functions informations array
		const FunctionsInfosVec& Functions() const;

		// Returns functions informations array with all base types
		FunctionsInfosVec AllFunctions() const;

		// Returns field information by name
		const FieldInfo* Field(const String& name) const;

		// Returns function info by name
		const FunctionInfo* GetFunction(const String& name) const;

		// Invokes function with name
		template<typename _res_type, typename ... _args>
		_res_type Invoke(const String& name, void* object, _args ... args);

		// Returns inherited types
		Vector<const Type*> DerivedTypes() const;

		// Creates sample copy and returns him
		void* CreateSample() const;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		// Returns field path by pointer from source object
		String GetFieldPath(void* sourceObject, void *targetObject, FieldInfo*& fieldInfo) const;

	public:
		// --------------------
		// Dummy type container
		// --------------------
		struct Dummy { static Type* type; };

		// -----------------------------
		// Type sample creator interface
		// -----------------------------
		struct ISampleCreator
		{
			// Returns new sample of type
			virtual void* CreateSample() const = 0;
		};

		// -------------------------------
		// Specialized type sample creator
		// -------------------------------
		template<typename _type>
		struct SampleCreator: public ISampleCreator
		{
			// Returns new sample of type
			void* CreateSample() const { return mnew _type(); }
		};

	protected:
		Id                mId;            // Id of type
		String            mName;          // Name of object type
		TypesVec          mBaseTypes;     // Base types ids
		FieldInfosVec     mFields;        // Fields information
		FunctionsInfosVec mFunctions;     // Functions informations
		ISampleCreator*   mSampleCreator; // Template type agent
		int               mPointer;       // Amount of pointers of type
		mutable Type*     mPtrType;       // Pointer type from this
		mutable Type*     mUnptrType;     // Unpoint type from this
		int               mSize;          // Size of type in bytes

		void(*mInitializeFunc)(Type*);    // Type initializing function

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const;

		friend class FieldInfo;
		friend class FunctionInfo;
		friend class Reflection;
		friend class TypeInitializer;
		friend class VectorType;

		template<typename _type>
		friend class StringPointerAccessorType;
	};

	// ----------------------
	// Type of Vector<> value
	// ----------------------
	class VectorType: public Type
	{
	public:
		// Default constructor
		template<typename _element_type>
		VectorType(_element_type* x);

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns type of vector element
		const Type* GetElementType() const;

		// Returns size of vector by pointer
		int GetObjectVectorSize(void* object) const;

		// Sets size of vector by pointer
		void SetObjectVectorSize(void* object, int size) const;

		// Returns element's pointer by index
		void* GetObjectVectorElementPtr(void* object, int idx) const;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

	protected:
		const Type* mElementType;

		SharedLambda<int(void*)>        mGetVectorObjectSizeFunc;
		SharedLambda<void(void*, int)>  mSetVectorObjectSizeFunc;
		SharedLambda<void*(void*, int)> mGetObjectVectorElementPtrFunc;

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const;
	};

	// --------------------------
	// Type of Dictionary<> value
	// --------------------------
	class DictionaryType: public Type
	{
	public:
		// Default constructor
		template<typename _key_type, typename _value_type>
		DictionaryType(_key_type* x, _value_type* y);

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

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const;
	};

	// -------------------------------------------
	// Accessor<_return_type*, const String&> type
	// -------------------------------------------
	template<typename _return_type>
	class StringPointerAccessorType: public Type
	{
	public:
		StringPointerAccessorType();

		// Returns type usage
		virtual Usage GetUsage() const;

		// Returns filed pointer by path
		virtual void* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

	protected:
		const Type* mReturnType;

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const;
	};

	// --------------------------
	// Fundamental type container
	// --------------------------
	template<typename _type>
	class FundamentalType
	{
	public:
		static Type* type;
	};

	// ----------------
	// Type initializer
	// ----------------
	class TypeInitializer
	{
	public:
		// Adds basic type
		template<typename _type, typename X = std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type>
		static void AddBaseType(Type*& type);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, _type& value, ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...), ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const, ProtectSection section);
	};

	// -------------------------------
	// Types meta information macroses
	// -------------------------------
	// 
#define CLASS_META(NAME)                                                \
    o2::Type* NAME::type = o2::Reflection::InitializeType<NAME>(#NAME); \
    void NAME::InitializeType(o2::Type* type)                           \
	{                                                                   \
	    typedef NAME thisclass;                                         \
	    thisclass::type = type;                                         \
	    thisclass* __this = 0;      

#define META_TEMPLATES(...) \
    template<__VA_ARGS__>

#define CLASS_TEMPLATE_META(NAME)                                       \
    void NAME::InitializeType(o2::Type* type)                           \
	{                                                                   \
	    typedef NAME thisclass;                                         \
	    thisclass::type = type;                                         \
	    thisclass* __this = 0;                                          

#define BASE_CLASS(NAME) \
    o2::TypeInitializer::AddBaseType<NAME>(type)

#define FIELD(NAME, PROTECT_SECTION) \
    o2::TypeInitializer::RegField(type, #NAME, (size_t)(&__this->NAME), __this->NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FIELD(NAME) \
    o2::TypeInitializer::RegField(type, #NAME, (size_t)(&__this->NAME), __this->NAME, ProtectSection::Public)

#define PRIVATE_FIELD(NAME) \
    o2::TypeInitializer::RegField(type, #NAME, (size_t)(&__this->NAME), __this->NAME, ProtectSection::Private)

#define PROTECTED_FIELD(NAME) \
    o2::TypeInitializer::RegField(type, #NAME, (size_t)(&__this->NAME), __this->NAME, ProtectSection::Protected)

#define ATTRIBUTE(NAME) \
    AddAttribute(new NAME)

#define ATTRIBUTES(...)

#define ATTRIBUTE_COMMENT_DEFINITION(X)

#define ATTRIBUTE_SHORT_DEFINITION(X)

#define FUNCTION(PROTECT_SECTION, RETURN_TYPE, NAME, ...) \
    o2::TypeInitializer::RegFunction<thisclass, RETURN_TYPE, __VA_ARGS__>(type, #NAME, &thisclass::NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    o2::TypeInitializer::RegFunction<thisclass, RETURN_TYPE, __VA_ARGS__>(type, #NAME, &thisclass::NAME, ProtectSection::Public)

#define PRIVATE_FUNCTION(RETURN_TYPE, NAME, ...) \
    o2::TypeInitializer::RegFunction<thisclass, RETURN_TYPE, __VA_ARGS__>(type, #NAME, &thisclass::NAME, ProtectSection::Private)

#define PROTECTED_FUNCTION(RETURN_TYPE, NAME, ...) \
    o2::TypeInitializer::RegFunction<thisclass, RETURN_TYPE, __VA_ARGS__>(type, #NAME, &thisclass::NAME, ProtectSection::Protected)

#define END_META }

	// -------------------
	// Type implementation
	// -------------------

	template<typename _res_type, typename ... _args>
	_res_type Type::Invoke(const String& name, void* object, _args ... args)
	{
		FunctionInfo* func = GetFunction(name);
		if (func)
			return func->Invoke(object, args ...);

		return _res_type();
	}

	// -------------------------
	// VectorType implementation
	// -------------------------

	template<typename _element_type>
	VectorType::VectorType(_element_type* x):
		Type((String)"o2::Vector<" + GetTypeOf<_element_type>().Name() + ">", new SampleCreator<Vector<_element_type>>(),
			 sizeof(Vector<_element_type>))
	{
		mElementType = &GetTypeOf<_element_type>();

		mGetVectorObjectSizeFunc = [](void* obj) { return ((Vector<_element_type>*)obj)->Count(); };

		mSetVectorObjectSizeFunc = [](void* obj, int size) { 
			auto vectorObj = ((Vector<_element_type>*)obj);
			int oldSize = vectorObj->Count();
			vectorObj->Resize(size);

			for (int i = oldSize; i < size; i++)
				(*vectorObj)[i] = _element_type();
		};

		mGetObjectVectorElementPtrFunc = [](void* obj, int idx) { return &((Vector<_element_type>*)obj)->Get(idx); };
	}

	// -----------------------------
	// DictionaryType implementation
	// ----------------------------- 

	template<typename _key_type, typename _value_type>
	DictionaryType::DictionaryType(_key_type* x, _value_type* y):
		Type((String)"o2::Dictionary<" + GetTypeOf<_key_type>().Name() + ", " + GetTypeOf<_value_type>().Name() + ">",
			 new SampleCreator<Dictionary<_key_type, _value_type>>, sizeof(Dictionary<_key_type, _value_type>))
	{
		mKeyType = &GetTypeOf<_key_type>();
		mValueType = &GetTypeOf<_value_type>();

		mGetDictionaryObjectSizeFunc = [](void* obj) { return ((Dictionary<_key_type, _value_type>*)obj)->Count(); };
		mSetDictionaryObjectSizeFunc = [](void* obj, int size) { ((Dictionary<_key_type, _value_type>*)obj)->Resize(size); };

		mGetObjectDictionaryKeyPtrFunc = [](void* obj, int idx) {
			return &(((Dictionary<_key_type, _value_type>*)obj)->Begin() + idx).Key();
		};

		mGetObjectDictionaryValuePtrFunc = [](void* obj, int idx) {
			return &(((Dictionary<_key_type, _value_type>*)obj)->Begin() + idx).Value();
		};
	}

	// ----------------------------------------
	// StringPointerAccessorType implementation
	// ----------------------------------------

	template<typename _return_type>
	StringPointerAccessorType<_return_type>::StringPointerAccessorType():
		Type((String)"Accessor<" + GetTypeOf<_return_type>().Name() + "*, const o2::String&>",
			 new SampleCreator<Accessor<_return_type*, const String&>>(),
			 sizeof(Accessor<_return_type*, const String&>))
	{
		mReturnType = &GetTypeOf<_return_type>();
	}

	template<typename _return_type>
	Type::Usage StringPointerAccessorType<_return_type>::GetUsage() const
	{
		return Usage::StringAccessor;
	}

	template<typename _return_type>
	void* StringPointerAccessorType<_return_type>::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		Accessor<_return_type*, const String&>* accessor = ((Accessor<_return_type*, const String&>*)object);

		auto allFromAccessor = accessor->GetAll();
		for (auto kv : allFromAccessor)
		{
			if (kv.Key() == pathPart)
				return mReturnType->GetFieldPtr(kv.Value(), path.SubStr(delPos + 1), fieldInfo);
		}

		return nullptr;
	}

	template<typename _return_type>
	FieldInfo* StringPointerAccessorType<_return_type>::SearchFieldPath(void* obj, void* target, const String& path, String& res,
																		Vector<void*>& passedObjects) const
	{
		Accessor<_return_type*, const String&>* accessor = ((Accessor<_return_type*, const String&>*)obj);

		auto allFromAccessor = accessor->GetAll();
		auto allFields = mReturnType->AllFields();
		for (auto kv : allFromAccessor)
		{
			for (auto field : allFields)
			{
				void* fieldObj = field->GetValuePtr(kv.Value());

				if (fieldObj == nullptr)
					continue;

				if (passedObjects.Contains(fieldObj))
					continue;

				passedObjects.Add(fieldObj);

				String newPath = path + "/" + kv.Key() + "/" + field->mName;
				if (fieldObj == target)
				{
					res = newPath;
					return field;
				}

				FieldInfo* childField = field->SearchFieldPath(fieldObj, target, newPath, res, passedObjects);
				if (childField)
					return childField;
			}
		}

		return nullptr;
	}

	// ------------------------------
	// TypeInitializer implementation
	// ------------------------------

	template<typename _type, typename X>
	void TypeInitializer::AddBaseType(Type*& type)
	{
		if (std::is_same<X, Type::Dummy>::value)
			return;

		Type*& baseType = X::type;

		type->mBaseTypes.Add(baseType);
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type);
		bool isProperty = IsProperty<_type>::value;
		bool isPointer = std::is_pointer<_type>::value;

		typedef std::conditional<DataNode::IsSupport<_type>::value,
			FieldInfo::FieldSerializer<_type>,
			FieldInfo::IFieldSerializer>::type serializerType;

		type->mFields.Add(new FieldInfo(name, offset, isProperty, isPointer, valType, section, new serializerType()));
		return *type->mFields.Last();
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...), ProtectSection section)
	{
		auto retType = &TypeOf(_res_type);

		auto funcInfo = new SpecFunctionInfo<_class_type, _res_type, _args ...>();
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
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const, ProtectSection section)
	{
		auto retType = &TypeOf(_res_type);

		auto funcInfo = new SpecConstFunctionInfo<_class_type, _res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mIsContant = true;
		funcInfo->mProtectSection = section;
		funcInfo->mOwnerType = type;
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}
	}
