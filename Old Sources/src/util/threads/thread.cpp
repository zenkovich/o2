#include "thread.h"

#include "../callback.h"

OPEN_O2_NAMESPACE

Thread::Thread():mStarted(false), mThreadFunc(NULL)
{
}

Thread::~Thread()
{
	SafeRelease(mThreadFunc);
}

int Thread::Start(ICallback* threadCallback, Priority threadPriority /*= TP_NORMAL*/)
{
	mThreadFunc = threadCallback;

	int res = pthread_create(&mThreadId, NULL, ThreadFunc, this);

	if (res)
		mStarted = true;

	return res;
}

int Thread::Join()
{
	if (!mStarted)
		return -1;

	return pthread_join(mThreadId, NULL);
}

int Thread::Cancel()
{
	if (!mStarted)
		return -1;

	return pthread_cancel(mThreadId);
}

void* Thread::ThreadFunc(void* arg)
{
	Thread* threadObj = reinterpret_cast<Thread*>(arg);
	threadObj->mThreadFunc->Invoke();
	pthread_exit(NULL);
	return NULL;
}

CLOSE_O2_NAMESPACE