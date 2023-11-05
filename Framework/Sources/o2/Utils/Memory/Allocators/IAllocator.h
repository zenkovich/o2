#pragma once
#include <stddef.h>

namespace o2
{
    class IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        virtual void* Allocate(size_t size) = 0;
        virtual void Deallocate(void* ptr) = 0;
        virtual void* Reallocate(void* ptr, size_t oldSize, size_t newSize) = 0;
    };
};
