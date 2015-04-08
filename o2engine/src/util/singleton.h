#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

template <typename CLASS> class Singleton
{
public:
	Singleton()                            { mInstance = static_cast<CLASS*>(this); }
	virtual ~Singleton()                   { mInstance = NULL; }

	static CLASS& Instance()               { o2assert(mInstance, "Singleton not initialized"); return *mInstance; }

	static CLASS* InstancePtr()            { return mInstance; }

	static void   InitializeSingleton()    { if (!mInstance) mnew CLASS; }
	static void   DeinitializeSingleton()  { SafeRelease(mInstance); }

	static bool   IsSingletonInitialzed()  { return (mInstance != NULL); }

public:
	static CLASS* mInstance;
};

#define DECLARE_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::mInstance = NULL
#define CREATE_SINGLETON(CLASS)  template<> CLASS* Singleton<CLASS>::mInstance = mnew CLASS()

CLOSE_O2_NAMESPACE