#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"

namespace o2
{
	void MemoryManager::OnObjectCreating(IObject* objectPtr, UInt size, bool managed, const char* srcFile, int srcFileLine)
	{
#if defined(MEM_TRACE) || defined(MEM_LEAKS_CHECK)

		ObjectInfo* info = new ObjectInfo();
		objectPtr->mObjectInfo = info;

		info->mObjectPtr = objectPtr;

#ifdef MEM_LEAKS_CHECK
		info->mSize = size;
		info->mManaged = managed;
		info->mMark = mInstance->mCurrentMark;
		mInstance->mObjectsInfos.Add(info);
#else
		if (managed)
		{
			info->mSize = size;
			info->mManaged = managed;
			info->mMark = mInstance->mCurrentMark;
			mInstance->mObjectsInfos.Add(info);
		}
		else
		{
			info->mSize = 0;
			info->mManaged = false;
		}
#endif

#ifdef MEM_TRACE
		strncpy(info->mAllocSrcFile, srcFile, 127);
		info->mAllocSrcFileLine = srcFileLine;
#else
		info->mAllocSrcFile[0] = '\0';
		info->mAllocSrcFileLine = 0;
#endif

#else
		objectPtr->mObjectInfo = nullptr;
#endif //defined(MEM_TRACE) || defined(MEM_LEAKS_CHECK)
	}

	void MemoryManager::OnObjectDestroying(IObject* object)
	{
		if (object->mObjectInfo)
		{
			mInstance->mObjectsInfos.Remove(object->mObjectInfo);
			delete object->mObjectInfo;
		}
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		char* cptr = (char*)ptr;
		for (auto obj : mInstance->mObjectsInfos)
		{
			char* beg = (char*)obj->mObjectPtr;
			char* end = beg + obj->mSize;
			if (cptr >= beg && cptr < end)
			{
				obj->mChildPointers.Add(ptr);
				return;
			}
		}

		mInstance->mPointers.Add(ptr);
		ptr->mStatic = true;
	}

	void MemoryManager::OnPtrDestroying(IPtr* ptr)
	{
		mInstance->mPointers.Remove(ptr);
	}

	void MemoryManager::CollectGarbage()
	{
		mInstance->mCurrentMark = !mInstance->mCurrentMark;

		for (auto ptr : mInstance->mPointers)
		{
			if (ptr->mObject)
				ptr->mObject->mObjectInfo->Mark(mInstance->mCurrentMark);
		}

		ObjectsInfosArr freeObjects;

		for (auto obj : mInstance->mObjectsInfos)
		{
			if (obj->mMark != mInstance->mCurrentMark)
				freeObjects.Add(obj);
		}

		for (auto obj : freeObjects)
		{
			if (obj->mManaged)
				delete obj;
			else
				printf("Leaked object: %x %s:%i\n", obj, obj->mAllocSrcFile, obj->mAllocSrcFileLine);
		}
	}

	void ObjectInfo::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
		{
			if (ptr->mObject->mObjectInfo->mMark != mark)
				ptr->mObject->mObjectInfo->Mark(mark);
		}
	}
}