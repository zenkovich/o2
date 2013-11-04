#ifndef MUTEX_H
#define MUTEX_H

#include "public.h"

#include "pthreads/pthread.h"

OPEN_O2_NAMESPACE

class cMutex
{
	pthread_mutex_t mMutex;
	bool            mInitialized;

public:
	enum Type { MT_DEFAULT = 0, MT_RECURSIVE };

	cMutex(Type type = MT_DEFAULT);
	~cMutex();

	int lock();
	int tryLock();
	int unlock();
};

CLOSE_O2_NAMESPACE

#endif //MUTEX_H