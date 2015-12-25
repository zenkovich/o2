#include "std_allocator.h"

#include <stdlib.h>

OPEN_O2_NAMESPACE

StdAllocator::StdAllocator()
{
}

StdAllocator::~StdAllocator()
{
}

void* StdAllocator::Alloc(uint bytes)
{
	return malloc(bytes);
}

void* StdAllocator::Realloc(void* ptr, uint bytes)
{
	return mrealloc(ptr, bytes);
}

void StdAllocator::Free(void* ptr)
{
	mfree(ptr);
}


CLOSE_O2_NAMESPACE