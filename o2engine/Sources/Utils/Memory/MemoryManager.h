#pragma once

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"

void* operator new(size_t size, const char* location, int line);
void  operator delete(void* allocMemory, const char* location, int line);

namespace o2
{
	class IPtr;

	/** Object info. Contains pointer to object, child objects pointers and allocation source. */
	struct ObjectInfo
	{
		typedef Vector<IPtr*> PointersArr;

		void*       mObjectPtr;         /** Object pointer. */
		PointersArr mPointers;          /** Pointers to that object. */
		PointersArr mChildPointers;     /** Child objects pointers array. */
		UInt        mSize;              /** Size of object in bytes. */
		bool        mMark;              /** Current mark. For Garbage Collector. */
		char        mAllocSrcFile[128]; /** Allocation source file name. */
		int         mAllocSrcFileLine;  /** Number of line, where object was allocated in source file. */

		/** Sets mark for this object and for his children. */
		void Mark(bool mark);
	};

	/** Memory manager. Storing information about all allocated objects, looks for memory leaks and collecting garbage. */
	class MemoryManager
	{
		friend class IPtr;

		template<typename _type>
		friend class ITemplPtr;

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory, const char* location, int line);

		typedef Vector<IPtr*> PointersArr;

	public:
		typedef Vector<ObjectInfo*> ObjectsInfosArr;

		static MemoryManager* mInstance;      /** Instance pointer. */

	protected:
		ObjectsInfosArr       mObjectsInfos;  /** All static objects infos. */
		PointersArr           mPointers;      /** All pointers. */
		bool                  mCurrentGCMark; /** Current Garbage collection mark. */

	protected:
		/** Calling when object created. */
		static void OnObjectCreating(void* object, ObjectInfo* info, UInt size, const char* srcFile, int srcFileLine);

		/** Calling when objects destroying. */
		static void OnObjectDestroying(void* object);

		/** Calling when pointer creating. */
		static void OnPtrCreating(IPtr* ptr);

		/** Calling when pointer destroying. */
		static void OnPtrDestroying(IPtr* ptr);

		static ObjectInfo* GetObjectInfo(void* object);

	public:
		/** Collects all unused objects and destroys them. */
		static void CollectGarbage();
	};
}

#define mnew new(__FILE__, __LINE__)