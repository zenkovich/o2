#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	template <typename UnusedType>
	class IFunction;

	// ------------------------
	// Basic delegate interface
	// ------------------------
	template<typename _res_type, typename ... _args>
	class IFunction <_res_type(_args ...)>
	{
	public:
		// Virtual destructor
		virtual ~IFunction() {}

		// Returns cloned copy of this
		virtual IFunction* Clone() const = 0;

		// Invokes function with arguments
		virtual _res_type Invoke(_args ... args) const = 0;

		// Returns true if other functions is equal
		virtual bool Equals(IFunction<_res_type(_args ...)>* other) const = 0;

		// Invokes function with arguments as functor
		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}
	};

	template <typename UnusedType>
	class FunctionPtr;

	// ------------------------
	// Static function delegate
	// ------------------------
	template<typename _res_type, typename ... _args>
	class FunctionPtr <_res_type(_args ...)>: public IFunction<_res_type(_args ...)>
	{
		_res_type(*mFunctionPtr)(_args ... args); // Pointer to static function

	public:
		// Constructor
		FunctionPtr(_res_type(*functionPtr)(_args ... args)):
			mFunctionPtr(functionPtr)
		{}

		// Copy-constructor
		FunctionPtr(const FunctionPtr& other):
			mFunctionPtr(other.mFunctionPtr)
		{}

		// Copy-operator
		FunctionPtr& operator=(const FunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			return *this;
		}

		// Equal operator
		bool operator==(const FunctionPtr& other) const
		{
			return mFunctionPtr == other.mFunctionPtr;
		}

		// Not equal operator
		bool operator!=(const FunctionPtr& other) const
		{
			return mFunctionPtr != other.mFunctionPtr;
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const
		{
			return mnew FunctionPtr(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const
		{
			return mFunctionPtr(args ...);
		}

		// Returns true if functions is equal
		bool Equals(IFunction<_res_type(_args ...)>* other) const
		{
			FunctionPtr* otherFuncPtr = dynamic_cast<FunctionPtr*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	// ------------------------
	// Object function delegate
	// ------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class ObjFunctionPtr: public IFunction<_res_type(_args ...)>
	{
		_res_type(_class_type::*mFunctionPtr)(_args ... args); // Pointer to function
		_class_type* mObject;                                  // Pointer to function's owner object

	public:
		// Constructor
		ObjFunctionPtr(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args)):
			mFunctionPtr(functionPtr), mObject(object)
		{}

		// Copy-constructor
		ObjFunctionPtr(const ObjFunctionPtr& other):
			mFunctionPtr(other.mFunctionPtr), mObject(other.mObject)
		{}

		// Copy-operator
		ObjFunctionPtr& operator=(const ObjFunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			mObject = other.mObject;
			return *this;
		}

		// Equals operator
		bool operator==(const ObjFunctionPtr& other) const
		{
			return mObject == other.mObject && mFunctionPtr == other.mFunctionPtr;
		}

		// Not equals operator
		bool operator!=(const ObjFunctionPtr& other) const
		{
			return mObject != other.mObject || mFunctionPtr != other.mFunctionPtr;
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const
		{
			return mnew ObjFunctionPtr(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const
		{
			return (mObject->*mFunctionPtr)(args ...);
		}

		// Returns true if functions is equal
		bool Equals(IFunction<_res_type(_args ...)>* other) const
		{
			ObjFunctionPtr* otherFuncPtr = dynamic_cast<ObjFunctionPtr*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	// ---------------------------------
	// Object constant function delegate
	// ---------------------------------
	template<typename _class_type, typename _res_type, typename ... _args>
	class ObjConstFunctionPtr: public IFunction<_res_type(_args ...)>
	{
		_res_type(_class_type::*mFunctionPtr)(_args ... args) const; // Pointer to const function
		_class_type* mObject;                                        // Pointer to function's owner object

	public:
		// Constructor
		ObjConstFunctionPtr(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args) const):
			mFunctionPtr(functionPtr), mObject(object)
		{}

		// Copy-constructor
		ObjConstFunctionPtr(const ObjConstFunctionPtr& other):
			mFunctionPtr(other.mFunctionPtr), mObject(other.mObject)
		{}

		// Copy-operator
		ObjConstFunctionPtr& operator=(const ObjConstFunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			mObject = other.mObject;
			return *this;
		}

		// Equals operator
		bool operator==(const ObjConstFunctionPtr& other) const
		{
			return mObject == other.mObject && mFunctionPtr == other.mFunctionPtr;
		}

		// Not equals operator
		bool operator!=(const ObjConstFunctionPtr& other) const
		{
			return mObject != other.mObject || mFunctionPtr != other.mFunctionPtr;
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const
		{
			return mnew ObjConstFunctionPtr(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const
		{
			return (mObject->*mFunctionPtr)(args ...);
		}

		// Returns true if functions is equal
		bool Equals(IFunction<_res_type(_args ...)>* other) const
		{
			ObjConstFunctionPtr* otherFuncPtr = dynamic_cast<ObjConstFunctionPtr*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	template <typename UnusedType>
	class SharedLambda;

	// ----------------------
	// Shared lambda delegate
	// ----------------------
	template<typename _res_type, typename ... _args>
	class SharedLambda <_res_type(_args ...)>: public IFunction<_res_type(_args ...)>
	{
		// ------------------------
		// Lambda invoker interface
		// ------------------------
		struct ILambdaInvoker
		{
			virtual ~ILambdaInvoker() {}
			virtual _res_type Invoke(_args ... args) const = 0;
			virtual void IncRef() = 0;
			virtual void DecRef() = 0;
			virtual int RefCount() const = 0;
		};

		// -----------------------
		// Template lambda invoker
		// -----------------------
		template<typename _lambda_type>
		struct LambdaInvoker: ILambdaInvoker
		{
			_lambda_type mLambda;     // Lambda object (anonymous functor)
			int          mReferences; // References count to this

			// Constructor
			LambdaInvoker(const _lambda_type& lambda):
				mLambda(lambda), mReferences(1)
			{}

			// Invokes lambda
			_res_type Invoke(_args ... args) const
			{
				return mLambda(args ...);
			}

			// Increases references count
			void IncRef()
			{
				mReferences++;
			}

			// Decreases references count
			void DecRef()
			{
				mReferences--;
			}

			// Returns count of references
			int RefCount() const
			{
				return mReferences;
			}
		};

		ILambdaInvoker* mInvokerPtr; // Lambda invoker pinter

	public:
		SharedLambda():mInvokerPtr(nullptr) {}

		// Constructor
		template<typename _lambda_type>
		SharedLambda(const _lambda_type& lambda):
			mInvokerPtr(new LambdaInvoker<_lambda_type>(lambda))
		{}

		// Copy-constructor
		SharedLambda(const SharedLambda& other):
			mInvokerPtr(other.mInvokerPtr)
		{
			mInvokerPtr->IncRef();
		}

		// Destructor
		~SharedLambda()
		{
			if (mInvokerPtr)
			{
				mInvokerPtr->DecRef();
				if (mInvokerPtr->RefCount() == 0)
					delete mInvokerPtr;
			}
		}

		// Copy-operator
		SharedLambda& operator=(const SharedLambda& other)
		{
			if (mInvokerPtr)
			{
				mInvokerPtr->DecRef();
				if (mInvokerPtr->RefCount() == 0)
					delete mInvokerPtr;
			}

			mInvokerPtr = other.mInvokerPtr;
			mInvokerPtr->IncRef();

			return *this;
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const
		{
			return mnew SharedLambda(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const
		{
			return mInvokerPtr->Invoke(args ...);
		}

		// Equal operator
		bool operator==(const SharedLambda& other) const
		{
			return mInvokerPtr == other.mInvokerPtr;
		}

		// Not equal operator
		bool operator!=(const SharedLambda& other) const
		{
			return mInvokerPtr != other.mInvokerPtr;
		}

		// Returns true if functions is equals
		bool Equals(IFunction<_res_type(_args ...)>* other) const
		{
			SharedLambda* otherFuncPtr = dynamic_cast<SharedLambda*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	template <typename UnusedType>
	class Function;

	// --------------------------------------------------
	// Combined delegate. Can contain many other functors
	// --------------------------------------------------
	template<typename _res_type, typename ... _args>
	class Function <_res_type(_args ...)>: public IFunction<_res_type(_args ...)>
	{
		typedef std::vector<IFunction<_res_type(_args ...)>*> FunctionsVec;

		FunctionsVec mFunctions; // Vector of functors

	public:
		static const Function<_res_type(_args ...)> empty;

	public:
		// Constructor
		Function()
		{}

		// Copy-constructor
		Function(const Function& other)
		{
			for (auto func : other.mFunctions)
				mFunctions.push_back(func->Clone());
		}

		// Constructor from IFunction
		Function(const IFunction<_res_type(_args ...)>& func)
		{
			mFunctions.push_back(func.Clone());
		}

		// Constructor from static function pointer
		template<typename _func_type>
		Function(const _func_type* func)
		{
			mFunctions.push_back(new Function<_res_type(_args ...)>(*func));
		}

		// Constructor from lambda
		template<typename _lambda_type>
		Function(const _lambda_type& lambda)
		{
			mFunctions.push_back(new SharedLambda<_res_type(_args ...)>(lambda));
		}

		// Constructor from object and his function
		template<typename _class_type>
		Function(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			mFunctions.push_back(new ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Constructor from object and his function
		template<typename _class_type>
		Function(const ObjFunctionPtr<_class_type, _res_type, _args ...>& func)
		{
			mFunctions.push_back(new ObjFunctionPtr<_class_type, _res_type, _args ...>(func));
		}

		// Constructor from object and his function
		template<typename _class_type>
		Function(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args) const)
		{
			mFunctions.push_back(new ObjConstFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Destructor
		~Function()
		{
			Clear();
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const
		{
			return mnew Function(*this);
		}

		// Removing all inside functions
		void Clear()
		{
			for (auto func : mFunctions)
				delete func;

			mFunctions.clear();
		}

		// Returns true when function is empty
		bool IsEmpty() const
		{
			return mFunctions.size() == 0;
		}

		// Add delegate to inside list
		template<typename _class_type>
		void Add(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			mFunctions.push_back(new ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Add delegate to inside list
		template<typename _class_type>
		void Add(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args) const)
		{
			mFunctions.push_back(new ObjConstFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Add delegate to inside list
		void Add(const IFunction<_res_type(_args ...)>& func)
		{
			mFunctions.push_back(func.Clone());
		}

		// Add delegate to inside list
		void Add(const Function& funcs)
		{
			for (auto func : funcs.mFunctions)
				mFunctions.push_back(func->Clone());
		}

		// Add delegate to inside list
		void Remove(IFunction<_res_type(_args ...)>& func)
		{
			for (auto funcIt = mFunctions.begin(); funcIt != mFunctions.end(); ++funcIt)
			{
				if ((*funcIt)->Equals(&func))
				{
					delete *funcIt;
					mFunctions.erase(funcIt);
					break;
				}
			}
		}

		// Remove delegate from list
		void Remove(const Function& func)
		{
			for (auto funcIt = mFunctions.begin(); funcIt != mFunctions.end(); )
			{
				bool found = false;
				for (auto funcIt2 = func.mFunctions.begin(); funcIt2 != func.mFunctions.end(); ++funcIt2)
				{
					if ((*funcIt)->Equals(*funcIt2))
					{
						found = true;
						break;
					}
				}

				if (found)
				{
					delete *funcIt;
					funcIt = mFunctions.erase(funcIt);
				}
				else ++funcIt;
			}
		}

		// Remove delegate from list
		template<typename _class_type>
		void Remove(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			Remove(ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Remove delegate from list
		template<typename _class_type>
		void Remove(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args) const)
		{
			Remove(ObjConstFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		// Returns true, if this contains the delegate
		bool Contains(const IFunction<_res_type(_args ...)>& func) const
		{
			for (auto funcIt = mFunctions.begin(); funcIt != mFunctions.end(); ++funcIt)
			{
				if ((*funcIt)->Equals(&func))
					return true;
			}

			return false;
		}

		// Invokes function with arguments as functor
		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}

		// Invokes function with arguments
		_res_type Invoke(_args ... args) const
		{
			if (mFunctions.size() == 0)
				return _res_type();

			auto it = mFunctions.cbegin();
			for (; it != mFunctions.cend() - 1; ++it)
				(*it)->Invoke(args ...);

			return (*it)->Invoke(args ...);
		}

		// Copy operator
		Function<_res_type(_args ...)>& operator=(const IFunction<_res_type(_args ...)>& func)
		{
			Clear();
			Add(func);
			return *this;
		}

		// Copy operator
		Function<_res_type(_args ...)>& operator=(const Function& other)
		{
			Clear();
			Add(other);
			return *this;
		}

		// Equal operator
		bool operator==(const Function& other) const
		{
			for (auto func : mFunctions)
			{
				bool found = false;
				for (auto otherFunc : other.mFunctions)
				{
					if (func->Equals(otherFunc))
					{
						found = true;
						break;
					}
				}

				if (!found)
					return false;
			}

			return true;
		}

		// Not equal operator
		bool operator!=(const Function& other) const
		{
			return !(*this == other);
		}

		// Equal operator
		bool operator==(const IFunction<_res_type(_args ...)>& func) const
		{
			if (mFunctions.size() != 1)
				return false;

			return mFunctions[0]->Equals(&func);
		}

		// Not equal operator
		bool operator!=(const IFunction<_res_type(_args ...)>& func) const
		{
			return !(*this == func);
		}

		// Returns true, when delegates list isn't empty
		operator bool() const
		{
			return mFunctions.size() > 0;
		}

		// Returns true when functions is equal
		bool Equals(IFunction<_res_type(_args ...)>* other) const
		{
			Function* otherFuncPtr = dynamic_cast<Function*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}

		// Add delegate to inside list
		Function<_res_type(_args ...)> operator+(const IFunction<_res_type(_args ...)>& func) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Add(func);
			return res;
		}

		// Add delegate to inside list
		Function<_res_type(_args ...)>& operator+=(const IFunction<_res_type(_args ...)>& func)
		{
			Add(func);
			return *this;
		}

		// Add delegate to inside list
		Function<_res_type(_args ...)> operator+(const Function& other) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Add(other);
			return res;
		}

		// Add delegate to inside list
		Function<_res_type(_args ...)>& operator+=(const Function& other)
		{
			Add(other);
			return *this;
		}

		// Removes delegate from list
		Function<_res_type(_args ...)> operator-(const IFunction<_res_type(_args ...)>& func) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Remove(func);
			return res;
		}

		// Removes delegate from list
		Function<_res_type(_args ...)>& operator-=(IFunction<_res_type(_args ...)>& func)
		{
			Remove(func);
			return *this;
		}

		// Removes delegate from list
		Function<_res_type(_args ...)> operator-(const Function& other) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Remove(other);
			return res;
		}

		// Removes delegate from list
		Function<_res_type(_args ...)>& operator-=(const Function& other)
		{
			Remove(other);
			return *this;
		}
	};

	template<typename _res_type, typename ... _args>
	const Function<_res_type(_args ...)> Function<_res_type(_args ...)>::empty;

	template<typename _class_type, typename _res_type, typename ... _args>
	Function<_res_type(_args ...)> Func(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
	{
		return Function<_res_type(_args ...)>(object, functionPtr);
	}

#define THIS_FUNC(NAME) Func(this, &thisclass::NAME)
}
