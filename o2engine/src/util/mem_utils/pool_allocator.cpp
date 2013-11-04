#include "pool_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

	//void mfree(void* ptr) { free(ptr); }

cPoolAllocator::cPoolAllocator( uint32 chunksCount, uint16 chunkSize /*= 4*/, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator)
{
	mMemorySize = chunksCount*chunkSize;
	mChunkSize = chunkSize;
	mChunksCount = chunksCount;

	uint32 mutexSize = sizeof(cMutex);

	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, mMemorySize + mutexSize);
	}
	else
	{
		mMemory = (char*)malloc(mMemorySize + mutexSize);
	}

	for (uint32 i = 0; i < mChunksCount - 1; i++)
	{
		*(uint32*)(mMemory + i*mChunkSize) = (uint32)(mMemory + (i + 1)*mChunkSize);
	}
	*(uint32*)(mMemory + (mChunksCount - 1)*mChunkSize) = NULL;

	mHead = mMemory;

	mMutex = new (mMemory + mMemorySize) cMutex;
}

cPoolAllocator::~cPoolAllocator()
{
	mMutex->~cMutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		mfree(mMemory);
	}
}

void* cPoolAllocator::alloc( uint32 bytes )
{	
	mMutex->lock();

	if (!mHead)
		return NULL;

	void* res = mHead;
	mHead = (char*)(*(uint32*)mHead);

	mMutex->unlock();

	return res;
}

void* cPoolAllocator::realloc( void* ptr, uint32 bytes )
{
	return ptr;
}

void cPoolAllocator::free( void* ptr )
{
	mMutex->lock();

	if (!ptr)
		return;
	
	*(uint32*)(ptr) = (uint32)(mHead);
	mHead = (char*)ptr;

	mMutex->unlock();
}

CLOSE_O2_NAMESPACE