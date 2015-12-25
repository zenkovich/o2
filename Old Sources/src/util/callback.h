#pragma once

#include <cstdarg>
#include "public.h"

OPEN_O2_NAMESPACE

/* Stupid simple class. */
struct Dummy
{
	Dummy() {}
	~Dummy() {}
};


/* Callback interface. */
class ICallback
{
public:
	virtual ~ICallback() {}
	virtual void Invoke() = 0;
	virtual void Invoke(void* param, ...) { Invoke(); }
	virtual ICallback* Clone() const = 0;
};

/* Callback with return value interface. */
template<typename RetType>
class IRetCallback: public ICallback
{
public:
	virtual ~IRetCallback() {}
	virtual RetType InvokeRes() = 0;
	virtual RetType InvokeRes(void* param, ...) { return InvokeRes(); };
	virtual void Invoke() { InvokeRes(); }
};


/***********************************************************************/
/* Callback without parameters and with return value. */
/***********************************************************************/
template<typename RetType, typename ClassType = Dummy>
class RetCallback:public IRetCallback<RetType>
{
	ClassType* mObject;
	RetType(ClassType::*mObjectFunction)();
	RetType(*mFunction)();

public:
	RetCallback(ClassType* object, RetType(ClassType::*function)()):
		mObject(object), mObjectFunction(function) {}

	RetCallback(RetType(*function)()):
		mObject(NULL), mObjectFunction(NULL), mFunction(function) {}

	RetCallback(const RetCallback& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
	}

	RetType InvokeRes()
	{
		if (mObject && mObjectFunction)
			return (mObject->*mObjectFunction)();
		else
		if (mFunction)
			return (*mFunction)();
	}

	ICallback* Clone() const
	{
		return mnew RetCallback<RetType, ClassType>(*this);
	}
};

/* Fast callback creation function. */
template<typename RetType, typename ClassType>
IRetCallback<RetType>* callback(ClassType* object, RetType(ClassType::*function)())
{
	return mnew RetCallback<RetType, ClassType>(object, function);
}

/* Fast callback creation function. */
template<typename RetType>
inline IRetCallback<RetType>* callback(RetType(*function)())
{
	return mnew RetCallback<RetType, Dummy>(function);
}


/***********************************************************************/
/* Callbacks chain. */
/***********************************************************************/
class CallbackChain:public ICallback
{
public:
	typedef Vector<ICallback*> CallbacksVec;

protected:
	CallbacksVec mCallbacks;

public:
	CallbackChain() {}

	CallbackChain(int count, ...)
	{
		va_list vlist;
		va_start(vlist, count);

		for (int i = 0; i < count; i++)
			mCallbacks.push_back(va_arg(vlist, ICallback*));

		va_end(vlist);
	}

	CallbackChain(const CallbackChain& callbackChain)
	{
		FOREACH_CONST(CallbacksVec, callbackChain.mCallbacks, it)
		{
			mCallbacks.push_back((*it)->Clone());
		}
	}

	~CallbackChain()
	{
		RemoveAll();
	}

	void Add(ICallback* callback)
	{
		mCallbacks.push_back(callback);
	}

	void Remove(ICallback* callback, bool release = true)
	{
		CallbacksVec::iterator fnd = std::find(mCallbacks.begin(), mCallbacks.end(), callback);
		if (fnd != mCallbacks.end())
			mCallbacks.erase(fnd);

		if (release)
			SafeRelease(callback);
	}

	void RemoveAll()
	{
		for (CallbacksVec::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			SafeRelease(*it);

		mCallbacks.clear();
	}

	void Invoke()
	{
		for (CallbacksVec::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			(*it)->Invoke();
	}

	ICallback* Clone() const
	{
		return mnew CallbackChain(*this);
	}
};


/* Fast callback chain creation function. */
inline ICallback* callbackChain(int count, ...)
{
	CallbackChain* res = mnew CallbackChain();
	va_list vlist;
	va_start(vlist, count);

	for (int i = 0; i < count; i++)
		res->Add(va_arg(vlist, ICallback*));

	va_end(vlist);

	return res;
}


/***********************************************************************/
/* Callback without parametres. */
/***********************************************************************/
template<typename ClassType = Dummy>
class Callback:public ICallback
{
	ClassType* mObject;
	void (ClassType::*mObjectFunction)();
	void(*mFunction)();

public:
	Callback(ClassType* object, void (ClassType::*function)()):
		mObject(object), mObjectFunction(function) {}

	Callback(void(*function)()):
		mObject(NULL), mObjectFunction(NULL), mFunction(function) {}

	Callback(const Callback& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
	}

	void Invoke()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)();
		else if (mFunction) (*mFunction)();
	}

