#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace o2
{
//#define Function std::function

	template <typename UnusedType>
	class IFunction;

	template<typename _res_type, typename ... _args>
	class IFunction <_res_type(_args ...)>
	{
	public:
		virtual ~IFunction() {}
		virtual IFunction* Clone() const = 0;

		virtual _res_type Invoke(_args ... args) const = 0;

		virtual bool Equals(IFunction* other) const = 0;

		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}
	};

	template <typename UnusedType>
	class FunctionPtr;

	template<typename _res_type, typename ... _args>
	class FunctionPtr <_res_type(_args ...)> : public IFunction<_res_type(_args ...)>
	{
		_res_type(*mFunctionPtr)(_args ... args);

	public:
		FunctionPtr(_res_type(*functionPtr)(_args ... args)) :
			mFunctionPtr(functionPtr)
		{}

		FunctionPtr(const FunctionPtr& other) :
			mFunctionPtr(other.mFunctionPtr)
		{}

		FunctionPtr& operator=(const FunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			return *this;
		}

		bool operator==(const FunctionPtr& other) const
		{
			return mFunctionPtr == other.mFunctionPtr;
		}

		bool operator!=(const FunctionPtr& other) const
		{
			return mFunctionPtr != other.mFunctionPtr;
		}

		IFunction* Clone() const
		{
			return new FunctionPtr(*this);
		}

		_res_type Invoke(_args ... args) const
		{
			return mFunctionPtr(args ...);
		}

		bool Equals(IFunction* other) const
		{
			FunctionPtr* otherFuncPtr = dynamic_cast<FunctionPtr*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	template<typename _class_type, typename _res_type, typename ... _args>
	class ObjFunctionPtr : public IFunction<_res_type(_args ...)>
	{
		_res_type(_class_type::*mFunctionPtr)(_args ... args);
		_class_type* mObject;

	public:
		ObjFunctionPtr(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args)) :
			mFunctionPtr(functionPtr), mObject(object)
		{}

		ObjFunctionPtr(const ObjFunctionPtr& other) :
			mFunctionPtr(other.mFunctionPtr), mObject(other.mObject)
		{}

		ObjFunctionPtr& operator=(const ObjFunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			mObject = other.mObject;
			return *this;
		}

		bool operator==(const ObjFunctionPtr& other) const
		{
			return mObject == other.mObject && mFunctionPtr == other.mFunctionPtr;
		}

		bool operator!=(const ObjFunctionPtr& other) const
		{
			return mObject != other.mObject || mFunctionPtr != other.mFunctionPtr;
		}

		IFunction* Clone() const
		{
			return new ObjFunctionPtr(*this);
		}

		_res_type Invoke(_args ... args) const
		{
			return (mObject->*mFunctionPtr)(args ...);
		}
		
		bool Equals(IFunction* other) const
		{
			ObjFunctionPtr* otherFuncPtr = dynamic_cast<ObjFunctionPtr*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	template <typename UnusedType>
	class SharedLambda;

	template<typename _res_type, typename ... _args>
	class SharedLambda <_res_type(_args ...)> : public IFunction<_res_type(_args ...)>
	{
		struct ILambdaInvoker
		{
			virtual ~ILambdaInvoker() {}
			virtual _res_type Invoke(_args ... args) const = 0;
			virtual void IncRef() = 0;
			virtual void DecRef() = 0;
			virtual int RefCount() const = 0;
		};

		template<typename _lambda_type>
		struct LambdaInvoker : ILambdaInvoker
		{
			_lambda_type mLambda;
			int          mReferences;

			LambdaInvoker(const _lambda_type& lambda) :
				mLambda(lambda), mReferences(1)
			{}

			_res_type Invoke(_args ... args) const
			{
				return mLambda(args ...);
			}

			void IncRef()
			{
				mReferences++;
			}

			void DecRef()
			{
				mReferences--;
			}

			int RefCount() const
			{
				return mReferences;
			}
		};

		ILambdaInvoker* mInvokerPtr;

	public:
		template<typename _lambda_type>
		SharedLambda(const _lambda_type& lambda) :
			mInvokerPtr(new LambdaInvoker<_lambda_type>(lambda))
		{
		}

		SharedLambda(const SharedLambda& other) :
			mInvokerPtr(other.mInvokerPtr)
		{
			mInvokerPtr->IncRef();
		}

		~SharedLambda()
		{
			mInvokerPtr->DecRef();
			if (mInvokerPtr->RefCount() == 0)
				delete mInvokerPtr;
		}

		SharedLambda& operator=(const SharedLambda& other)
		{
			mInvokerPtr->DecRef();
			if (mInvokerPtr->RefCount() == 0)
				delete mInvokerPtr;

			mInvokerPtr = other.mInvokerPtr;
			mInvokerPtr->IncRef();

			return *this;
		}

		IFunction* Clone() const
		{
			return new SharedLambda(*this);
		}

		_res_type Invoke(_args ... args) const
		{
			return mInvokerPtr->Invoke(args ...);
		}

		bool operator==(const SharedLambda& other) const
		{
			return mInvokerPtr == other.mInvokerPtr;
		}

		bool operator!=(const SharedLambda& other) const
		{
			return mInvokerPtr != other.mInvokerPtr;
		}
		bool Equals(IFunction* other) const
		{
			SharedLambda* otherFuncPtr = dynamic_cast<SharedLambda*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}
	};

	template <typename UnusedType>
	class Function;

	template<typename _res_type, typename ... _args>
	class Function <_res_type(_args ...)> : public IFunction<_res_type(_args ...)>
	{
		typedef std::vector<IFunction*> FunctionsVec;

		FunctionsVec mFunctions;

	public:
		Function()
		{
		}

		Function(const Function& other)
		{
			for (auto func : other.mFunctions)
				mFunctions.push_back(func->Clone());
		}

		Function(const IFunction& func)
		{
			mFunctions.push_back(func.Clone());
		}

		template<typename _func_type>
		Function(const _func_type* func)
		{
			mFunctions.push_back(new FunctionPtr<_res_type(_args ...)>(func));
		}

		template<typename _lambda_type>
		Function(const _lambda_type& lambda)
		{
			mFunctions.push_back(new SharedLambda<_res_type(_args ...)>(lambda));
		}

		template<typename _class_type>
		Function(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			mFunctions.push_back(new ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		~Function()
		{
			Clear();
		}

		IFunction* Clone() const
		{
			return new Function(*this);
		}

		void Clear()
		{
			for (auto func : mFunctions)
				delete func;

			mFunctions.clear();
		}

// 		template<typename _func_type>
// 		void Add(const void* func)
// 		{
// 			mFunctions.push_back(new FunctionPtr<_res_type(_args ...)>((_func_type*)func));
// 		}

// 		template<typename _lambda_type>
// 		void Add(const _lambda_type* lambda)
// 		{
// 			mFunctions.push_back(new SharedLambda<_res_type(_args ...)>(*lambda));
// 		}
// 
		template<typename _class_type>
		void Add(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			mFunctions.push_back(new ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		void Add(const IFunction& func)
		{
			mFunctions.push_back(func.Clone());
		}

		void Add(const Function& funcs)
		{
			for (auto func : funcs.mFunctions)
				mFunctions.push_back(func->Clone());
		}

		void Remove(IFunction& func)
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

		template<typename _class_type>
		void Remove(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args))
		{
			Remove(ObjFunctionPtr<_class_type, _res_type, _args ...>(object, functionPtr));
		}

		bool Contains(const IFunction& func) const
		{
			for (auto funcIt = mFunctions.begin(); funcIt != mFunctions.end(); ++funcIt)
			{
				if ((*funcIt)->Equals(&func))
					return true;
			}

			return false;
		}

		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}

		_res_type Invoke(_args ... args) const
		{
			if (mFunctions.size() == 0)
				return _res_type();

			auto it = mFunctions.cbegin();
			for (; it != mFunctions.cend() - 1; ++it)
				(*it)->Invoke(args ...);

			return (*it)->Invoke(args ...);
		}

		Function<_res_type(_args ...)>& operator=(const IFunction& func)
		{
			Clear();
			Add(func);
			return *this;
		}

		Function<_res_type(_args ...)>& operator=(const Function& other)
		{
			Clear();
			Add(other);
			return *this;
		}		

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

		bool operator!=(const Function& other) const
		{
			return !(*this == other);
		}

		bool operator==(const IFunction& func) const
		{
			if (mInvokers.size() != 1)
				return false;

			return mFunctions[0]->Equals(&func);
		}

		bool operator!=(const IFunction& func) const
		{
			return !(*this == func);
		}

		operator bool() const
		{
			return mFunctions.size() > 0;
		}

		bool Equals(IFunction* other) const
		{
			Function* otherFuncPtr = dynamic_cast<Function*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}

		Function<_res_type(_args ...)> operator+(const IFunction& func) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Add(func);
			return res;
		}

		Function<_res_type(_args ...)>& operator+=(const IFunction& func)
		{
			Add(func);
			return *this;
		}
		
// 		template<typename _func_type>
// 		Functions<_res_type(_args ...)> operator+(const _func_type* func)
// 		{
// 			Functions<_res_type(_args ...)> res(*this);
// 			res.Add(func);
// 			return res;
// 		}

// 		template<typename _func_type>
// 		Functions<_res_type(_args ...)>& operator+=(const _func_type* func)
// 		{
// 			Add(func);
// 			return *this;
// 		}

		Function<_res_type(_args ...)> operator+(const Function& other) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Add(other);
			return res;
		}

		Function<_res_type(_args ...)>& operator+=(const Function& other)
		{
			Add(other);
			return *this;
		}

		Function<_res_type(_args ...)> operator-(const IFunction& func) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Remove(func);
			return res;
		}

		Function<_res_type(_args ...)>& operator-=(IFunction& func)
		{
			Remove(func);
			return *this;
		}

// 		template<typename _func_type>
// 		Functions<_res_type(_args ...)> operator-(const _func_type* func)
// 		{
// 			Functions<_res_type(_args ...)> res(*this);
// 			res.Remove(func);
// 			return res;
// 		}
// 
// 		template<typename _func_type>
// 		Functions<_res_type(_args ...)>& operator-=(const _func_type* func)
// 		{
// 			Remove(func);
// 			return *this;
// 		}

		Function<_res_type(_args ...)> operator-(const Function& other) const
		{
			Function<_res_type(_args ...)> res(*this);
			res.Remove(other);
			return res;
		}

		Function<_res_type(_args ...)>& operator-=(const Function& other)
		{
			Remove(other);
			return *this;
		}
	};
}