#pragma once
#include "o2/Utils/Memory/Allocators/IAllocator.h"

namespace o2
{
    class DefaultAllocator: public IAllocator
    {
    public:
        void* Allocate(size_t size) override;
        void Deallocate(void* ptr) override;
        void* Reallocate(void* ptr, size_t oldSize, size_t newSize) override;

    public:
        static DefaultAllocator* GetInstance();
    };
};
