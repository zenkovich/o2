#ifndef TEST_ALLOCATOR_H
#define TEST_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class cMutex;

class cTestAllocator:public IAllocator
{
	IAllocator* mParentAllocator;
	char*       mMemory;
	uint32      mMemorySize;

	//cMutex*     mMutex;

public:
	cTestAllocator(uint32 size, IAllocator* parentAllocator = NULL);
	~cTestAllocator();

	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);
	const char* getName() const { return "test alloc"; }

	void dump();

protected:
	void* putBlock(void* ptr, uint32 size, bool clear);
};

CLOSE_O2_NAMESPACE

#endif //TEST_ALLOCATOR_H