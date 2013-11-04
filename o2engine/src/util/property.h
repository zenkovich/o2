#ifndef PROPERTY_H
#define PROPERTY_H

#include "public.h"

#include "callback.h"

OPEN_O2_NAMESPACE
	
template<typename T>
class prop
{
	T          mObject;
	bool       mChanged;

	ICallback* mChangedCallback;

public:
	prop():mChanged(false), mChangedCallback(NULL) {}
	prop(const T& obj):mObject(obj), mChanged(false), mChangedCallback(NULL) {}

	void setCallback(ICallback* callback)
	{
		safe_release(mChangedCallback);
		mChangedCallback = callback;
	}

	void reset() { mChanged = false; }
	bool isChanged() const { return mChanged; }

	const T& value() const { return mObject; }

	prop<T>&& operator=(const T& obj)
	{
		if (obj != mObject)
		{
			mObject = obj;
			mChanged = true;
			if (mChangedCallback)
				mChangedCallback->call();
		}

		return this;
	}
	
	//T& operator=(prop<T>& propp) { return propp.mObject; }
	
	prop<T>& operator=(const prop<T>& propp) { return *this; }
};

template<typename T>
const T& operator=(const prop<T>& propp) { return propp.mObject; }

CLOSE_O2_NAMESPACE

#endif //PROPERTY_H