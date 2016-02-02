#pragma once

#include "Utils/Property.h"
#include "Utils/Reflection/FieldInfo.h"
#include "Utils/Reflection/FunctionInfo.h"
#include "Utils/String.h"

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

		struct Dummy
		{
			static Type type;
		};

		struct ITypeCreator
		{
			virtual IObject* Create() const = 0;
		};

		template<typename _type>
		struct TypeCreator: public ITypeCreator
		{
			IObject* Create() const { return mnew _type(); }
		};

	public:
		// Default constructor
		Type();

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
		TypesVec DerivedTypes() const;

		// Creates sample copy and returns him
		IObject* CreateSample() const;

		// Returns filed pointer by path
		template<typename _type>
		_type* GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const;

		// Returns field path by pointer from source object
		String GetFieldPath(void* sourceObject, void *targetObject, FieldInfo*& fieldInfo) const;

	protected:
		String            mName;        // Name of object type
		FieldInfosVec     mFields;      // Fields information
		FunctionsInfosVec mFunctions;   // Functions informations
		Id                mId;          // Id of type
		TypesVec          mBaseTypes;   // Base types ids
		ITypeCreator*     mTypeCreator; // Type creator

		friend class FieldInfo;
		friend class FunctionInfo;
		friend class Reflection;
		friend class TypeInitializer;

		template<typename _type>
		friend class AccessorFieldInfo;
	};


	class TypeInitializer
	{
	public:
		// Adds basic type
		static void AddBaseType(Type* type, Type* baseType);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, _type*& value);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, _type& value);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, Property<_type>& value);

		// Registers field in type
		template<typename _type>
		static FieldInfo& RegField(Type* type, const String& name, UInt offset, Accessor<_type*, const String&>& value);

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...));

		// Registers function in type
		template<typename _class_type, typename _res_type, typename ... _args>
		static FunctionInfo* RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const);

		// Registers parameter in function info
		template<typename _type>
		static FunctionInfo* RegFuncParam(FunctionInfo* info, const String& name);
	};

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type*& value)
	{
		Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
		type->mFields.Add(new FieldInfo(name, offset, false, true, valType));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, _type& value)
	{
		Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
		type->mFields.Add(new FieldInfo(name, offset, false, false, valType));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, Property<_type>& value)
	{
		Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
		type->mFields.Add(new FieldInfo(name, offset, true, false, valType));
		return *type->mFields.Last();
	}

	template<typename _type>
	FieldInfo& TypeInitializer::RegField(Type* type, const String& name, UInt offset, Accessor<_type*, const String&>& value)
	{
		Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
		type->mFields.Add(new AccessorFieldInfo<_type>(name, offset, valType));
		return *type->mFields.Last();
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...))
	{
		Type* retType = &std::conditional<std::is_base_of<IObject, _res_type>::value, _res_type, Type::Dummy>::type::type;

		auto funcInfo = new SpecFunctionInfo<_class_type, _res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mIsContant = false;
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}

	template<typename _class_type, typename _res_type, typename ... _args>
	FunctionInfo* TypeInitializer::RegFunction(Type* type, const String& name, _res_type(_class_type::*pointer)(_args ...) const)
	{
		Type* retType = &std::conditional<std::is_base_of<IObject, _res_type>::value, _res_type, Type::Dummy>::type::type;

		auto funcInfo = new SpecConstFunctionInfo<_class_type, _res_type, _args ...>();
		funcInfo->mName = name;
		funcInfo->mFunctionPtr = pointer;
		funcInfo->mReturnType = retType;
		funcInfo->mIsContant = true;
		type->mFunctions.Add(funcInfo);

		return funcInfo;
	}

	template<typename _type>
	FunctionInfo* TypeInitializer::RegFuncParam(FunctionInfo* info, const String& name)
	{
		Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;

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
