#pragma once
#include "o2/Utils/Memory/Allocators/IAllocator.h"
#include "o2/Utils/Memory/Allocators/DefaultAllocator.h"

namespace o2
{
	class ChunkPoolAllocator: public IAllocator
	{
	public:
		ChunkPoolAllocator(size_t chunkSize = 64*1024, IAllocator* baseAllocator = DefaultAllocator::GetInstance());
		ChunkPoolAllocator(ChunkPoolAllocator& other);
		~ChunkPoolAllocator() override;

		ChunkPoolAllocator& operator=(ChunkPoolAllocator& other);

		void* Allocate(size_t size) override;
		void Deallocate(void* ptr) override;
		void* Reallocate(void* ptr, size_t oldSize, size_t newSize) override;

		void Clear();

	private:
		struct Chunk
		{
			void*  ptr;
			size_t currentSize;
			size_t capacity;

			Chunk* prev;
		};

	private:
		IAllocator* mBaseAllocator;

		size_t mChunkSize;
		Chunk* mHead = nullptr;

	private:
		void AddChunk(size_t capacity);
	};
};
