#include "linear_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

cLinearAllocator::cLinearAllocator( uint32 size, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size + sizeof(cMutex));
	}
	else
	{
		mMemory = (char*)malloc(size + sizeof(cMutex));
	}

	mMutex = new (mMemory + mMemorySize) cMutex;

	mUsedMemory = 0;
}

cLinearAllocator::~cLinearAllocator()
{
	mMutex->~cMutex();

	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		free(mMemory);
	}
}

void* cLinearAllocator::alloc( uint32 bytes )
{
	mMutex->lock();

	void* res = NULL;

	if (mUsedMemory + bytes < mMemorySize)
	{
		void* res = mMemory + mUsedMemory;
		mUsedMemory +=  bytes;
	}

	mMutex->unlock();

	return NULL;
}

void cLinearAllocator::free( void* ptr )
{
}

CLOSE_O2_NAMESPACE