#include "MemoryManager.h"

#include "Utils/Assert.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Memory/IPtr.h"

void* operator new(size_t size, const char* location, int line)
{
	void* allocMemory = malloc(size + sizeof(o2::AllocObjectInfo*));
	void* object = (char*)allocMemory + sizeof(o2::AllocObjectInfo*);
	o2::AllocObjectInfo* info = new o2::AllocObjectInfo();
	*(o2::AllocObjectInfo**)allocMemory = info;
	o2::MemoryManager::OnObjectCreating(object, info, size, location, line);
	return object;
}

void operator delete(void* allocMemory)
{
	o2::MemoryManager::OnObjectDestroying(allocMemory);
}

void operator delete(void* allocMemory, const char* location, int line)
{
	o2::ErrorMessage("Used overrided delete operator!", location, line);
	_asm { int 3 };
}

namespace o2
{
	void MemoryManager::OnObjectCreating(void* object, AllocObjectInfo* info, UInt size, const char* srcFile, 
										 int srcFileLine)
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
		AllocObjectInfo* info = GetObjectInfo(object);

		for (auto it = mInstance->mObjectsInfos.Begin(); it != mInstance->mObjectsInfos.End(); ++it)
		{
			if (*it == info)
			{
				for (auto ptr : mInstance->mPointers)
				{
					if (ptr->ObjectPtr() == object)
						ptr->ObjectReleased();
				}

				info->~AllocObjectInfo();
				free(info);

				mInstance->mObjectsInfos.Remove(it);

				free((char*)object - sizeof(o2::AllocObjectInfo*));

				return;
			}
		}

		free(object);
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		mInstance->mPointers.Add(ptr);
	}

	void MemoryManager::OnPtrDestroying(IPtr* ptr)
	{
		mInstance->mPointers.Remove(ptr);
	}

	void MemoryManager::CollectGarbage()
	{
		mInstance->mCurrentGCMark = !mInstance->mCurrentGCMark;

		mInstance->RebuildMemoryTree();

		for (auto ptr : mInstance->mPointers)
		{
			if (ptr->mIsOnTop && ptr->mObjectInfo)
				ptr->mObjectInfo->Mark(mInstance->mCurrentGCMark);
		}

		ObjectsInfosVec freeObjects;
		mInstance->FindFreeObjects(freeObjects);
		mInstance->PrintObjectsInfos(freeObjects);
		mInstance->FreeObjects(freeObjects);
	}

	void MemoryManager::ResetMemoryTree()
	{
		for (auto ptr : mInstance->mPointers)
			ptr->mIsOnTop = true;

		for (auto obj : mInstance->mObjectsInfos)
			obj->mChildPointers.Clear();
	}

	void MemoryManager::RebuildMemoryTree()
	{
		ResetMemoryTree();

		int stackValue = 5;
		char* stackEndPtr = (char*)&stackValue;

		for (auto ptr : mInstance->mPointers)
		{
			char* cptr = (char*)ptr;
			bool foundParent = false;
			bool foundObjectInfo = false;

			if (cptr > stackEndPtr)
				ptr->mIsOnTop = false;

			for (auto obj : mInstance->mObjectsInfos)
			{
				char* beg = (char*)obj->mObjectPtr;
				char* end = beg + obj->mSize;

				if (foundParent && cptr >= beg && cptr <= end)
				{
					ptr->mIsOnTop = false;
					obj->mChildPointers.Add(ptr);
					foundParent = true;
				}

				if (ptr->mObjectInfo == obj)
					foundObjectInfo = true;

				if (foundParent && foundObjectInfo)
					break;
			}

			if (!foundObjectInfo)
				ptr->mObjectInfo = nullptr;
		}
	}

	void MemoryManager::FindFreeObjects(ObjectsInfosVec& result)
	{
		result.Clear();

		for (auto obj : mInstance->mObjectsInfos)
		{
			if (obj->mMark != mInstance->mCurrentGCMark)
				result.Add(obj);
		}
	}

	void MemoryManager::FreeObjects(const ObjectsInfosVec& objectsVec)
	{
		for (auto obj : objectsVec)
		{
			delete obj;
			mObjectsInfos.Remove(obj);
		}
	}

	void MemoryManager::PrintObjectsInfos(const ObjectsInfosVec& objectsVec)
	{
		for (auto obj : objectsVec)
			printf("Leaked object: %x %s:%i\n", (UInt)obj, obj->mAllocSrcFile, obj->mAllocSrcFileLine);
	}

	AllocObjectInfo* MemoryManager::GetObjectInfo(void* object)
	{
		return *(AllocObjectInfo**)((char*)object - sizeof(o2::AllocObjectInfo*));
	}

	void AllocObjectInfo::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
		{
			if (ptr->mObjectInfo && ptr->mObjectInfo->mMark != mark)
				ptr->mObjectInfo->Mark(mark);
		}
	}
}