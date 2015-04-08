#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

/** Allocator interface. */
class IAllocator
{
public:
	/** Allocates bytes memory. */
	virtual void* Alloc(uint bytes) { return 0; }

	/** Frees memory by pointer. */
	virtual void Free(void* ptr) {}

	/** Returns allocator identificator. */
	virtual const char* GetName() const { return "unknown"; }

	/** Allocates bytes memory and registering allocation. */
	void* AllocSrc(uint bytes, const char* source, unsigned int line);

	/** Frees memory by pointer and removing from registry. */
	void Frees(void* ptr);
};

void* mrealloc(void* ptr, uint newSize);
void mfree(void*ptr);

CLOSE_O2_NAMESPACE