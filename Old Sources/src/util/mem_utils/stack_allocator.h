#pragma once

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class Mutex;

class StackAllocator:public IAllocator
{
	IAllocator* mParentAllocator;
	char*       mMemory;
	uint        mMemorySize;
	uint        mUsedMemory;
	Mutex*      mMutex;

public:
	StackAllocator(uint size, IAllocator* parentAllocator = NULL);
	~StackAllocator();

	void* Alloc(uint bytes);
	void* Realloc(void* ptr, uint bytes);
	void  Free(void* ptr);
	const char* GetName() const { return "stack alloc"; }
};

CLOSE_O2_NAMESPACE