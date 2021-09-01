#include "o2/stdafx.h"
#include "LinearAllocator.h"
#include <string.h>
#include <cstddef>

namespace o2
{
	LinearAllocator::LinearAllocator(size_t blockSize, IAllocator* baseAllocator /*= DefaultAllocator::GetInstance()*/)
	{
		mBaseAllocator = baseAllocator;
		mBlock = mBaseAllocator->Allocate(blockSize);
		mBlockSize = blockSize;
		mCurrentSize = 0;
	}

	void* LinearAllocator::Reallocate(void* ptr, size_t oldSize, size_t newSize)
	{
		void* newMemory = Allocate(newSize);
		memcpy(newMemory, ptr, oldSize);
		return newMemory;
	}

	void LinearAllocator::Deallocate(void* ptr)
	{}

	void* LinearAllocator::Allocate(size_t size)
	{
		if (mCurrentSize + size > mBlockSize)
		{
			size_t newSize = mCurrentSize > 1024*1024 ? mCurrentSize + 512*1024 : mCurrentSize*2;
			mBlock = mBaseAllocator->Reallocate(mBlock, mCurrentSize, newSize);
			mBlockSize = newSize;
		}

		void* ptr = reinterpret_cast<std::byte*>(mBlock) + size;
		mCurrentSize += size;

		return ptr;
	}
}
