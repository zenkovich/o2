#include "o2/stdafx.h"
#include "ChunkPoolAllocator.h"

namespace o2
{

	ChunkPoolAllocator::ChunkPoolAllocator(size_t chunkSize /*= 64*1024*/, IAllocator* baseAllocator /*= DefaultAllocator::GetInstance()*/)
	{
		mBaseAllocator = baseAllocator;
		mChunkSize = chunkSize;

		AddChunk();
	}

	ChunkPoolAllocator::~ChunkPoolAllocator()
	{
		Clear();
	}

	void ChunkPoolAllocator::AddChunk()
	{
		void* mem = mBaseAllocator->Allocate(mChunkSize + sizeof(Chunk));
		Chunk* nextChunk = new (mem) Chunk();
		nextChunk->ptr = reinterpret_cast<std::byte*>(mem) + sizeof(Chunk);
		nextChunk->currentSize = 0;
		nextChunk->capacity = mChunkSize;
		nextChunk->prev = mHead;

		mHead = nextChunk;
	}

	void* ChunkPoolAllocator::Allocate(size_t size)
	{
		if (mHead->currentSize + size > mHead->capacity)
			AddChunk();

		void* res = reinterpret_cast<std::byte*>(mHead->ptr) + mHead->currentSize;
		mHead->currentSize += size;
		return res;
	}

	void ChunkPoolAllocator::Deallocate(void* ptr)
	{}

	void* ChunkPoolAllocator::Reallocate(void* ptr, size_t oldSize, size_t newSize)
	{
		void* newMem = Allocate(newSize);
		memcpy(newMem, ptr, oldSize);
		return newMem;
	}

	void ChunkPoolAllocator::Clear()
	{
		while (mHead)
		{
			Chunk* chunk = mHead;
			mHead = chunk->prev;
			mBaseAllocator->Deallocate(chunk);
		}
	}

}