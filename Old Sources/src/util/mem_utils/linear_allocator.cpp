#include "linear_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

LinearAllocator::LinearAllocator(uint size, IAllocator* parentAllocator /*= NULL*/):
mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size + sizeof(Mutex));
	}
	else
	{
		mMemory = (char*)malloc(size + sizeof(Mutex));
	}

	mMutex = new (mMemory + mMemorySize) Mutex;

	mUsedMemory = 0;
}

LinearAllocator::~LinearAllocator()
{
	mMutex->~Mutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		Free(mMemory);
	}
}

void* LinearAllocator::Alloc(uint bytes)
{
	mMutex->Lock();

	void* res = NULL;

	if (mUsedMemory + bytes < mMemorySize)
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
	}

	mMutex->Unlock();

	return NULL;
}

void LinearAllocator::Free(void* ptr)
{
}

CLOSE_O2_NAMESPACE