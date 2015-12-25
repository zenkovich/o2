#pragma once

#include "public.h"
#include "pthreads/pthread.h"

OPEN_O2_NAMESPACE

/** Mutex. Uses POSIX. */
class Mutex
{
	pthread_mutex_t mMutex;       /** Mutex identificator. */
	bool            mInitialized; /** True, when initialized. */

public:
	enum class Type { Default = 0, Recursive };

	/** ctor. */
	Mutex(Type type = Type::Default);

	/** dtor. */
	~Mutex();

	/** Locking mutex. Returns 0 if success. */
	int Lock();

	/** Trying to lock mutex. Returns 0 if success. */
	int TryLock();

	/** Unlocking mutex. */
	int Unlock();
};

CLOSE_O2_NAMESPACE