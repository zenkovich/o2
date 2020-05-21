#pragma once
#include "o2/Utils/Memory/Allocators/IAllocator.h"
#include "o2/Utils/Memory/Allocators/DefaultAllocator.h"

namespace o2
{
	class LinearAllocator: public IAllocator
	{
	public:
		LinearAllocator(size_t blockSize, IAllocator* baseAllocator = DefaultAllocator::GetInstance());

		void* Allocate(size_t size) override;
		void Deallocate(void* ptr) override;
		void* Reallocate(void* ptr, size_t oldSize, size_t newSize) override;

	private:
		IAllocator* mBaseAllocator;

		void* mBlock;
		size_t mBlockSize;
		size_t mCurrentSize;
	};
};