	ICallback* Clone() const
	{
		return mnew Callback<ClassType>(*this);
	}
};

/* Fast callback creation function. */
template<typename ClassType>
ICallback* callback(ClassType* object, void (ClassType::*function)()) { return mnew Callback<ClassType>(object, function); }

/* Fast callback creation function. */
inline ICallback* callback(void(*function)()) { return mnew Callback<Dummy>(function); }


/***********************************************************************/
/* Callback with 1 parameter. */
/***********************************************************************/
template<typename ArgT, typename ClassType = Dummy>
class Callback1Param:public ICallback
{
	ArgT       mArg;
	ClassType* mObject;
	void (ClassType::*mObjectFunction)(ArgT);
	void(*mFunction)(ArgT);

public:
	Callback1Param(ClassType* object, void (ClassType::*function)(ArgT), const ArgT& arg):
		mObject(object), mObjectFunction(function) {
		mArg = arg;
	}

	Callback1Param(void(*function)(ArgT), const ArgT& arg):
		mObject(NULL), mObjectFunction(NULL), mFunction(function) {
		mArg = arg;
	}

	Callback1Param(const Callback1Param<ArgT, ClassType>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
	}

	void Invoke()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg);
		else if (mFunction) (*mFunction)(mArg);
	}

	void Invoke(void* param, ...)
	{
		mArg = *((ArgT*)param);
		Invoke();
	}

	ICallback* Clone() const
	{
		return mnew Callback1Param<ArgT, ClassType>(*this);
	}
};

/* Fast callback1 creation function. */
template<typename ArgT, typename ClassType>
ICallback* callback(ClassType* object, void (ClassType::*function)(ArgT), const ArgT& arg)
{
	return mnew Callback1Param<ArgT, ClassType>(object, function, arg);
}

/* Fast callback1 creation function. */
template<typename ArgT>
ICallback* callback(void(*function)(ArgT), const ArgT& arg)
{
	return mnew Callback1Param<ArgT>(function, arg);
}


/***********************************************************************/
/* Callback with 2 parameters. */
/***********************************************************************/
template<typename ArgT, typename ArgT2, typename ClassType = Dummy>
class Callback2Param:public ICallback
{
	ArgT       mArg;
	ArgT2      mArg2;
	ClassType* mObject;
	void (ClassType::*mObjectFunction)(ArgT, ArgT2);
	void(*mFunction)(ArgT, ArgT2);

public:
	Callback2Param(ClassType* object, void (ClassType::*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2):
		mObject(object), mObjectFunction(function), mArg(arg1), mArg2(arg2) {}

	Callback2Param(void(*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2):
		mObject(NULL), mObjectFunction(NULL), mFunction(function), mArg(arg1), mArg2(arg2) {}

	Callback2Param(const Callback2Param<ArgT, ArgT2, ClassType>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
		mArg2 = callback.mArg2;
	}

	void Invoke()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg, mArg2);
		else if (mFunction) (*mFunction)(mArg, mArg2);
	}

	void Invoke(void* param, ...)
	{
		va_list vlist;
		va_start(vlist, param);

		mArg = *((ArgT*)param);
		mArg = va_arg(vlist, ArgT2);

		va_end(vlist);

		Invoke();
	}

	ICallback* Clone() const
	{
		return mnew Callback2Param<ArgT, ArgT2, ClassType>(*this);
	}
};

/* Fast callback2 creation function. */
template<typename ArgT, typename ArgT2, typename ClassType>
ICallback* callback(ClassType* object, void (ClassType::*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2)
{
	return mnew Callback2Param<ClassType>(object, function, arg, arg2);
}

/* Fast callback2 creation function. */
template<typename ArgT, typename ArgT2>
ICallback* callback(void(*function)(ArgT, ArgT2), const ArgT& arg, const ArgT2& arg2)
{
	return mnew Callback2Param(function, arg, arg2);
}

CLOSE_O2_NAMESPACE