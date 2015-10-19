#pragma once

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"
#include "Utils/String.h"

// Memory manager access macros
#define o2Memory (*MemoryManager::instance)

void* operator new(size_t size);
void* operator new(size_t size, const char* location, int line);
void  operator delete(void* allocMemory);
void  operator delete(void* allocMemory, const char* location, int line);

namespace o2
{
	class IPtr;
	class LogStream;

	// ---------------------------------------------
	// Object information, using for memory managing
	// ---------------------------------------------
	struct AllocObjectInfo
	{
		typedef Vector<IPtr*> PointersVec;

		void*       mObjectPtr;         // Object pointer
		UInt        mSize;              // Size of object in bytes
		bool        mMark;              // Current mark. For Garbage Collector
		PointersVec mChildPointers;     // Child pointers, using for GC
		PointersVec mPointers;          // Object pointers, using for GC
		char        mAllocSrcFile[128]; // Allocation source file name
		int         mAllocSrcFileLine;  // Number of line, where object was allocated in source file

		// Marks this object and all children
		void Mark(bool mark);
	};


	// ------------------------------------------------------------------
	// Memory manager, using for collecting garbage, tracing memory leaks
	// ------------------------------------------------------------------
	class MemoryManager
	{
		typedef Vector<IPtr*>       PointersVec;
		typedef Vector<AllocObjectInfo*> ObjectsInfosVec;

	public:
		static MemoryManager* instance; // Instance pointer

	public:
		// Collects all unused objects and destroys them
		static void CollectGarbage();

	protected:
		ObjectsInfosVec mObjectsInfos;  // All static objects infos
		PointersVec     mPointers;      // All pointers
		bool            mCurrentGCMark; // Current Garbage collection mark

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
		void FreeObjects(const ObjectsInfosVec& objectsVec);

		// Prints objects infos
		void PrintObjectsInfos(const ObjectsInfosVec& objectsVec);

		friend class IPtr;

		template<typename _type>
		friend class ITemplPtr;

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory);
	};
}

#define mnew new(__FILE__, __LINE__)