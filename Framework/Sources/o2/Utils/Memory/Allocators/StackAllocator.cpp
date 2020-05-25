#include "o2/stdafx.h"
#include "StackAllocator.h"

namespace o2
{
	StackAllocator::StackAllocator(size_t capacity, IAllocator* baseAllocator /*= DefaultAllocator::GetInstance()*/):
		mBaseAllocator(baseAllocator)
	{
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
		Initialize();
	}

}