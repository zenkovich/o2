#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace o2
{
#define Function std::function

	template<typename _res_type, typename ... _args>
	class IFunction
	{
	public:
		virtual ~IFunction() {}
		virtual IFunction* Clone() const = 0;

		virtual _res_type Invoke(_args ... args) const = 0;

		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}
	};

	template<typename _res_type, typename ... _args>
	class FunctionPtr: public IFunction<_res_type, _args ...> 
	{
		_res_type (*mFunctionPtr)(_args ... args);

	public:
		FunctionPtr(_res_type (*functionPtr)(_args ... args)):
			mFunctionPtr(functionPtr)
		{}

		FunctionPtr(const FunctionPtr& other):
			mFunctionPtr(other.mFunctionPtr)
		{}

		FunctionPtr& operator=(const FunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			return *this;
		}

		IFunction* Clone() const
		{
			return new FunctionPtr(*this);
		}

		_res_type Invoke(_args ... args) const
		{
			return mFunctionPtr(args ...);
		}
	};

	template<typename _class_type, typename _res_type, typename ... _args>
	class ObjFunctionPtr//: public IFuncInvoker<_res_type, _args ...>
	{
		_res_type (_class_type::*mFunctionPtr)(_args ... args);
		_class_type* mObject;

	public:
		ObjFunctionPtr(_class_type* object, _res_type(_class_type::*functionPtr)(_args ... args)):
			mFunctionPtr(functionPtr), mObject(object)
		{}

		ObjFunctionPtr(const ObjFunctionPtr& other):
			mFunctionPtr(other.mFunctionPtr), mObject(other.mObject)
		{}

		ObjFunctionPtr& operator=(const ObjFunctionPtr& other)
		{
			mFunctionPtr = other.mFunctionPtr;
			mObject = other.mObject;
			return *this;
		}

/*		IFunction* Clone() const
		{
			return new ObjFunctionPtr(*this);
		}*/

		_res_type Invoke(_args ... args) const
		{
			return (mObject->*mFunctionPtr)(args ...);
		}
	};


	template <typename UnusedType>
	class TFunction;

	template<typename _res_type, typename ... _args>
	class TFunction <_res_type(_args ...)>
	{
		struct IFuncInvoker
		{
			enum class InvokerType { Static, Object };

			virtual IFuncInvoker* Clone() const = 0;
			virtual _res_type Invoke(_args ... args) const = 0;
			virtual InvokerType GetType() const = 0;
			virtual bool Equals(IFuncInvoker* other) const = 0;
		};
		typedef std::vector<IFuncInvoker*> InvokersVec;

		template<typename _func_type>
		struct StaticFuncInvoker: public IFuncInvoker
		{
			_func_type* mFunction;

			StaticFuncInvoker(_func_type* func):
				mFunction(func)
			{
			}

			IFuncInvoker* Clone() const
			{
				return new StaticFuncInvoker(mFunction);
			}

			_res_type Invoke(_args ... args) const
			{
				return (*mFunction)(args ...);
			}

			InvokerType GetType() const
			{
				return IFuncInvoker::InvokerType::Static;
			}

			bool Equals(IFuncInvoker* other) const
			{
				if (other->GetType() != InvokerType::Static)
					return false;

				StaticFuncInvoker* otherStatic = (StaticFuncInvoker*)other;
				return mFunction == otherStatic->mFunction;
			}
		};

		template <typename _func_type, typename _class_type>
		struct ObjectFuncInvoker: public IFuncInvoker
		{
			typedef _func_type _class_type::* ObjectFuncSignature;

			ObjectFuncSignature mFunction;
			_class_type*        mObject;

			ObjectFuncInvoker(ObjectFuncSignature func, _class_type* object):
				mFunction(func), mObject(object)
			{
			}

			IFuncInvoker* Clone() const
			{
				return new ObjectFuncInvoker(mFunction, mObject);
			}

			_res_type Invoke(_args ... args) const
			{
				return (mObject->*mFunction)(args ...);
			}

			InvokerType GetType() const
			{
				return IFuncInvoker::InvokerType::Object;
			}

			bool Equals(IFuncInvoker* other) const
			{
				if (other->GetType() != InvokerType::Object)
					return false;

				ObjectFuncInvoker* otherObject = (ObjectFuncInvoker*)other;
				return mFunction == otherObject->mFunction && mObject == otherObject->mObject;
			}
		};

		InvokersVec mInvokers;

	public:
		TFunction()
		{
		}

		TFunction(const TFunction& other)
		{
			Add(other);
		}

		template<typename _func_type>
		TFunction(_func_type* func)
		{
			mInvokers.push_back(new StaticFuncInvoker<_func_type>(func));
		}

		template <typename _class_type>
		TFunction(_res_type(_class_type::*func)(_args...), _class_type& object)
		{
			mInvokers.push_back(new ObjectFuncInvoker<_res_type(_args ...), _class_type>(func, &object));
		}

		~TFunction()
		{
			Clear();
		}

		void Clear()
		{
			for (auto invoker:mInvokers)
				delete invoker;

			mInvokers.clear();
		}

		template<typename _func_type>
		void Add(_func_type* func)
		{
			mInvokers.push_back(new StaticFuncInvoker<_func_type>(func));
		}

		template <typename _class_type>
		void Add(_res_type(_class_type::*func)(_args...), _class_type& object)
		{
			mInvokers.push_back(new ObjectFuncInvoker<_res_type(_args ...), _class_type>(func, &object));
		}

		void Add(const TFunction& other)
		{
			for (auto inv:other.mInvokers)
				mInvokers.push_back(inv->Clone());
		}

		template<typename _func_type>
		void Remove(_func_type* func)
		{
			for (auto invokerIt = mInvokers.begin(); invokerIt != mInvokers.end(); ++invokerIt)
			{
				if ((*invokerIt)->GetType() == IFuncInvoker::InvokerType::Static)
				{
					StaticFuncInvoker<_func_type>* invStatic = (StaticFuncInvoker<_func_type>*)(*invokerIt);
					if (func == invStatic->mFunction)
					{
						mInvokers.erase(invokerIt);
						return;
					}
				}
			}
		}

		template <typename _class_type>
		void Remove(_res_type(_class_type::*func)(_args...), _class_type& object)
		{
			for (auto invokerIt = mInvokers.begin(); invokerIt != mInvokers.end(); ++invokerIt)
			{
				if ((*invokerIt)->GetType() == IFuncInvoker::InvokerType::Object)
				{
					ObjectFuncInvoker<_res_type(_args ...), _class_type>* invObject =
						(ObjectFuncInvoker<_res_type(_args ...), _class_type>*)(*invokerIt);

					if (func == invObject->mFunction && &object == invObject->mObject)
					{
						mInvokers.erase(invokerIt);
						return;
					}
				}
			}
		}

		void Remove(const TFunction<_res_type(_args ...)>& other)
		{
			for (auto otherInvoker:other.mInvokers)
			{
				for (auto invokerIt = mInvokers.begin(); invokerIt != mInvokers.end(); ++invokerIt)
				{
					if ((*invokerIt)->Equals(otherInvoker))
					{
						mInvokers.erase(invokerIt);
						break;
					}
				}
			}
		}

		template<typename _func_type>
		bool Contains(_func_type* func) const
		{
			for (auto invoker:mInvokers)
			{
				if (invoker->GetType() == IFuncInvoker::InvokerType::Static)
				{
					StaticFuncInvoker<_func_type>* invStatic = (StaticFuncInvoker<_func_type>*)invoker;
					if (func == invStatic->mFunction)
						return true;
				}
			}

			return false;
		}

		template <typename _class_type>
		bool Contains(_res_type(_class_type::*func)(_args...), _class_type& object) const
		{
			for (auto invoker:mInvokers)
			{
				if (invoker->GetType() == IFuncInvoker::InvokerType::Object)
				{
					ObjectFuncInvoker<_res_type(_args ...), _class_type>* invObject =
						(ObjectFuncInvoker<_res_type(_args ...), _class_type>*)invoker;

					if (func == invObject->mFunction && &object == invObject->mObject)
						return true;
				}
			}

			return false;
		}

		bool Contains(const TFunction& other) const
		{
			for (auto otherInvoker:other.mInvokers)
			{
				bool found = false;
				for (auto invoker:mInvokers)
				{
					if (invoker->Equals(otherInvoker))
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

		_res_type operator()(_args ... args) const
		{
			return Invoke(args ...);
		}

		_res_type Invoke(_args ... args) const
		{
			if (mInvokers.size() == 0)
				return _res_type();

			auto it = mInvokers.cbegin();
			for (; it != mInvokers.cend() - 1; ++it)
				(*it)->Invoke(args ...);

			return (*it)->Invoke(args ...);
		}

		template<typename _func_type>
		TFunction<_res_type(_args ...)>& operator=(_func_type* func)
		{
			Clear();
			Add(func);
			return *this;
		}

		TFunction<_res_type(_args ...)>& operator=(const TFunction& other)
		{
			Clear();
			Add(other);
			return *this;
		}

		bool operator==(const TFunction& other) const
		{
			for (auto invoker:mInvokers)
			{
				bool found = false;
				for (auto otherInvoker:other.mInvokers)
				{
					if (invoker->Equals(otherInvoker))
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

		bool operator!=(const TFunction& other) const
		{
			return !(*this == other);
		}

		template<typename _func_type>
		bool operator==(_func_type* func) const
		{
			if (mInvokers.size() != 1)
				return false;

			IFuncInvoker* invoker = mInvokers[0];
			if (invoker->GetType() != IFuncInvoker::InvokerType::Static)
				return false;

			StaticFuncInvoker<_func_type>* invStatic = (StaticFuncInvoker<_func_type>*)invoker;
			if (func != invStatic->mFunction)
				return false;

			return true;
		}

		template<typename _func_type>
		bool operator!=(_func_type* func) const
		{
			return !(*this == func);
		}

		template<typename _func_type>
		TFunction<_res_type(_args ...)> operator+(_func_type* func)
		{
			TFunction<_res_type(_args ...)> res(*this);
			res.Add(func);
			return res;
		}

		template<typename _func_type>
		TFunction<_res_type(_args ...)>& operator+=(_func_type* func)
		{
			Add(func);
			return *this;
		}

		TFunction<_res_type(_args ...)> operator+(const TFunction& other)
		{
			TFunction<_res_type(_args ...)> res(*this);
			res.Add(other);
			return res;
		}

		TFunction<_res_type(_args ...)> operator+=(const TFunction& other)
		{
			Add(other);
			return *this;
		}


		template<typename _func_type>
		TFunction<_res_type(_args ...)> operator-(_func_type* func)
		{
			TFunction<_res_type(_args ...)> res(*this);
			res.Remove(func);
			return res;
		}

		template<typename _func_type>
		TFunction<_res_type(_args ...)>& operator-=(_func_type* func)
		{
			Remove(func);
			return *this;
		}

		TFunction<_res_type(_args ...)> operator-(const TFunction& other)
		{
			TFunction<_res_type(_args ...)> res(*this);
			res.Remove(other);
			return res;
		}

		TFunction<_res_type(_args ...)> operator-=(const TFunction& other)
		{
			Remove(other);
			return *this;
		}
	};
}