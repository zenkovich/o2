#include "memory_manager.h"

#include "linear_allocator.h"
#include "std_allocator.h"
#include "pool_allocator.h"

#include "util/log.h"
#include "util/math/math.h"
#include "util/threads/mutex.h"

OPEN_O2_NAMESPACE

cMemoryManager::AllocSign::AllocSign( void* memPtr, uint32 size, const char* source, unsigned int sourceLine, 
                                      IAllocator* allocator ):
	mPtr(memPtr), mSize(size), mSourceLine(sourceLine), mAllocator(allocator)
{
	strcpy(mSource, source + max<int>(0, (strlen(source) - 65)));
}

cMemoryManager::AllocSign::AllocSign( const AllocSign& allocSign )
{
	*this = allocSign;
}

cMemoryManager::AllocSign& cMemoryManager::AllocSign::operator=( const AllocSign& allocSign )
{
	memcpy(this, &allocSign, sizeof(allocSign));
	return *this;
}


cMemoryManager::cMemoryManager()
{
	#ifdef BASIC_MEMORY_ALLOCATOR
		BASIC_MEMORY_ALLOCATOR* allocator = (BASIC_MEMORY_ALLOCATOR*)malloc(sizeof(BASIC_MEMORY_ALLOCATOR));
		allocator = new (allocator) BASIC_MEMORY_ALLOCATOR(BASIC_MEMORY_ALLOCATOR_PARAMS);
		mBasicAllocator = allocator;
	#else
		mBasicAllocator = NULL;
	#endif
	
	mAllocSigns = (AllocSignsList*)malloc(sizeof(AllocSignsList));
	mAllocSigns = new (mAllocSigns) AllocSignsList;
	mUsedMemory = 0;

	mAllocSignsMutex = (cMutex*)malloc(sizeof(cMutex));
	mAllocSignsMutex = new (mAllocSignsMutex) cMutex;
}

cMemoryManager::~cMemoryManager()
{
	free(mAllocSigns);

	#ifdef BASIC_MEMORY_ALLOCATOR
		static_cast<BASIC_MEMORY_ALLOCATOR*>(mBasicAllocator)->~BASIC_MEMORY_ALLOCATOR();
		free(mBasicAllocator);
	#endif

	mAllocSignsMutex->~cMutex();
	free(mAllocSignsMutex);
}

void cMemoryManager::registAlloc( void* memPtr, uint32 size, const char* source, unsigned int sourceLine, IAllocator* allocator )
{
	mStaticObj.mAllocSignsMutex->lock();

	mStaticObj.mAllocSigns->push_back(AllocSign(memPtr, size, source, sourceLine, allocator));
	mStaticObj.mUsedMemory += size;

	mStaticObj.mAllocSignsMutex->unlock();
}

void cMemoryManager::unregistAlloc( void* memPtr )
{
	if (mStaticObj.mAllocSignsMutex->tryLock() == 0)
	{
		for (AllocSignsList::iterator it = mStaticObj.mAllocSigns->begin(); it != mStaticObj.mAllocSigns->end(); ++it)
		{
			if (memPtr == it->mPtr)
			{
				mStaticObj.mUsedMemory -= it->mSize;
				mStaticObj.mAllocSigns->erase(it);
				mStaticObj.mAllocSignsMutex->unlock();
				return;
			}
		}

		mStaticObj.mAllocSignsMutex->unlock();
	}
}

void cMemoryManager::dump()
{
	//mStaticObj.mAllocSignsMutex->lock();

	hlog("=====Memory manager dump=====");
	int i = 0;
	for (AllocSignsList::iterator it = mStaticObj.mAllocSigns->begin(); it != mStaticObj.mAllocSigns->end(); ++it)
	{
		hlog("%i: %s: %s:%i %i bytes", i++, (it->mAllocator ? it->mAllocator->getName():"sys allocator"), it->mSource, 
			                           it->mSourceLine, it->mSize);
	}
	hlog("Total %i bytes (%.3f MB)", mStaticObj.mUsedMemory, (float)mStaticObj.mUsedMemory/1024.0f);
	hlog("=============================");

	//mStaticObj.mAllocSignsMutex->unlock();
}

cMemoryManager cMemoryManager::mStaticObj;

CLOSE_O2_NAMESPACE