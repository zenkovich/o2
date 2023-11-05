#include "o2/stdafx.h"
#include "StackAllocator.h"

namespace o2
{
    StackAllocator::StackAllocator(size_t capacity, IAllocator* baseAllocator /*= DefaultAllocator::GetInstance()*/):
        mBaseAllocator(baseAllocator)
    {
        mInitialCapacity = capacity;
        Initialize();
    }

    StackAllocator::~StackAllocator()
    {
        Clear();
    }

    void StackAllocator::Clear()
    {
        mBaseAllocator->Deallocate(mStack);
        mStack = nullptr;
        mStackEnd = nullptr;
        mTop = nullptr;
    }

    void StackAllocator::Initialize()
    {
        mStack = (std::byte*)mBaseAllocator->Allocate(mInitialCapacity);
        mTop = mStack;
        mStackEnd = mStack + mInitialCapacity;
    }

}