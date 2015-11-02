#pragma once

#include <vector>

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"

// Memory manager access macros
#define o2Memory (*MemoryManager::instance)

void* operator new(size_t size, const char* location, int line);
void  operator delete(void* allocMemory);
void  operator delete(void* allocMemory, const char* location, int line);

void* _mmalloc(size_t size, const char* location, int line);
void mfree(void* ptr);

namespace o2
{
	class IPtr;
	class LogStream;

	typedef std::vector<IPtr*> PointersVec;

	// ---------------------------------------------
	// Object information, using for memory managing
	// ---------------------------------------------
	struct AllocObjectInfo
	{
		void*       mObjectPtr;         // Object pointer
		UInt        mSize;              // Size of object in bytes
		bool        mMark;              // Current mark. For Garbage Collector
		PointersVec mChildPointers;     // Child pointers, using for GC
		PointersVec mPointers;          // Object pointers, using for GC
		char        mAllocSrcFile[128]; // Allocation source file name
		int         mAllocSrcFileLine;  // Number of line, where object was allocated in source file
		int         mIdx;
		int         mId;

		// Constructor
		AllocObjectInfo() {}

		// Copy-constructor
		AllocObjectInfo(const AllocObjectInfo& other);

		// Copy-operator
		AllocObjectInfo& operator=(const AllocObjectInfo& other);

		// Marks this object and all children
		void Mark(bool mark);
	};
	typedef std::vector<AllocObjectInfo*> ObjectsInfosVec;


	// ------------------------------------------------------------------
	// Memory manager, using for collecting garbage, tracing memory leaks
	// ------------------------------------------------------------------
	class MemoryManager
	{
	public:
		static MemoryManager* instance; // Instance pointer

	public:
		// Constructor
		MemoryManager();

		// Destructor
		~MemoryManager();

		// Collects all unused objects and destroys them
		static void CollectGarbage();

	protected:
		const int mInitialObjectInfosCount    = 10000;
		const int mInitialPointersCount       = 10000;
		const int mObjectInfosExpandStepCount = 5000;
		const int mPointersExpandStepCount    = 5000;

		bool             mCurrentGCMark;           // Current Garbage collection mark

		AllocObjectInfo* mAllocObjsInfos;          // Array of allocated object infos
		int              mAllocObjsCount;          // Allocated objects infos pool size
		int*             mFreeAllocObjsInfos;      // Free allocated object infos indexes
		int              mFreeAllocObjsInfosCount; // Free allocated object infos count

		IPtr**           mPointers;                // All pointers to pointers
		int              mPointersCount;           // Pointers pool size
		int*             mFreePointers;            // Free pointers indexes
		int              mFreePointersCount;       // Free pointers count

		PointersVec      mRootPointers;            // Root pointers (actual when collecting garbage)

	protected:
		// Calling when object created
		static void OnObjectCreating(void* object, UInt size, const char* srcFile, int srcFileLine);

		// Calling when objects destroying
		static void OnObjectDestroying(void* object);

		// Calling when pointer creating
		static void OnPtrCreating(IPtr* ptr);

		// Calling when pointer destroying
		static void OnPtrDestroying(IPtr* ptr);

		// Returns object info
		static AllocObjectInfo* GetObjectInfo(void* object);

	private:
		// Resets memory tree for all pointers and objects
		void ResetMemoryTree();

		// Builds actual memory tree
		void RebuildMemoryTree();

		// Searching free object by memory tree
		void FindFreeObjects(ObjectsInfosVec& result);

		// Frees objects
		void FreeObjects(const ObjectsInfosVec& infos);

		// Prints objects infos
		void PrintObjectsInfos(const ObjectsInfosVec& infos);


		friend class IPtr;

		template<typename _type>
		friend class ITemplPtr;

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory);
		friend void* ::_mmalloc(size_t size, const char* location, int line);
		friend void ::mfree(void* ptr);
	};
}

#define mnew new(__FILE__, __LINE__)
#define mmalloc(sz) _mmalloc(sz, __FILE__, __LINE__)