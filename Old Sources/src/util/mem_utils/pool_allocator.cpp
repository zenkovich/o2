#include "pool_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE


PoolAllocator::PoolAllocator(uint chunksCount, uint16 chunkSize /*= 4*/, IAllocator* parentAllocator /*= NULL*/):
mParentAllocator(parentAllocator)
{
	mMemorySize = chunksCount*chunkSize;
	mChunkSize = chunkSize;
	mChunksCount = chunksCount;

	uint mutexSize = sizeof(Mutex);

	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, mMemorySize + mutexSize);
	}
	else
	{
		mMemory = (char*)malloc(mMemorySize + mutexSize);
	}

	for (uint i = 0; i < mChunksCount - 1; i++)
	{
		*(uint*)(mMemory + i*mChunkSize) = (uint)(mMemory + (i + 1)*mChunkSize);
	}
	*(uint*)(mMemory + (mChunksCount - 1)*mChunkSize) = NULL;

	mHead = mMemory;

	mMutex = new (mMemory + mMemorySize) Mutex;
}

PoolAllocator::~PoolAllocator()
{
	mMutex->~Mutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		mfree(mMemory);
	}
}

void* PoolAllocator::Alloc(uint bytes)
{
	mMutex->Lock();

	if (!mHead)
		return NULL;

	void* res = mHead;
	mHead = (char*)(*(uint*)mHead);

	mMutex->Unlock();

	return res;
}

void* PoolAllocator::Realloc(void* ptr, uint bytes)
{
	return ptr;
}

void PoolAllocator::Free(void* ptr)
{
	mMutex->Lock();

	if (!ptr)
		return;

	*(uint*)(ptr) = (uint)(mHead);
	mHead = (char*)ptr;

	mMutex->Unlock();
}

CLOSE_O2_NAMESPACE