#pragma once

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"

void* operator new(size_t size, const char* location, int line);
void  operator delete(void* allocMemory);
void  operator delete(void* allocMemory, const char* location, int line);

namespace o2
{
	class IPtr;
	class LogStream;

	/** Object info. Contains pointer to object, child objects pointers and allocation source. */
	struct ObjectInfo
	{
		typedef Vector<IPtr*> PointersVec;

		void*       mObjectPtr;         /** Object pointer. */
		UInt        mSize;              /** Size of object in bytes. */
		bool        mMark;              /** Current mark. For Garbage Collector. */
		PointersVec mChildPointers;     /** Child pointers, using for GC. */
		PointersVec mPointers;          /** Object pointers, using for GC. */
		char        mAllocSrcFile[128]; /** Allocation source file name. */
		int         mAllocSrcFileLine;  /** Number of line, where object was allocated in source file. */

		void Mark(bool mark);
	};

	/** Memory manager. Storing information about all allocated objects, looks for memory leaks and collecting garbage. */
	class MemoryManager
	{
		friend class IPtr;

		template<typename _type>
		friend class ITemplPtr;

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory);

		struct GCObjectInfo;

		typedef Vector<IPtr*>       PointersVec;
		typedef Vector<ObjectInfo*> ObjectsInfosVec;

	public:
		static MemoryManager* mInstance; /** Instance pointer. */

	protected:
		ObjectsInfosVec mObjectsInfos;   /** All static objects infos. */
		PointersVec     mPointers;       /** All pointers. */
		bool            mCurrentGCMark;  /** Current Garbage collection mark. */

	protected:
		/** Calling when object created. */
		static void OnObjectCreating(void* object, ObjectInfo* info, UInt size, const char* srcFile, int srcFileLine);

		/** Calling when objects destroying. */
		static void OnObjectDestroying(void* object);

		/** Calling when pointer creating. */
		static void OnPtrCreating(IPtr* ptr);

		/** Calling when pointer destroying. */
		static void OnPtrDestroying(IPtr* ptr);

		/** Returns object info. */
		static ObjectInfo* GetObjectInfo(void* object);

	public:
		/** Collects all unused objects and destroys them. */
		static void CollectGarbage();

	private:
		/** Resets memory tree for all pointers and objects. */
		void ResetMemoryTree();

		/** Builds actual memory tree. */
		void RebuildMemoryTree();

		/** Searching free object by memory tree. */
		void FindFreeObjects(ObjectsInfosVec& result);

		/** Frees objects. */
		void FreeObjects(const ObjectsInfosVec& objectsVec);

		/** Prints objects infos. */
		void PrintObjectsInfos(const ObjectsInfosVec& objectsVec);
	};
}

#define mnew new(__FILE__, __LINE__)