#pragma once
#include "o2/Utils/Memory/Allocators/DefaultAllocator.h"
#include "o2/Utils/Memory/Allocators/IAllocator.h"
#include "o2/Utils/Debug/Assert.h"
#include "o2/Utils/Math/Math.h"
#include <cstddef>

namespace o2
{
    class StackAllocator
    {
    public:
        StackAllocator(size_t capacity, IAllocator* baseAllocator = DefaultAllocator::GetInstance());

        ~StackAllocator();

        template<typename T>
        T* Push(int count = 1);

        template<typename T>
        T* Pop(int count = 1);

        template<typename T>
        T* Top();

        void Clear();

    protected:
        IAllocator* mBaseAllocator;

        size_t mInitialCapacity;

        std::byte* mStack;
        std::byte* mStackEnd;
        std::byte* mTop;

    protected:
        void Initialize();
    };

    template<typename T>
    T* StackAllocator::Pop(int count)
    {
        size_t size = sizeof(T)*count;
        Assert(mTop - size >= mStack, "Too much popdata size");
        mTop -= size;
        return (T*)mTop;
    }

    template<typename T>
    T* StackAllocator::Top()
    {
        return (T*)(mTop - sizeof(T));
    }

    template<typename T>
    T* StackAllocator::Push(int count)
    {
        if (!mStack)
            Initialize();

        size_t pushSize = sizeof(T)*count;
        if (mTop + pushSize > mStackEnd)
        {
            size_t size = mTop - mStack;
            size_t capacity = mStackEnd - mStack;
            size_t newCapacity = Math::Max(size + pushSize, capacity + capacity/2);
            mStack = (std::byte*)mBaseAllocator->Reallocate(mStack, capacity, newCapacity);
            mStackEnd = mStack + newCapacity;
            mTop = mStack + size;
        }

        T* res = (T*)mTop;
        mTop += pushSize;

        return res;
    }

}
