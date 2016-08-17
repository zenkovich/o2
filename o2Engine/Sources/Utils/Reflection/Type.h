#pragma once

#include "Utils/Math/Basis.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Reflection/FunctionInfo.h"
#include "Utils/String.h"

#define TypeOf(TYPE) GetTypeOf<TYPE>()

namespace o2
{
	class IObject; 

	// -----------------------
	// Object type information
	// -----------------------
	class Type
	{
	public:
		typedef UInt Id;
		typedef Vector<FieldInfo*> FieldInfosVec;
		typedef Vector<FunctionInfo*> FunctionsInfosVec;
		typedef Vector<Type*> TypesVec;

		struct ISampleCreator;

	public:
		// Default constructor
		Type(const String& name, ISampleCreator* creator, int size);

		// Destructor
		~Type();

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

		// Returns vector of base types
		const TypesVec& BaseTypes() const;

		// Returns fields informations array
		const FieldInfosVec& Fields() const;

		// Returns functions informations array
		const FunctionsInfosVec& Functions() const;

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
		template<typename _type>
		_type* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		// Returns field path by pointer from source object
		String GetFieldPath(void* sourceObject, void *targetObject, FieldInfo*& fieldInfo) const;

	public:
		// --------------------
		// Dummy type container
		// --------------------
		struct Dummy
		{
			static Type* type;
		};

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
		String            mName;          // Name of object type
		FieldInfosVec     mFields;        // Fields information
		FunctionsInfosVec mFunctions;     // Functions informations
		Id                mId;            // Id of type
		TypesVec          mBaseTypes;     // Base types ids
		ISampleCreator*   mSampleCreator; // Template type agent
		int               mPointer;       // Amount of pointers of type
		mutable Type*     mPtrType;       // Pointer type from this
		mutable Type*     mUnptrType;     // Unpoint type from this
		int               mSize;          // Size of type in bytes

	protected:
		// Sets name for this and pointer/unpoint types
		void SetName(const String& name);

		friend class FieldInfo;
		friend class FunctionInfo;
		friend class Reflection;
		friend class TypeInitializer;

		template<typename _type>
		friend class AccessorFieldInfo;
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

	template<typename T, typename X = 	
		    std::conditional<std::is_base_of<IObject, T>::value, 
		        T,
		    //else
		        std::conditional<(std::is_fundamental<T>::value ||
		                         std::is_same<T, Basis>::value ||
		                         std::is_same<T, Color4>::value ||
		                         std::is_same<T, RectI>::value ||
		                         std::is_same<T, RectF>::value ||
		                         std::is_same<T, Vec2I>::value ||
		                         std::is_same<T, Vec2F>::value ||
		                         std::is_same<T, Vertex2>::value ||
		                         std::is_same<T, String>::value ||
		                         std::is_same<T, WString>::value ||
		                         std::is_same<T, DataNode>::value) && !std::is_const<T>::value,
		            FundamentalType<T>,
		        //else
		            Type::Dummy
		        >::type
			>::type>
	struct TypeDeductor
	{
		typedef X type;
	};

	template<typename _type>
	const Type& GetTypeOf()
	{
		if (std::is_pointer<_type>::value)
			return *GetTypeOf<std::remove_pointer<_type>::type>().GetPointerType();


		return *std::conditional<std::is_pointer<_type>::value, 
			        TypeDeductor<std::remove_pointer<_type>::type>::type,
			    //else
			        TypeDeductor<_type>::type
		        >::type::type;
	}

	// ----------------
	// Type initializer
	// ----------------
	class TypeInitializer
	{
	public:
		// Adds basic type
		template<typename _type, typename _baseType>
		static void AddBaseType();

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, _type*& value, ProtectSection section);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, _type& value, ProtectSection section);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, Property<_type>& value, ProtectSection section);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, Accessor<_type*, const String&>& value, ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...), ProtectSection section);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const, ProtectSection section);

		// Registers parameter in function info
		template<typename _type>
		static FunctionInfo* RegFuncParam(FunctionInfo* info, const String& name);
	}; 
	

	template<typename _type, typename _baseType>
	void TypeInitializer::AddBaseType()
	{
		_type::type.mBaseTypes.Add(&_baseType::type);
		_type::type.mFields.Insert(_baseType::type.mFields.Select<FieldInfo*>([](FieldInfo* x) { return x->Clone(); }), 0);
		_type::type.mFunctions.Insert(_baseType::type.mFunctions.Select<FunctionInfo*>([](FunctionInfo* x) { return x->Clone(); }), 0);
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type*& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type*);
		type->mFields.Add(new FieldInfo(name, offset, false, true, valType, section,
										new FieldInfo::FieldSerializer<_type*>()));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type);
		type->mFields.Add(new FieldInfo(name, offset, false, false, valType, section, 
										new FieldInfo::FieldSerializer<_type>()));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, Property<_type>& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type);
		type->mFields.Add(new FieldInfo(name, offset, true, false, valType, section,
										new FieldInfo::FieldSerializer<Property<_type>>()));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, Accessor<_type*, const String&>& value, ProtectSection section)
	{
		auto valType = &TypeOf(_type);
		type->mFields.Add(new AccessorFieldInfo<_type>(name, offset, valType, section,
													   new FieldInfo::FieldSerializer<Accessor<_type*, const String&>>()));
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
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}

	template<typename _type>
	FunctionInfo* TypeInitializer::RegFuncParam(FunctionInfo* info, const String& name)
	{
		auto valType = &TypeOf(_type);

		FunctionInfo::Parameter param;
		param.type = valType;
		param.name = name;
		param.isPointer = std::is_pointer<_type>::value;
		param.isConstant = std::is_const<_type>::value;
		param.isReference = std::is_reference<_type>::value;
		info->mParameters.Add(param);

		return info;
	}

	template<typename _type>
	_type* Type::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto field : mFields)
		{
			if (field->mName == pathPart)
			{
				if (delPos == -1)
				{
					fieldInfo = field;
					return field->GetValuePtr<_type>(object);
				}
				else
				{
					char* val = field->GetValuePtr<char>(object);

					if (!val)
						return nullptr;

					return (_type*)(field->SearchFieldPtr(val, path.SubStr(delPos + 1), fieldInfo));
				}
			}
		}

		return nullptr;
	}

	template<typename _res_type, typename ... _args>
	_res_type Type::Invoke(const String& name, void* object, _args ... args)
	{
		FunctionInfo* func = GetFunction(name);
		if (func)
			return func->Invoke(object, args ...);

		return _res_type();
	}
}
