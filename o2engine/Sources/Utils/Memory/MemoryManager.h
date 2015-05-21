#pragma once

#include "Utils/Singleton.h"
#include "Utils/Containers/Array.h"
#include "Utils/Memory/IPtr.h"
#include "Utils/Memory/AllocOperators.h"
#include "Utils/Memory/ClassFieldInfo.h"
#include "EngineSettings.h"

namespace o2
{
	typedef Array<IPtr*> PointersArr;
	struct ObjectInfo
	{
		IObject*    mObjectPtr;
		PointersArr mPointers;
		PointersArr mChildPointers;
		UInt        mSize;
		bool        mMark;
		char        mAllocSrcFile[128];
		int         mAllocSrcFileLine;

		void Mark(bool mark);
	};

	class MemoryManager : public Singleton<MemoryManager>
	{
		friend class IPtr;
		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* obj, const char* location, int line);

		typedef Array<IPtr*> PointersArr;

	public:
		typedef Array<ObjectInfo*> ObjectsInfosArr;

	protected:
		ObjectsInfosArr mObjectsInfos;
		PointersArr     mPointers;
		bool            mCurrentMark;

	protected:
		static void OnObjectCreating(IObject* objectPtr, UInt size, const char* srcFile, int srcFileLine);
		static void OnObjectDestroying(IObject* objectPtr);

		static void OnPtrCreating(IPtr* ptr);
		static void OnPtrDestroying(IPtr* ptr);

	public:
		static void CollectGarbage(bool checkLeaks = false);
	};
}