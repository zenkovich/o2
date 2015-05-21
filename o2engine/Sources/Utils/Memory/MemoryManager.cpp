#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"

namespace o2
{
	void MemoryManager::OnObjectCreating(IObject* objectPtr, UInt size, const char* srcFile, int srcFileLine)
	{
		ObjectInfo* info = new ObjectInfo();
		objectPtr->mObjectInfo = info;

		info->mObjectPtr = objectPtr;

		info->mSize = size;
		info->mMark = mInstance->mCurrentMark;

#ifdef MEM_TRACE
		strncpy(info->mAllocSrcFile, srcFile, 127);
		info->mAllocSrcFileLine = srcFileLine;
#else
		info->mAllocSrcFile[0] = '\0';
		info->mAllocSrcFileLine = 0;
#endif

		mInstance->mObjectsInfos.Add(info);
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

	void MemoryManager::CollectGarbage(bool checkLeaks /*= false*/)
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
			if (!checkLeaks)
			{
				delete obj;
			}
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