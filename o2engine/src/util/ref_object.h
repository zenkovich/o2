#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

/** Object, containing reference count. */
class IRefObject
{
protected:
	int mRefCount; /**< Reference count. */

public:
	/** ctor. */
	IRefObject():mRefCount(0) {}

	/** dtor. */
	virtual ~IRefObject() {}

	/** Increase reference count. */
	void IncRefCount()
	{
		mRefCount++;
		OnRefIncreased();
	}

	/** Decrease reference count. */
	void DecRefCount()
	{
		mRefCount--;
		OnRefDecreased();
		if (mRefCount == 0)
			OnZeroRefCount();
	}

	/** Returns reference count. */
	int GetRefCount()
	{
		return mRefCount;
	}

protected:
	/** Runs when reference counter was increased. */
	virtual void OnRefIncreased() {}

	/** Runs when reference counter was decraised.*/
	virtual void OnRefDecreased() {}

	/** Runs when reference counter sets to zero. */
	virtual void OnZeroRefCount() {}
};


/** Reference object. Containing IRefObject and working with reference count. */
template<typename T>
class cObjectRef
{
protected:
	T* mObject; /**< IRefObject object. */

public:
	/** ctor. */
	cObjectRef(T* object):mObject(object)
	{
		if (mObject)
			mObject->IncRefCount();
	}

	/** copy ctor. Increases reference. */
	cObjectRef(const cObjectRef<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->IncRefCount();
	}

	/** dtor. */
	virtual ~cObjectRef()
	{
		if (mObject)
			mObject->DecRefCount();
	}

	/** copy operator. */
	cObjectRef<T> operator=(const cObjectRef<T>& refObject)
	{
		mObject = refObject.mObject;
		if (mObject)
			mObject->IncRefCount();
		return *this;
	}
};

#define DEFINE_REF_TYPE() public:                                        \
	virtual UniqueType GetType() { return mObject->GetType(); }          \
	virtual const char* GetTypeName() { return mObject->GetTypeName(); }

CLOSE_O2_NAMESPACE
