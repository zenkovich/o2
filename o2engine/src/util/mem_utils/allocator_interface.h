#ifndef IALLOCATOR_H
#define IALLOCATOR_H

#include "public.h"

OPEN_O2_NAMESPACE
	
/** Allocator interface. */
class IAllocator
{
public:
	/** Allocates bytes memory. */
	virtual void* alloc(uint32 bytes) { return 0; }

	/** Frees memory by pointer. */
	virtual void free(void* ptr) {}

	/** Returns allocator identificator. */
	virtual const char* getName() const { return "unknown"; }

	/** Allocates bytes memory and registering allocation. */
	void* allocs(uint32 bytes, const char* source, unsigned int line);
	
	/** Frees memory by pointer and removing from registry. */
	void frees(void* ptr);
};

void* mrealloc(void* ptr, uint32 newSize);
void mfree(void*ptr);

CLOSE_O2_NAMESPACE

#endif //IALLOCATOR_H