#include "MemoryManager.h"

#include "Utils/Assert.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Memory/IPtr.h"

void* operator new(size_t size)
{
	return malloc(size);
}

void* operator new(size_t size, const char* location, int line)
{
	void* object = malloc(size);
	o2::MemoryManager::OnObjectCreating(object, size, location, line);
	return object;
}

void operator delete(void* allocMemory)
{
	o2::MemoryManager::OnObjectDestroying(allocMemory);
	free(allocMemory);
}

void operator delete(void* allocMemory, const char* location, int line)
{
	free(allocMemory);
}

namespace o2
{
	void MemoryManager::OnObjectCreating(void* object, UInt size, const char* srcFile,
										 int srcFileLine)
	{
		AllocObjectInfo* info = new AllocObjectInfo();

		info->mObjectPtr = object;
		info->mSize = size;
		info->mMark = instance->mCurrentGCMark;
		info->mAllocSrcFileLine = srcFileLine;
		strncpy(info->mAllocSrcFile, srcFile, 127);

		instance->mObjectsInfos.Add(info);
	}

	void MemoryManager::OnObjectDestroying(void* object)
	{
		int i = 0;
		for (auto info : instance->mObjectsInfos)
		{
			if (info->mObjectPtr == object)
			{
				for (auto ptr : instance->mPointers)
				{
					if (ptr->ObjectPtr() == object)
						ptr->ObjectReleased();
				}

				delete info;
				instance->mObjectsInfos.RemoveAt(i);

				return;
			}

			i++;
		}
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		instance->mPointers.Add(ptr);
	}

	void MemoryManager::OnPtrDestroying(IPtr* ptr)
	{
		instance->mPointers.Remove(ptr);
	}

	void MemoryManager::CollectGarbage()
	{
		instance->mCurrentGCMark = !instance->mCurrentGCMark;

		instance->RebuildMemoryTree();

		for (auto ptr : instance->mPointers)
		{
			if (ptr->mIsOnTop && ptr->mObjectInfo)
				ptr->mObjectInfo->Mark(instance->mCurrentGCMark);
		}

		ObjectsInfosVec freeObjects;
		instance->FindFreeObjects(freeObjects);
		instance->PrintObjectsInfos(freeObjects);
		instance->FreeObjects(freeObjects);
	}

	void MemoryManager::ResetMemoryTree()
	{
		for (auto ptr : instance->mPointers)
			ptr->mIsOnTop = true;

		for (auto obj : instance->mObjectsInfos)
			obj->mChildPointers.Clear();
	}

	void MemoryManager::RebuildMemoryTree()
	{
		ResetMemoryTree();

		int stackValue = 5;
		char* stackEndPtr = (char*)&stackValue;

		for (auto ptr : instance->mPointers)
		{
			char* cptr = (char*)ptr;
			bool foundParent = false;
			bool foundObjectInfo = false;

			if (cptr < stackEndPtr)
				ptr->mIsOnTop = false;

			for (auto obj : instance->mObjectsInfos)
			{
				char* beg = (char*)obj->mObjectPtr;
				char* end = beg + obj->mSize;

				if (!foundParent && cptr >= beg && cptr <= end)
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

		for (auto obj : instance->mObjectsInfos)
		{
			if (obj->mMark != instance->mCurrentGCMark)
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
		for (auto info : instance->mObjectsInfos)
			if (info->mObjectPtr == object)
				return info;

		return nullptr;
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