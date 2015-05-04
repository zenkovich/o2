#pragma once

#include <stdarg.h>
#include "Utils/Assert.h"

namespace o2
{
	template <typename CLASS> class Singleton
	{
	public:
		Singleton()                            { mInstance = static_cast<CLASS*>(this); }
		virtual ~Singleton()                   { mInstance = nullptr; }

		static CLASS& Instance()               { Assert(mInstance, "Singleton not initialized"); return *mInstance; }

		static CLASS* InstancePtr()            { return mInstance; }

		static void   InitializeSingleton()    { if (!mInstance) new CLASS; }
		static void   DeinitializeSingleton()  { delete mInstance; }

		static bool   IsSingletonInitialzed()  { return (mInstance != nullptr); }

	public:
		static CLASS* mInstance;
	};

#	define DECLARE_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::mInstance = NULL
#	define CREATE_SINGLETON(CLASS)  template<> CLASS* Singleton<CLASS>::mInstance = new CLASS()
}