#include "test_allocator.h"

#include "util/threads/mutex.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

cTestAllocator::cTestAllocator( uint32 size, IAllocator* parentAllocator /*= NULL*/ ):
	mParentAllocator(parentAllocator), mMemorySize(size)
{
	if (parentAllocator)
	{
		mMemory = (char*)ALLOC(parentAllocator, size);
	}
	else
	{
		mMemory = (char*)malloc(size);
	}
	
	uint32 pp = 4456 + (1 << 31);
	memcpy(mMemory, &pp, sizeof(pp));
	//*(uint32*)mMemory = pp;

	putBlock(mMemory, mMemorySize - sizeof(uint32)*2, true);
}

cTestAllocator::~cTestAllocator()
{
	if (mParentAllocator)
	{
		FREE(mParentAllocator, mMemory);
	}
	else
	{
		free(mMemory);
	}
}

void* cTestAllocator::alloc( uint32 bytes )
{	
	void* res = NULL;
	char* ptr = mMemory;

	while (ptr < mMemory + mMemorySize)
	{
		uint32 side = *(uint32*)ptr;
		uint32 blockClear = side >> 31;
		uint32 blockSize = side << 1 >> 1;

		if (blockClear && blockSize >= bytes)
		{
			uint32 diff = blockSize - bytes;
			uint32 resBlockSize = bytes;
			if (diff > 8)
				putBlock((char*)ptr + bytes + sizeof(uint32)*2, blockSize - bytes - sizeof(uint32)*2, true);
			else
				resBlockSize += diff;

			res = putBlock(ptr, resBlockSize, false);
			break;
		}

		ptr += blockSize + sizeof(uint32)*2;
	}

	return res;
}

void* cTestAllocator::realloc( void* ptr, uint32 bytes )
{
	return NULL;
}

void cTestAllocator::free( void* ptr )
{
	if (!ptr)
		return;

	uint32 leftBlockClear;
	char* leftNearSidePtr = (char*)ptr - sizeof(uint32)*2;
	uint32 leftNearSide = *(uint32*)leftNearSidePtr;

	if (leftNearSidePtr < mMemory)
		leftBlockClear = 0;
	else
		leftBlockClear = leftNearSide >> 31;


	uint32 side = *(uint32*)((char*)ptr - sizeof(uint32));
	uint32 blockSize = side << 1 >> 1;


	uint32 rightBlockClear;
	char* rightNearSidePtr = (char*)ptr + blockSize + sizeof(uint32);
	uint32 rightNearSide = *(uint32*)rightNearSidePtr;
	if (rightNearSidePtr >= mMemory + mMemorySize)
		rightBlockClear = 0;
	else
		rightBlockClear = rightNearSide >> 31;


	char* newLeftSide = (char*)ptr - sizeof(uint32);
	uint32 newSize = blockSize;
	if (leftBlockClear)
	{
		uint32 leftBlockSize = leftNearSide << 1 >> 1;
		newLeftSide -= leftBlockSize + sizeof(uint32)*2;
		newSize += leftBlockSize + sizeof(uint32)*2;
	}

	if (rightBlockClear)
	{
		uint32 rightBlockSize = rightNearSide << 1 >> 1;
		newSize += rightBlockSize + sizeof(uint32)*2;
	}

	putBlock(newLeftSide, newSize, true);
}

void* cTestAllocator::putBlock( void* ptr, uint32 size, bool clear )
{
	uint32 side = size + ((uint32)clear << 31);
	*(uint32*)ptr = side;
	*(uint32*)((char*)ptr + size + sizeof(uint32)) = side;

	return (void*)((char*)ptr + sizeof(uint32));
}

void cTestAllocator::dump()
{	
	void* res = NULL;
	char* ptr = mMemory;

	while (ptr < mMemory + mMemorySize)
	{
		uint32 side = *(uint32*)ptr;
		uint32 blockClear = side >> 31;
		uint32 blockSize = side << 1 >> 1;

		printf("%s(%i)%s - ", (blockClear ? "O":"X"), blockSize, (blockClear ? "0":"X"));

		ptr += blockSize + sizeof(uint32)*2;
	}
	printf("\n");
}

CLOSE_O2_NAMESPACE