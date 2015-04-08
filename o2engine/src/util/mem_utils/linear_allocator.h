#pragma once

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class Mutex;

/** Linear allocator. */
class LinearAllocator:public IAllocator
{
	IAllocator* mParentAllocator; /**< Parent allocator. This allocator allocates memory from parent allocator.
									*  If parent allocator not specified, using system allocator. */

	char*       mMemory;          /**< Memory ptr. */
	uint        mMemorySize;      /**< Size of memory. */
	uint        mUsedMemory;      /**< Size of used memory. */
	Mutex*      mMutex;           /**< Mutex. */

public:
	/** ctor. */
	LinearAllocator(uint size, IAllocator* parentAllocator = NULL);

	/** dtor. */
	~LinearAllocator();

	/** Allocates bytes memory. */
	void* Alloc(uint bytes);

	/** Frees memory by pointer. */
	void Free(void* ptr);

	/** Returns allocator identificator. */
	const char* GetName() const { return "linear alloc"; }
};

CLOSE_O2_NAMESPACE