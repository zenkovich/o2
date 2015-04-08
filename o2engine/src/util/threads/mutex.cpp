#include "mutex.h"

OPEN_O2_NAMESPACE

Mutex::Mutex(Type type /*= MT_DEFAULT*/)
{
	if (type == Type::Recursive)
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		int res = pthread_mutex_init(&mMutex, &attr);
		pthread_mutexattr_destroy(&attr);

		mInitialized = res == 0;
	}
	else
	{
		mInitialized = pthread_mutex_init(&mMutex, NULL) == 0;
	}
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mMutex);
}

int Mutex::Lock()
{
	if (!mInitialized)
		return -1;

	return pthread_mutex_lock(&mMutex);
}

int Mutex::TryLock()
{
	if (!mInitialized)
		return -1;

	return pthread_mutex_trylock(&mMutex);
}

int Mutex::Unlock()
{
	if (!mInitialized)
		return -1;

	return pthread_mutex_unlock(&mMutex);
}

CLOSE_O2_NAMESPACE