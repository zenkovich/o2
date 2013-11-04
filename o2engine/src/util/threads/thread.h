#ifndef THREAD_H
#define THREAD_H

#include "public.h"

#include "pthreads/pthread.h"

OPEN_O2_NAMESPACE

class ICallback;

class cThread
{
public:
	enum ThreadPriority { TP_NORMAL = 0, TP_LOW, TP_HIGHT };

protected:
	pthread_t  mThreadId;
	ICallback* mThreadFunc;
	bool       mStarted;

	cThread(const cThread& thread) {}
	cThread& operator=(const cThread& thread) { return *this; }

	static void* threadFunc(void* arg);

public:
	cThread();
	~cThread();

	int start(ICallback* threadCallback, ThreadPriority threadPriority = TP_NORMAL);

	int join();

	int cancel();
};

CLOSE_O2_NAMESPACE

#endif //THREAD_H