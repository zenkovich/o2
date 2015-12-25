#include "allocator_interface.h"

OPEN_O2_NAMESPACE

void* IAllocator::AllocSrc(uint bytes, const char* source, unsigned int line)
{
	void* res = Alloc(bytes);
	MemoryManager::Instance().RegistAllocation(res, bytes, source, line, this);
	return res;
}

void IAllocator::Frees(void* ptr)
{
	MemoryManager::Instance().UnregistAllocation(ptr);
	Free(ptr);
}

void* mrealloc(void* ptr, uint newSize) { return realloc(ptr, newSize); }
void mfree(void*ptr) { free(ptr); }

CLOSE_O2_NAMESPACE