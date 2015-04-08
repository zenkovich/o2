#pragma once

#include "util/public_namespace.h"
#include "util/public_types.h"

/* Define overloaded new/delete operators if basic memory allocator was specified. */
#ifdef BASIC_MEMORY_ALLOCATOR 

void* operator new(uint size);
void* operator new[](uint size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif //BASIC_MEMORY_ALLOCATOR

/* Define overloaded and tracing new/delete operators if memory trace enabled. */
#ifdef MEMORY_TRACE

void* operator new(uint size, const char* location, int line);
void* operator new[](uint size, const char* location, int line);

void operator delete(void* ptr, const char* location, int line);
void operator delete[](void* ptr, const char* location, int line);

/* Basic engine allocation command, what tracing source of allocation. */
#define mnew new (__FILE__, __LINE__) 

/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
#define ALLOC(allocator, size) allocator->AllocSrc(size, __FILE__, __LINE__); 

/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
#define REALLOC(allocator, mem_ptr, size) allocator->reallocs(mem_ptr, size, __FILE__, __LINE__); 

/** Frees memory from allocator with specified size. Tracing location, if enabled. */
#define FREE(allocator, mem_ptr) allocator->Frees(mem_ptr); 

#else

/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
#define mnew new

/** Alloc memory from allocator with specified size. Tracing location, if enabled. */
#define ALLOC(allocator, size) allocator->Alloc(size); 

/** Realloc memory from allocator with specified size. Tracing location, if enabled. */
#define REALLOC(allocator, mem_ptr, size) allocator->Realloc(mem_ptr, size); 

/** Frees memory from allocator with specified size. Tracing location, if enabled. */
#define FREE(allocator, mem_ptr) allocator->Free(mem_ptr); 

#endif //MEMORY_TRACE

OPEN_O2_NAMESPACE

#define SafeRelease(obj)       \
{                              \
if (obj)                       \
	delete _safe_release(obj); \
	obj = NULL;                \
}

#define SafeReleaseArr(obj) \
{                           \
if (obj != 0)               \
	delete[] obj;           \
	obj = NULL;             \
}

template<typename T>
T* _safe_release(T* object)
{
	return object;
}

CLOSE_O2_NAMESPACE