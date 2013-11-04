#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <list>

#include "public.h"

OPEN_O2_NAMESPACE

class IAllocator;
class cMutex;

/** Memory manager, singleton. 
 ** Containg information about allocations, usage of memory, basic allocator if specified. */
class cMemoryManager
{
	friend class IAllocator;

	/** Allocation signature. */
	struct AllocSign
	{
		void*        mPtr;        /**< Pointer of allocated memory. */
		uint32       mSize;       /**< Size of allocated memory. */
		char         mSource[64]; /**< Source file name. */
		unsigned int mSourceLine; /**< Source line number. */
		IAllocator*  mAllocator;  /**< Used allocator. NULL if memory allocated by system. */

		/** ctor. */
		AllocSign(void* memPtr, uint32 size, const char* source, unsigned int sourceLine, IAllocator* allocator);

		/** copy ctor. */
		AllocSign(const AllocSign& allocSign);		

		/** operator=. */
		AllocSign& operator=(const AllocSign& allocSign);
	};
	typedef std::list<AllocSign> AllocSignsList;

	AllocSignsList* mAllocSigns;      /**< Allocations signatures. */
	uint32          mUsedMemory;      /**< Size of used and signed memory. */
	cMutex*         mAllocSignsMutex; /**< Mutex for signs. */

	static cMemoryManager mStaticObj;

public:
	IAllocator* mBasicAllocator;

	cMemoryManager();
	~cMemoryManager();

	static cMemoryManager& instance() { return mStaticObj; }

	static void dump();

	static void registAlloc(void* memPtr, uint32 size, const char* source, unsigned int sourceLine, IAllocator* allocator);
	static void unregistAlloc(void* memPtr);
};

CLOSE_O2_NAMESPACE

#endif //MEMORY_MANAGER_H