#include "memory_manager.h"

#include "linear_allocator.h"
#include "pool_allocator.h"
#include "std_allocator.h"

#include "util/log.h"
#include "util/math/math.h"
#include "util/threads/mutex.h"

OPEN_O2_NAMESPACE

MemoryManager::AllocSign::AllocSign(void* memPtr, uint size, const char* source, unsigned int sourceLine,
IAllocator* allocator):
mPtr(memPtr), mSize(size), mSourceLine(sourceLine), mAllocator(allocator)
{
	strcpy(mSource, source + Max<int>(0, (strlen(source) - 65)));
}

MemoryManager::AllocSign::AllocSign(const AllocSign& allocSign)
{
	*this = allocSign;
}

MemoryManager::AllocSign& MemoryManager::AllocSign::operator=(const AllocSign& allocSign)
{
	memcpy(this, &allocSign, sizeof(allocSign));
	return *this;
}


MemoryManager::MemoryManager()
{
#ifdef BASIC_MEMORY_ALLOCATOR
	BASIC_MEMORY_ALLOCATOR* allocator = (BASIC_MEMORY_ALLOCATOR*)malloc(sizeof(BASIC_MEMORY_ALLOCATOR));
	allocator = new (allocator)BASIC_MEMORY_ALLOCATOR(BASIC_MEMORY_ALLOCATOR_PARAMS);
	mBasicAllocator = allocator;
#else
	mBasicAllocator = NULL;
#endif

	mAllocSigns = (AllocSignsList*)malloc(sizeof(AllocSignsList));
	mAllocSigns = new (mAllocSigns)AllocSignsList;
	mUsedMemory = 0;

	mAllocSignsMutex = (Mutex*)malloc(sizeof(Mutex));
	mAllocSignsMutex = new (mAllocSignsMutex)Mutex;
}

MemoryManager::~MemoryManager()
{
	free(mAllocSigns);

#ifdef BASIC_MEMORY_ALLOCATOR
	static_cast<BASIC_MEMORY_ALLOCATOR*>(mBasicAllocator)->~BASIC_MEMORY_ALLOCATOR();
	free(mBasicAllocator);
#endif

	mAllocSignsMutex->~Mutex();
	free(mAllocSignsMutex);
}

void MemoryManager::RegistAllocation(void* memPtr, uint size, const char* source, unsigned int sourceLine, IAllocator* allocator)
{
	mStaticObj.mAllocSignsMutex->Lock();

	mStaticObj.mAllocSigns->push_back(AllocSign(memPtr, size, source, sourceLine, allocator));
	mStaticObj.mUsedMemory += size;

	mStaticObj.mAllocSignsMutex->Unlock();
}

void MemoryManager::UnregistAllocation(void* memPtr)
{
	if (mStaticObj.mAllocSignsMutex->TryLock() == 0)
	{
		for (AllocSignsList::iterator it = mStaticObj.mAllocSigns->begin(); it != mStaticObj.mAllocSigns->end(); ++it)
		{
			if (memPtr == it->mPtr)
			{
				mStaticObj.mUsedMemory -= it->mSize;
				mStaticObj.mAllocSigns->erase(it);
				mStaticObj.mAllocSignsMutex->Unlock();
				return;
			}
		}

		mStaticObj.mAllocSignsMutex->Unlock();
	}
}

void MemoryManager::Dump()
{
	//mStaticObj.mAllocSignsMutex->lock();

	HightLog("=====Memory manager dump=====");
	int i = 0;
	for (AllocSignsList::iterator it = mStaticObj.mAllocSigns->begin(); it != mStaticObj.mAllocSigns->end(); ++it)
	{
		HightLog("%i: %s: %s:%i %i bytes", i++, (it->mAllocator ? it->mAllocator->GetName():"sys allocator"), it->mSource,
				 it->mSourceLine, it->mSize);
	}
	HightLog("Total %i bytes (%.3f MB)", mStaticObj.mUsedMemory, (float)mStaticObj.mUsedMemory/1024.0f);
	HightLog("=============================");

	//mStaticObj.mAllocSignsMutex->unlock();
}

MemoryManager MemoryManager::mStaticObj;

CLOSE_O2_NAMESPACE