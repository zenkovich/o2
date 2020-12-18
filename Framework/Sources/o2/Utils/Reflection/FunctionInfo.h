#pragma once

#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Type;
	// -------------------------
	// Base function information
	// -------------------------
	class FunctionInfoBase
	{
	public:
		struct Parameter
		{
			String name;

			const Type* type;

			bool isConstant;
			bool isReference;
			bool isPointer;

			bool operator==(const Parameter& other) const;
		};

	public:
		// Returns cloned copy
		virtual FunctionInfoBase* Clone() const;

		// Check Equals operator
		bool operator==(const FunctionInfoBase& other) const;

		// Returns owner type
		Type* GetOwnerType() const;

		// Returns function's name
		const String& GetName() const;

		// Returns return value type
		const Type* GetReturnType() const;

		// Returns function's parameters
		const Vector<Parameter>& GetParameters() const;

		// Returns protection section of function
		ProtectSection GetProtectionSection() const;

	protected:
		ProtectSection    mProtectSection; // Protection section
		Type*             mOwnerType;      // Owner type pointer
		String            mName;           // Name of function
		const Type*       mReturnType;     // Function returning type
		Vector<Parameter> mParameters;     // Function parameters list

		friend class Type;
		friend class ReflectionInitializationTypeProcessor;
	};

	// ----------------------------
	// Regular function information
	// ----------------------------
	class FunctionInfo : public FunctionInfoBase
	{
	public:
		// Returns cloned copy
		virtual FunctionInfoBase* Clone() const;

		// Check Equals operator
		bool operator==(const FunctionInfo& other) const;

		// Returns is function constant
		bool IsConstant() const;

		// Invokes function with parameters
		template<typename _res_type, typename ... _args>
		_res_type Invoke(void* object, _args ... args) const;

	protected:
		bool mIsContant; // Is function constant

		friend class Type;
		friend class ReflectionInitializationTypeProcessor;
	};

	// ---------------------------
	// Static function information
	// ---------------------------
	class StaticFunctionInfo : public FunctionInfoBase
	{
	public:
		// Returns cloned copy
		virtual FunctionInfoBase* Clone() const;

		// Invokes function with parameters
		template<typename _res_type, typename ... _args>
		_res_type Invoke(_args ... args) const;

	protected:
		friend class Type;
		friend class ReflectionInitializationTypeProcessor;
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

	// -----------------------------------
	// Static Function info specialization
	// -----------------------------------
	template<typename _res_type, typename ... _args>
	class SpecStaticFunctionInfo : public StaticFunctionInfo
	{
	public:
		// Returns cloned copy
		FunctionInfoBase* Clone() const
		{
			return mnew SpecStaticFunctionInfo(*this);
		}

		virtual _res_type Invoke(_args ... args) const
		{
			return mFunctionPtr(args ...);
		}

	private:
		_res_type(*mFunctionPtr)(_args ... args); // Pointer to static function

		friend class ReflectionInitializationTypeProcessor;
	};

	// -------------------------------
	// Function specialized class info
	// -------------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class SpecFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
	{
	public:
		// Returns cloned copy
		FunctionInfoBase* Clone() const
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

		friend class ReflectionInitializationTypeProcessor;
	};

	// ----------------------------------------
	// Constant function specialized class info
	// ----------------------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class SpecConstFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
	{
	public:
		// Returns cloned copy
		FunctionInfoBase* Clone() const
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

		friend class ReflectionInitializationTypeProcessor;
	};
	
	template<typename _res_type, typename ... _args>
	_res_type FunctionInfo::Invoke(void* object, _args ... args) const
	{
		const ISpecFunctionInfo<_res_type, _args ...>* thisSpec = dynamic_cast<const ISpecFunctionInfo<_res_type, _args ...>*>(this);
		return thisSpec->Invoke(object, args ...);
	}

	template<typename _res_type, typename ... _args>
	_res_type StaticFunctionInfo::Invoke(_args ... args) const
	{
		const SpecStaticFunctionInfo<_res_type, _args ...>* thisSpec = dynamic_cast<const SpecStaticFunctionInfo<_res_type, _args ...>*>(this);
		return thisSpec->Invoke(args ...);
	}

}
