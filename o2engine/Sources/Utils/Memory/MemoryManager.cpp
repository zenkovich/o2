#include "MemoryManager.h"

#include "Utils/Memory/IPtr.h"

void* operator new(size_t size, const char* location, int line)
{
	void* allocMemory = ::operator new(size + sizeof(o2::ObjectInfo*));
	void* object = (char*)allocMemory + sizeof(o2::ObjectInfo*);
	o2::ObjectInfo* info = new o2::ObjectInfo();
	o2::ObjectInfo** ff = &((o2::ObjectInfo*)allocMemory);
	*ff = info;
	o2::MemoryManager::OnObjectCreating(object, info, size, location, line);
	return object;
}

void operator delete(void* obj, const char* location, int line)
{
	o2::MemoryManager::OnObjectDestroying(obj);
	::operator delete(o2::MemoryManager::GetObjectInfo(obj));
	::operator delete((char*)obj - sizeof(o2::ObjectInfo*));
}

namespace o2
{
	void MemoryManager::OnObjectCreating(void* object, ObjectInfo* info, UInt size, const char* srcFile, int srcFileLine)
	{
		info->mObjectPtr = object;
		info->mSize = size;
		info->mMark = mInstance->mCurrentGCMark;
		info->mAllocSrcFileLine = srcFileLine;
		strncpy(info->mAllocSrcFile, srcFile, 127);

		mInstance->mObjectsInfos.Add(info);
	}

	void MemoryManager::OnObjectDestroying(void* object)
	{
		mInstance->mObjectsInfos.Remove(GetObjectInfo(object));
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
		mInstance->mCurrentGCMark = !mInstance->mCurrentGCMark;

		for (auto ptr : mInstance->mPointers)
		{
			ptr->mObjectInfo->Mark(mInstance->mCurrentGCMark);
		}

		ObjectsInfosArr freeObjects;

		for (auto obj : mInstance->mObjectsInfos)
		{
			if (obj->mMark != mInstance->mCurrentGCMark)
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
		return (ObjectInfo*)((char*)object - sizeof(o2::ObjectInfo*));
	}

	void ObjectInfo::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
		{
			if (ptr->mObjectInfo->mMark != mark)
				ptr->mObjectInfo->Mark(mark);
		}
	}
}