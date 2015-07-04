#pragma once

#include <stdarg.h>
#include "Utils/Assert.h"
#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	template <typename CLASS> class Singleton
	{
	public:
		virtual ~Singleton()                   { mInstance = nullptr; }

		static CLASS& Instance()               { Assert(mInstance, "Singleton not initialized"); return *mInstance; }

		static Ptr<CLASS> InstancePtr()        { return mInstance; }

		static void InitializeSingleton()      { if (!mInstance) new CLASS; }
		static void DeinitializeSingleton()    { delete mInstance; }

		static bool IsSingletonInitialzed()    { return (mInstance != nullptr); }

	public:
		static Ptr<CLASS> mInstance;
	};

#	define DECLARE_SINGLETON(CLASS) template<> Ptr<CLASS> Singleton<CLASS>::mInstance = NULL
#	define CREATE_SINGLETON(CLASS)  template<> Ptr<CLASS> Singleton<CLASS>::mInstance = mnew CLASS()
}