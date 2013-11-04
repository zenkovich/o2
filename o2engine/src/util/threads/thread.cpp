#include "thread.h"

#include "../callback.h"

OPEN_O2_NAMESPACE

cThread::cThread():mStarted(false), mThreadFunc(NULL)
{
}

cThread::~cThread()
{
}

int cThread::start( ICallback* threadCallback, ThreadPriority threadPriority /*= TP_NORMAL*/ )
{
	mThreadFunc = threadCallback;

	int res = pthread_create(&mThreadId, NULL, threadFunc, this);

	if (res)
		mStarted = true;

	return res;
}

int cThread::join()
{
	if (!mStarted)
		return -1;

	return pthread_join(mThreadId, NULL);
}

int cThread::cancel()
{
	if (!mStarted)
		return -1;

	return pthread_cancel(mThreadId);
}

void* cThread::threadFunc( void* arg )
{
	cThread* threadObj = reinterpret_cast<cThread*>(arg);
	threadObj->mThreadFunc->call();
	pthread_exit(NULL);
	return NULL;
}

CLOSE_O2_NAMESPACE