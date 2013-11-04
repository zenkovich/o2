#include "std_allocator.h"

#include <stdlib.h>

OPEN_O2_NAMESPACE

cStdAllocator::cStdAllocator()
{

}

cStdAllocator::~cStdAllocator()
{

}

void* cStdAllocator::alloc( uint32 bytes )
{
	return malloc(bytes);
}

void* cStdAllocator::realloc( void* ptr, uint32 bytes )
{
	return mrealloc(ptr, bytes);
}

void cStdAllocator::free( void* ptr )
{
	mfree(ptr);
}


CLOSE_O2_NAMESPACE