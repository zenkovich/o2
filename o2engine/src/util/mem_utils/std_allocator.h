#ifndef STD_ALLOCATOR_H
#define STD_ALLOCATOR_H

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class cStdAllocator:public IAllocator
{
public:
	cStdAllocator();
	~cStdAllocator();
	
	void* alloc(uint32 bytes);
	void* realloc(void* ptr, uint32 bytes);
	void free(void* ptr);
	const char* getName() const { return "std alloc"; }
};

CLOSE_O2_NAMESPACE

#endif //STD_ALLOCATOR_H