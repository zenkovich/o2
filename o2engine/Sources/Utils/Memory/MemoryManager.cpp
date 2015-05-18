#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"

namespace o2
{
	void MemoryManager::OnObjectCreating(IObject* objectPtr, UInt size, bool managed, const char* srcFile, int srcFileLine)
	{

		ObjectInfo* info = new ObjectInfo();
		mInstance->mObjectsInfos.Add(info);
		objectPtr->mObjectInfo = info;

		info->mObjectPtr = objectPtr;
		info->mSize = size;
		info->mManaged = managed;

#ifdef MEM_TRACE
		strncpy(info->mAllocSrcFile, srcFile, 127);
		info->mAllocSrcFileLine = srcFileLine;
#else
		info->mAllocSrcFile[0] = '\0';
		info->mAllocSrcFileLine = 0;
#endif

		info->mMark = mInstance->mCurrentMark;
	}

	void MemoryManager::OnObjectDestroying(IObject* object)
	{
		mInstance->mObjectsInfos.Remove(object->mObjectInfo);
		delete object->mObjectInfo;
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
			ptr->mObject->mObjectInfo->Mark(mInstance->mCurrentMark);

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

	void MemoryManager::ObjectInfo::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
			ptr->mObject->mObjectInfo->Mark(mark);
	}

}