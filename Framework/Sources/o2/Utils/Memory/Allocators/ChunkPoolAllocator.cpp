#include "o2/stdafx.h"
#include "ChunkPoolAllocator.h"

namespace o2
{

	ChunkPoolAllocator::ChunkPoolAllocator(size_t chunkSize /*= 64*1024*/, 
										   IAllocator* baseAllocator /*= DefaultAllocator::GetInstance()*/)
	{
		mBaseAllocator = baseAllocator;
		mChunkSize = chunkSize;

		AddChunk(mChunkSize);
	}

	ChunkPoolAllocator::ChunkPoolAllocator(ChunkPoolAllocator& other):
		mBaseAllocator(other.mBaseAllocator), mChunkSize(other.mChunkSize), mHead(other.mHead)
	{
		other.mHead = nullptr;
	}

	ChunkPoolAllocator::~ChunkPoolAllocator()
	{
		Clear();
	}

	ChunkPoolAllocator& ChunkPoolAllocator::operator=(ChunkPoolAllocator& other)
	{
		mBaseAllocator = other.mBaseAllocator;
		mChunkSize = other.mChunkSize;
		mHead = other.mHead;

		other.mHead = nullptr;

		return *this;
	}

	void ChunkPoolAllocator::AddChunk(size_t capacity)
	{
		void* mem = mBaseAllocator->Allocate(capacity + sizeof(Chunk));
		Chunk* nextChunk = new (mem) Chunk();
		nextChunk->ptr = reinterpret_cast<std::byte*>(mem) + sizeof(Chunk);
		nextChunk->currentSize = 0;
		nextChunk->capacity = capacity;
		nextChunk->prev = mHead;

		mHead = nextChunk;
	}

	void* ChunkPoolAllocator::Allocate(size_t size)
	{
		if (!mHead || mHead->currentSize + size > mHead->capacity)
			AddChunk(Math::Max(size, mChunkSize));

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