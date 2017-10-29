#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Delegates.h"
#include "Utils/CommonTypes.h"
#include "Utils/StringDef.h"

#define TypeOf(TYPE) GetTypeOf<TYPE>()

namespace o2
{
	class IObject;
	class FieldInfo;
	class FunctionInfo;
	class Type;

	template<typename _key_type, typename _value_type>
	class Dictionary;

	template<typename _type, typename _getter>
	const Type& GetTypeOf();

	template<typename T, typename X>
	struct GetTypeHelper;

	template<typename _element_type>
	struct VectorCountFieldSerializer;

	typedef UInt TypeId;

	// -----------------------------
	// Type sample creator interface
	// -----------------------------
	struct ITypeSampleCreator
	{
		// Returns new sample of type
		virtual void* CreateSample() const = 0;
	};

	// -------------------------------
	// Specialized type sample creator
	// -------------------------------
	template<typename _type>
	struct TypeSampleCreator: public ITypeSampleCreator
	{
		// Returns new sample of type
		void* CreateSample() const { return mnew _type(); }
	};

	// -----------
	// Object type
	// -----------
	class Type
	{
	public:
		enum class Usage
		{
			Regular, Vector, Dictionary, StringAccessor, Enumeration, Pointer, Property
		};

		typedef Vector<FieldInfo*> FieldInfosVec;
		typedef Vector<FunctionInfo*> FunctionsInfosVec;
		typedef Vector<Type*> TypesVec;
		typedef Dictionary<Type*, int> BaseTypesVec;

	public:
		// Default constructor
		Type(const String& name, ITypeSampleCreator* creator, int size);

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
		const Type* GetPointerType() const;

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
		const FieldInfo* GetField(const String& name) const;

		// Returns function info by name
		const FunctionInfo* GetFunction(const String& name) const;

		// Invokes function with name
		template<typename _res_type, typename ... _args>
		_res_type Invoke(const String& name, void* object, _args ... args);

		// Returns derived types
		Vector<const Type*> GetDerivedTypes() const;

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

	protected:
		TypeId              mId;            // Id of type
		String              mName;          // Name of object type
		BaseTypesVec        mBaseTypes;     // Base types ids with offset 
		FieldInfosVec       mFields;        // Fields information
		FunctionsInfosVec   mFunctions;     // Functions informations
		ITypeSampleCreator* mSampleCreator; // Template type agent
		mutable Type*       mPtrType;       // Pointer type from this
		int                 mSize;          // Size of type in bytes

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

	// ----------------
	// Fundamental type
	// ----------------
	template<typename _type>
	class FundamentalType: public Type
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
		Usage GetUsage() const;

		// Returns unpointed type
		const Type* GetUnpointedType() const;

	protected:
		const Type* mUnptrType;
	};

	// ---------------
	// Property<> type
	// ---------------
	class PropertyType: public Type
	{
	public:
		// Default constructor
		PropertyType(const String& name, ITypeSampleCreator* creator, int size);

		// Returns type usage
		Usage GetUsage() const;

		// Returns type of value
		const Type* GetValueType() const;

	protected:
		const Type* mValueType;
	};

	// ---------------------------
	// Specialized Property<> type
	// ---------------------------
	template<typename _value_type>
	class TPropertyType: public PropertyType
	{
	public:
		// Constructor
		TPropertyType();
	};

	// ----------------------
	// Type of Vector<> value
	// ----------------------
	class VectorType: public Type
	{
	public:
		// Default constructor
		VectorType(const String& name, ITypeSampleCreator* creator, int size);

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

	protected:
		// Searches field recursively by pointer
		virtual FieldInfo* SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const; 
		
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

	// ----------------
	// Enumeration type
	// ----------------
	class EnumType: public Type
	{
	public:
		// Constructor
		EnumType(const String& name, ITypeSampleCreator* creator, int size);

		// Returns type usage
		Usage GetUsage() const;

		// Returns enum entries
		const Dictionary<int, String>& GetEntries() const;

	protected:
		Dictionary<int, String> mEntries;

		friend class Reflection;
	};

	// --------------------------
	// Fundamental type container
	// --------------------------
	template<typename _type>
	class FundamentalTypeContainer
	{
	protected:
		static Type* type;

