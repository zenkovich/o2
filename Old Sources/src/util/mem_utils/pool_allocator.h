#pragma once

#include "allocator_interface.h"
#include <Windows.h>

OPEN_O2_NAMESPACE

class Mutex;

class PoolAllocator:public IAllocator
{
	IAllocator* mParentAllocator;

	struct chunk
	{
		char* mData;
	};

	char*  mMemory;
	char*  mHead;
	uint   mMemorySize;
	uint16 mChunkSize;
	uint   mChunksCount;

	Mutex* mMutex;

public:
	PoolAllocator(uint chunksCount, uint16 chunkSize = 16, IAllocator* parentAllocator = NULL);
	~PoolAllocator();

	void* Alloc(uint bytes);
	void* Realloc(void* ptr, uint bytes);
	void  Free(void* ptr);

	const char* GetName() const { return "pool alloc"; }
};

CLOSE_O2_NAMESPACE