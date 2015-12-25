#pragma once

#include <list>
#include "public.h"

OPEN_O2_NAMESPACE

class IAllocator;
class Mutex;

/** Memory manager, singleton.
 ** Contains information about allocations, usage of memory, basic allocator if specified. */
class MemoryManager
{
	friend class IAllocator;

	/** Allocation signature. */
	struct AllocSign
	{
		void*        mPtr;        /**< Pointer of allocated memory. */
		uint         mSize;       /**< Size of allocated memory. */
		char         mSource[64]; /**< Source file name. */
		unsigned int mSourceLine; /**< Source line number. */
		IAllocator*  mAllocator;  /**< Used allocator. NULL if memory allocated by system. */

		/** ctor. */
		AllocSign(void* memPtr, uint size, const char* source, unsigned int sourceLine, IAllocator* allocator);

		/** copy ctor. */
		AllocSign(const AllocSign& allocSign);

		/** operator=. */
		AllocSign& operator=(const AllocSign& allocSign);
	};
	typedef std::list<AllocSign> AllocSignsList;

	AllocSignsList* mAllocSigns;      /**< Allocations signatures. */
	uint            mUsedMemory;      /**< Size of used and signed memory. */
	Mutex*          mAllocSignsMutex; /**< Mutex for signs. */

	static MemoryManager mStaticObj;

public:
	IAllocator* mBasicAllocator;

	MemoryManager();
	~MemoryManager();

	static MemoryManager& Instance() { return mStaticObj; }

	static void Dump();

	static void RegistAllocation(void* memPtr, uint size, const char* source, unsigned int sourceLine, IAllocator* allocator);
	static void UnregistAllocation(void* memPtr);
};

CLOSE_O2_NAMESPACE