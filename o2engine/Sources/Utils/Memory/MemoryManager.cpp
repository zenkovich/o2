#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"

void* operator new(size_t size, const char* location, int line)
{
	void* allocMemory = ::operator new(size + sizeof(void*));
	void* object = (char*)allocMemory + sizeof(void*);
	o2::ObjectInfo* info = new o2::ObjectInfo();
	allocMemory = info;
	o2::MemoryManager::OnObjectCreating(object, info, size, location, line);
	return object;
}

void operator delete(void* obj, const char* location, int line)
{
	o2::MemoryManager::OnObjectDestroying((o2::IObject*)obj);
	::operator delete(obj);
}

namespace o2
{
	void MemoryManager::OnObjectCreating(void* object, ObjectInfo* info, UInt size, const char* srcFile, int srcFileLine)
	{
		ObjectInfo* info = new ObjectInfo();
		objectPtr->mObjectInfo = info;

		info->mObjectPtr = objectPtr;

		info->mSize = size;
		info->mMark = mInstance->mCurrentMark;

		strncpy(info->mAllocSrcFile, srcFile, 127);
		info->mAllocSrcFileLine = srcFileLine;

		mInstance->mObjectsInfos.Add(info);
	}

	void MemoryManager::OnObjectDestroying(void* object)
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
			printf("Leaked object: %x %s:%i\n", obj, obj->mAllocSrcFile, obj->mAllocSrcFileLine);
			delete obj;
		}
	}

	ObjectInfo* MemoryManager::GetObjectInfo(void* object)
	{
		return (ObjectInfo*)((char*)object - sizeof(void*));
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