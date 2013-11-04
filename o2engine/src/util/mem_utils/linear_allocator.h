#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class cMutex;

/** Linear allocator. */
class cLinearAllocator:public IAllocator
{
	IAllocator* mParentAllocator; /**< Parent allocator. This allocator allocates memory from parent allocator. 
								    *  If parent allocator not specified, using system allocator. */

	char*       mMemory;          /**< Memory ptr. */
	uint32      mMemorySize;      /**< Size of memory. */
	uint32      mUsedMemory;      /**< Size of used memory. */
	cMutex*     mMutex;           /**< Mutex. */

public:
	/** ctor. */
	cLinearAllocator(uint32 size, IAllocator* parentAllocator = NULL);

	/** dtor. */
	~cLinearAllocator();

	/** Allocates bytes memory. */
	void* alloc(uint32 bytes);

	/** Frees memory by pointer. */
	void free(void* ptr);

	/** Returns allocator identificator. */
	const char* getName() const { return "linear alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //LINEAR_ALLOCATOR_H