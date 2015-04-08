#include "stack_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

StackAllocator::StackAllocator(uint size, IAllocator* parentAllocator /*= NULL*/):
mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size);
	}
	else
	{
		mMemory = (char*)malloc(size);
	}

	mMutex = mnew Mutex;

	mUsedMemory = 0;
}

StackAllocator::~StackAllocator()
{
	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		Free(mMemory);
	}

	SafeRelease(mMutex);
}

void* StackAllocator::Alloc(uint bytes)
{
	mMutex->Lock();

	if (mUsedMemory + bytes >= mMemorySize)
	{
		return NULL;
		mMutex->Unlock();
	}
	else
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
		return res;

		mMutex->Unlock();
	}
}

void* StackAllocator::Realloc(void* ptr, uint bytes)
{
	mMutex->Lock();

	void* res = Alloc(bytes);
	memcpy(res, ptr, Min<uint>(bytes, mMemory + mUsedMemory - (char*)ptr));

	mMutex->Unlock();

	return res;
}

void StackAllocator::Free(void* ptr)
{
	mMutex->Lock();

	mUsedMemory = (uint)ptr - (uint)mMemory;

	mMutex->Unlock();
}

CLOSE_O2_NAMESPACE