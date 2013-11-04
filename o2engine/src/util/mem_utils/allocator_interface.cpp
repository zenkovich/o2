#include "allocator_interface.h"

OPEN_O2_NAMESPACE

void* IAllocator::allocs( uint32 bytes, const char* source, unsigned int line )
{	
	void* res = alloc(bytes);
	cMemoryManager::instance().registAlloc(res, bytes, source, line, this);
	return res;
}

void IAllocator::frees( void* ptr )
{	
	cMemoryManager::instance().unregistAlloc(ptr);
	free(ptr);
}

void* mrealloc(void* ptr, uint32 newSize) { return realloc(ptr, newSize); }
void mfree(void*ptr) { free(ptr); }

CLOSE_O2_NAMESPACE