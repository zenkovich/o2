#pragma once

#include "public.h"
#include "pthreads/pthread.h"
#include "util/callback.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Thread starter class. Uses POSIX. */
class Thread
{
public:
	enum class Priority { Normal, Low, Hight };

protected:
	pthread_t  mThreadId;   /** Thread id. */
	ICallback* mThreadFunc; /** Function starting in thread. */
	bool       mStarted;    /** True, when started. */

	/** copy ctor. */
	Thread(const Thread& thread) {}

	/** copy operator. */
	Thread& operator=(const Thread& thread) { return *this; }

	/** Thread function. Here calls mThreadFunc. */
	static void* ThreadFunc(void* arg);

public:
	/** ctor. */
	Thread();

	/** dtor. */
	~Thread();

	/** Starts function with priority. */
	int Start(ICallback* threadCallback, Priority threadPriority = Priority::Normal);

	/** Join. */
	int Join();

	/** Canceling thread. */
	int Cancel();
};

CLOSE_O2_NAMESPACE