		template<typename _type, typename _getter>
		friend const Type& o2::GetTypeOf();

		template<typename T>
		friend struct RegularTypeGetter;

		template<typename T, typename X>
		friend struct o2::GetTypeHelper;

		friend class Reflection;

	public:
		template<typedef _processor_type>
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

		template<typename _type, typename _getter>
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
		template<typename _type, typename X = std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type>
		static void AddBaseType(Type*& type, int offset);

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
}

class ITypeProcessor
{
public:
	template<typename _object_type>
	void Start(_object_type* object) {}

	template<typename _object_type>
	void StartBases(_object_type* object) {}

	template<typename _object_type>
	void StartFields(_object_type* object) {}

	template<typename _object_type>
	void StartMethods(_object_type* object) {}

	template<typename _object_type, typename _base_type>
	void BaseType(_object_type* object, Type* type, const char* name) {}

	template<typename _object_type, typename _field_type>
	void Field(_object_type* object, Type* type, const char* name, _field_type& field, ProtectSection protection) {}

	template<typename _object_type, typename _res_type, typename ... _args>
	void Method(_object_type* object, Type* type, const char* name, _res_type(_class_type::*pointer)(_args ...), ProtectSection protection) {}

	template<typename _object_type, typename _res_type, typename ... _args>
	void Method(_object_type* object, Type* type, const char* name, _res_type(_class_type::*pointer)(_args ...) const, ProtectSection protection) {}
};

#define DECLARE_CLASS(CLASS)                                                                                   \
    o2::Type* CLASS::type = o2::Reflection::InitializeType<CLASS>(#CLASS)

#define CLASS_BASES_META(CLASS)                                                                                \
    template<typename _type_processor> void CLASS::ProcessBaseTypes(CLASS* object, _type_processor& processor) \
	{                                                                                                          \
		processor.StartBases<CLASS>(object, type);

#define CLASS_FIELDS_META(CLASS)                                                                               \
    template<typename _type_processor> void CLASS::ProcessFields(CLASS* object, _type_processor& processor)    \
	{                                                                                                          \
		processor.StartFields<CLASS>(object, type);															   

#define CLASS_METHODS_META(CLASS)                                                                              \
    template<typename _type_processor> void CLASS::ProcessMethods(CLASS* object, _type_processor& processor)   \
	{                                                                                                          \
		processor.StartMethods<CLASS>(object, type);
	 
#define META_TEMPLATES(...) \
    template<__VA_ARGS__>

#define FUNDAMENTAL_META(NAME)                                                                                                       \
    template<typename _type_processor> void FundamentalTypeContainer<NAME>::InitializeType(NAME* object, _type_processor& processor) \
	{                                                                                                                                \
		processor.StartFields<NAME>(object, type);

#define BASE_CLASS(CLASS) \
    processor.BaseType<thisclass, CLASS>(object, type, #CLASS)

#define FIELD(NAME, PROTECT_SECTION) \
    processor.Field<thisclass, decltype<object->NAME>>(object, type, #NAME, object->NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FIELD(NAME) \
    processor.Field<thisclass, decltype<object->NAME>>(object, type, #NAME, object->NAME, ProtectSection::Public)

#define PRIVATE_FIELD(NAME) \
    processor.Field<thisclass, decltype<object->NAME>>(object, type, #NAME, object->NAME, ProtectSection::Private)

#define PROTECTED_FIELD(NAME) \
    processor.Field<thisclass, decltype<object->NAME>>(object, type, #NAME, object->NAME, ProtectSection::Protected)


#define ATTRIBUTE(NAME) \
    AddAttribute(new NAME)

#define ATTRIBUTES(...)

#define ATTRIBUTE_COMMENT_DEFINITION(X)

#define ATTRIBUTE_SHORT_DEFINITION(X)

#define FUNCTION(PROTECT_SECTION, RETURN_TYPE, NAME, ...) \
    processor.Method<thisclass, RETURN_TYPE, __VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::PROTECT_SECTION)

#define PUBLIC_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.Method<thisclass, RETURN_TYPE, __VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Public)

#define PRIVATE_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.Method<thisclass, RETURN_TYPE, __VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Private)

#define PROTECTED_FUNCTION(RETURN_TYPE, NAME, ...) \
    processor.Method<thisclass, RETURN_TYPE, __VA_ARGS__>(object, type, #NAME, &thisclass::NAME, ProtectSection::Protected)

#define END_META }

#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Reflection/FunctionInfo.h"
#include "Utils/Reflection/TypeTraits.h"
#include "Utils/StringImpl.h"
#include "Utils/UID.h"

namespace o2
{
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

	// -----------------------------
	// FundamentalType implementation
	// -----------------------------

	template<typename _type>
	FundamentalType<_type>::FundamentalType(const String& name):
		Type(name, new TypeSampleCreator<_type>(), sizeof(_type))
	{}

	template<typename _type>
	void FundamentalType<_type>::InitializeType(Type* type)
	{}

	// ------------------------------
	// TPropertyType<> implementation
	// ------------------------------

	template<typename _value_type>
	TPropertyType<_value_type>::TPropertyType():
		PropertyType((String)"o2::Property<" + GetTypeOf<_value_type>().GetName() + ">", new TypeSampleCreator<_value_type>(),
					 sizeof(_value_type))
	{
		mValueType = &GetTypeOf<_value_type>();
	}

	// --------------------------
	// TVectorType implementation
	// --------------------------

	template<typename _element_type>
	struct VectorCountFieldSerializer: public FieldInfo::IFieldSerializer
	{
		VectorCountFieldSerializer() {}
		void Serialize(void* object, DataNode& data) const;
		void Deserialize(void* object, DataNode& data) const;
		bool Equals(void* objectA, void* objectB) const;
		void Copy(void* objectA, void* objectB) const;
		FieldInfo::IFieldSerializer* Clone() const;
	};

	template<typename _element_type>
	void* TVectorType<_element_type>::GetObjectVectorElementPtr(void* object, int idx) const
	{
		return &((Vector<_element_type>*)object)->Get(idx);
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
	int TVectorType<_element_type>::GetObjectVectorSize(void* object) const
	{
		return ((Vector<_element_type>*)object)->Count();
	}

	template<typename _element_type>
	TVectorType<_element_type>::TVectorType():
		VectorType((String)"o2::Vector<" + GetTypeOf<_element_type>().GetName() + ">", new TypeSampleCreator<Vector<_element_type>>(),
				   sizeof(Vector<_element_type>))
	{
		mElementType = &GetTypeOf<_element_type>();

		typedef std::conditional<DataNode::IsSupport<_element_type>::value,
			FieldInfo::FieldSerializer<_element_type>,
			FieldInfo::IFieldSerializer>::type serializerType;

		mElementFieldInfo = new FieldInfo("element", 0, mElementType, ProtectSection::Private, new serializerType());
		mCountFieldInfo = new FieldInfo("count", 0, &GetTypeOf<int>(), ProtectSection::Public, new VectorCountFieldSerializer<_element_type>());
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
	FieldInfo::IFieldSerializer* VectorCountFieldSerializer<_element_type>::Clone() const
	{
		return mnew VectorCountFieldSerializer();
	}

	// -----------------------------
	// DictionaryType implementation
	// ----------------------------- 

	template<typename _key_type, typename _value_type>
	DictionaryType::DictionaryType(_key_type* x, _value_type* y):
		Type((String)"o2::Dictionary<" + GetTypeOf<_key_type>().GetName() + ", " + GetTypeOf<_value_type>().GetName() + ">",
			 new TypeSampleCreator<Dictionary<_key_type, _value_type>>, sizeof(Dictionary<_key_type, _value_type>))
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
		Type((String)"Accessor<" + GetTypeOf<_return_type>().GetName() + "*, const o2::String&>",
			 new TypeSampleCreator<Accessor<_return_type*, const String&>>(),
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
		auto allFields = mReturnType->GetFieldsWithBaseClasses();
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
	void TypeInitializer::AddBaseType(Type*& type, int offset)
	{
		if (std::is_same<X, Type::Dummy>::value)
			return;

		Type*& baseType = X::type;

		type->mBaseTypes.Add(baseType, offset);
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type);

		typedef std::conditional<DataNode::IsSupport<_type>::value,
			FieldInfo::FieldSerializer<_type>,
			FieldInfo::IFieldSerializer>::type serializerType;

		type->mFields.Add(new FieldInfo(name, offset, valType, section, new serializerType()));
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
