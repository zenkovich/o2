#pragma once

#include "util/public_namespace.h"
#include "util/public_types.h"

#include <algorithm>

OPEN_O2_NAMESPACE

template<typename T, bool arr>
class AutoPtr
{
	T* mObjPtr;

public:
	AutoPtr():mObjPtr(NULL) {}

	AutoPtr(T* objPtr)
	{
		mObjPtr = objPtr;
	}

	~AutoPtr()
	{
		if (arr)
		{
			SafeReleaseArr(mObjPtr);
		}
		else
		{
			SafeRelease(mObjPtr);
		}
	}

	bool IsInitialized() const
	{
		return mObjPtr != NULL;
	}

	T* Get()
	{
		return mObjPtr;
	}

	operator bool() { return mObjPtr != NULL; }
	operator T*() { return mObjPtr; }
	T* operator->() { return mObjPtr; }
	T& operator*() { return *mObjPtr; }

	AutoPtr& operator=(T* objPtr)
	{
		if (arr)
		{
			SafeReleaseArr(mObjPtr);
		}
		else
		{
			SafeRelease(mObjPtr);
		}
		mObjPtr = objPtr;
		return *this;
	}

private:
	AutoPtr& operator=(const AutoPtr* ptr) { return *this; }
};

#define autoPtr(type) AutoPtr<type, false>
#define autoArr(type) AutoPtr<type, true>

CLOSE_O2_NAMESPACE
