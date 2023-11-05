#include "o2/stdafx.h"
#include "DefaultAllocator.h"

namespace o2
{
    void* DefaultAllocator::Allocate(size_t size)
    {
        return malloc(size);
    }

    void DefaultAllocator::Deallocate(void* ptr)
    {
        free(ptr);
    }

    void* DefaultAllocator::Reallocate(void* ptr, size_t oldSize, size_t newSize)
    {
        return realloc(ptr, newSize);
    }

    DefaultAllocator* DefaultAllocator::GetInstance()
    {
        static DefaultAllocator allocator;
        return &allocator;
    }

}