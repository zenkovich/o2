#pragma once

#include "allocator_interface.h"

OPEN_O2_NAMESPACE

class StdAllocator:public IAllocator
{
public:
	StdAllocator();
	~StdAllocator();

	void* Alloc(uint bytes);
	void* Realloc(void* ptr, uint bytes);
	void  Free(void* ptr);
	const char* GetName() const { return "std alloc"; }
};

CLOSE_O2_NAMESPACE