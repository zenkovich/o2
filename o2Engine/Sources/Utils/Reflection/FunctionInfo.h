#pragma once

#include "Utils/Types/CommonTypes.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/String.h"

namespace o2
{
	class Type;

	// --------------------
	// Function information
	// --------------------
	class FunctionInfo
	{
	public:
		struct Parameter
		{
			String      name;
			const Type* type;
			bool        isConstant;
			bool        isReference;
			bool        isPointer;

			bool operator==(const Parameter& other) const;
		};
		typedef Vector<Parameter> ParametersVec;

	public:
		// Returns cloned copy
		virtual FunctionInfo* Clone() const;

		// Check Equals operator
		bool operator==(const FunctionInfo& other) const;

		// Returns owner type
		Type* GetOwnerType() const;

		// Returns function's name
		const String& GetName() const;

		// Returns return value type
		const Type* GetReturnType() const;

		// Returns is function constant
		bool IsConstant() const;

		// Returns function's parameters
		const ParametersVec& GetParameters() const;

		// Returns protection section of function
		ProtectSection GetProtectionSection() const;

		// Invokes function with parameters
		template<typename _res_type, typename ... _args>
		_res_type Invoke(void* object, _args ... args) const;

	protected:
		ProtectSection mProtectSection; // Protection section
		Type*          mOwnerType;      // Owner type pointer
		String         mName;           // Name of function
		const Type*    mReturnType;     // Function returning type
		bool           mIsContant;      // Is function contant
		ParametersVec  mParameters;     // Function parameters list

		friend class Type;
		friend class TypeInitializer;
	};	

	// ----------------------------------
	// Function info specialization layer
	// ----------------------------------
	template<typename _res_type, typename ... _args>
	class ISpecFunctionInfo: public FunctionInfo
	{
	public:
		virtual _res_type Invoke(void* object, _args ... args) const = 0;
	};

	// -------------------------------
	// Function specialized class info
	// -------------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class SpecFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
	{
	public:
		// Returns cloned copy
		FunctionInfo* Clone() const
		{
			return mnew SpecFunctionInfo(*this);
		}

		// Invokes function with parameters
		_res_type Invoke(void* object, _args ... args) const
		{
			_class_type* classObject = (_class_type*)object;
			return (classObject->*mFunctionPtr)(args ...);
		}

	protected:
		_res_type(_class_type::*mFunctionPtr)(_args ... args);

		friend class TypeInitializer;
	};

	// ----------------------------------------
	// Constant function specialized class info
	// ----------------------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class SpecConstFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
	{
	public:
		// Returns cloned copy
		FunctionInfo* Clone() const
		{
			return mnew SpecConstFunctionInfo(*this);
		}

		// Invokes function with parameters
		_res_type Invoke(void* object, _args ... args) const
		{
			_class_type* classObject = (_class_type*)object;
			return (classObject->*mFunctionPtr)(args ...);
		}

	protected:
		_res_type(_class_type::*mFunctionPtr)(_args ... args) const;

		friend class TypeInitializer;
	};
	
	template<typename _res_type, typename ... _args>
	_res_type FunctionInfo::Invoke(void* object, _args ... args) const
	{
		const ISpecFunctionInfo<_res_type, _args ...>* thisSpec = (const ISpecFunctionInfo<_res_type, _args ...>*)this;
		return thisSpec->Invoke(object, args ...);
	}
}
