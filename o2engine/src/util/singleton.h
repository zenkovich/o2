#ifndef SINGLETON_H
#define SINGLETON_H

#include "public.h"
OPEN_O2_NAMESPACE

template <typename T> class cSingleton
{
public:
	cSingleton()                        { mInstance = static_cast<T*>(this); }
	virtual ~cSingleton()               { mInstance = NULL; }

	static T&   instance()              { assert(mInstance, "Singleton not initialized"); return *mInstance; }
			    
	static T*   instancePtr()           { return mInstance; }

	static void initializeSingleton()   { if (!mInstance) mInstance = new T; }
	static void deinitializeSingleton() { safe_release(mInstance); }

	static bool isSingletonInitialzed() { return (mInstance != NULL); }

protected:
	static T* mInstance;
};

#define DECLARE_SINGLETON(T) template<> T* cSingleton<T>::mInstance = NULL
#define CREATE_SINGLETON(T) template<> T* cSingleton<T>::mInstance = new T

CLOSE_O2_NAMESPACE

#endif //SINGLETON_H