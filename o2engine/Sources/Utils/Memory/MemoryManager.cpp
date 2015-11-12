#include "MemoryManager.h"

#include "Utils/Assert.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Memory/IPtr.h"

const size_t AllocObjInfoSize = sizeof(o2::AllocObjectInfo*);

void* operator new(size_t size, const char* location, int line)
{
	void* memory = malloc(size + AllocObjInfoSize);
	void* object = (char*)memory + AllocObjInfoSize;
	o2::MemoryManager::OnObjectCreating(object, size, location, line);
	return object;
}

void operator delete(void* allocMemory)
{
	o2::MemoryManager::OnObjectDestroying(allocMemory);
}

void operator delete(void* allocMemory, const char* location, int line)
{
	free(allocMemory);
}

void* _mmalloc(size_t size, const char* location, int line)
{
	void* memory = malloc(size + AllocObjInfoSize);
	void* object = (char*)memory + AllocObjInfoSize;
	o2::MemoryManager::OnObjectCreating(object, size, location, line);
	return object;
}

void mfree(void* ptr)
{
	o2::MemoryManager::OnObjectDestroying(ptr);
}

namespace o2
{
	MemoryManager::MemoryManager():
		mCurrentGCMark(true)
	{
		mAllocObjsCount = mInitialObjectInfosCount;
		mFreeAllocObjsInfosCount = mInitialObjectInfosCount;

		mAllocObjsInfos = (AllocObjectInfo*)malloc(sizeof(AllocObjectInfo)*mAllocObjsCount);
		mFreeAllocObjsInfos = (int*)malloc(sizeof(int)*mAllocObjsCount);

		for (int i = 0; i < mAllocObjsCount; i++)
		{
			new (mAllocObjsInfos + i) AllocObjectInfo();

			mAllocObjsInfos[i].mObjectPtr = nullptr;
			mFreeAllocObjsInfos[i] = i;
		}

		mPointersCount = mInitialPointersCount;
		mFreePointersCount = mInitialPointersCount;

		mPointers = (IPtr**)malloc(sizeof(IPtr*)*mPointersCount);
		mFreePointers = (int*)malloc(sizeof(int)*mPointersCount);

		for (int i = 0; i < mPointersCount; i++)
		{
			mPointers[i] = nullptr;
			mFreePointers[i] = i;
		}
	}

	MemoryManager::~MemoryManager()
	{
		free(mAllocObjsInfos);
		free(mFreeAllocObjsInfos);
		free(mPointers);
		free(mFreePointers);
	}

	void MemoryManager::OnObjectCreating(void* object, UInt size, const char* srcFile,
										 int srcFileLine)
	{
		auto inst = instance;
		static int id = 0;

		if (instance->mFreeAllocObjsInfosCount == 0)
		{
			int newAllocObjsInfosCount = inst->mAllocObjsCount + inst->mObjectInfosExpandStepCount;
			int lastAllocObjsInfosCount = inst->mAllocObjsCount;
			AllocObjectInfo* newAllocObjsInfos = (AllocObjectInfo*)malloc(sizeof(AllocObjectInfo)*newAllocObjsInfosCount);

			for (int i = 0; i < inst->mAllocObjsCount; i++)
			{
				new (newAllocObjsInfos + i) AllocObjectInfo(inst->mAllocObjsInfos[i]);

				if (newAllocObjsInfos[i].mObjectPtr)
					*(void**)((char*)newAllocObjsInfos[i].mObjectPtr - AllocObjInfoSize) = newAllocObjsInfos + i;
			}

			for (int i = inst->mAllocObjsCount; i < newAllocObjsInfosCount; i++)
			{
				new (newAllocObjsInfos + i) AllocObjectInfo();
				newAllocObjsInfos[i].mObjectPtr = nullptr;
			}

			free(inst->mAllocObjsInfos);
			inst->mAllocObjsInfos = newAllocObjsInfos;
			inst->mAllocObjsCount = newAllocObjsInfosCount;

			free(inst->mFreeAllocObjsInfos);
			inst->mFreeAllocObjsInfosCount = inst->mObjectInfosExpandStepCount;
			inst->mFreeAllocObjsInfos = (int*)malloc(sizeof(int)*newAllocObjsInfosCount);

			for (int i = 0; i < inst->mObjectInfosExpandStepCount; i++)
				inst->mFreeAllocObjsInfos[i] = i + lastAllocObjsInfosCount;
		}

		instance->mFreeAllocObjsInfosCount--;
		int idx = inst->mFreeAllocObjsInfos[inst->mFreeAllocObjsInfosCount];
		AllocObjectInfo* info = &(inst->mAllocObjsInfos[idx]);

		info->mObjectPtr = object;
		info->mSize = size;
		info->mMark = instance->mCurrentGCMark;
		info->mAllocSrcFileLine = srcFileLine;
		info->mIdx = idx;
		info->mId = id++;
		strncpy(info->mAllocSrcFile, srcFile, 127);

		*(AllocObjectInfo**)((char*)object - AllocObjInfoSize) = info;
	}

	void MemoryManager::OnObjectDestroying(void* object)
	{
		auto inst = instance;

		AllocObjectInfo* info = *(AllocObjectInfo**)((char*)object - AllocObjInfoSize);

		if (info >= inst->mAllocObjsInfos && info < inst->mAllocObjsInfos + inst->mAllocObjsCount)
		{
			size_t diff = (char*)info - (char*)inst->mAllocObjsInfos;
			size_t off = diff%sizeof(AllocObjectInfo);
//  			if (off == 0)
//  			{
				if (info->mObjectPtr == object)
				{
					int idx = diff/sizeof(AllocObjectInfo);
					info->mObjectPtr = nullptr;
					inst->mFreeAllocObjsInfos[inst->mFreeAllocObjsInfosCount++] = idx;

					free((char*)object - AllocObjInfoSize);

//  					for (IPtr** ptr = inst->mPointers; ptr < inst->mPointers + inst->mPointersCount; ptr++)
//  					{
//  						if (*ptr && (*ptr)->ObjectPtr() == object)
//  							(*ptr)->ObjectReleased();
//  					}

					return;
				}
			//}
		}

		free(object);
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		auto inst = instance;

		if (inst->mFreePointersCount == 0)
		{
			int newPointersCount = inst->mPointersCount + inst->mPointersExpandStepCount;
			int lastPointersCount = inst->mPointersCount;
			IPtr** newPointersArray = (IPtr**)malloc(sizeof(IPtr*)*newPointersCount);

			for (int i = 0; i < lastPointersCount; i++)
				newPointersArray[i] = inst->mPointers[i];

			for (int i = lastPointersCount; i < newPointersCount; i++)
				newPointersArray[i] = nullptr;

			free(inst->mPointers);

			inst->mPointers = newPointersArray;
			inst->mPointersCount = newPointersCount;

			free(inst->mFreePointers);
			inst->mFreePointers = (int*)malloc(sizeof(int)*newPointersCount);
			inst->mFreePointersCount = inst->mPointersExpandStepCount;

			for (int i = 0; i < inst->mFreePointersCount; i++)
				inst->mFreePointers[i] = i + lastPointersCount;
		}

		inst->mFreePointersCount--;
		int idx = inst->mFreePointers[inst->mFreePointersCount];
		IPtr** pptr = &inst->mPointers[idx];
		*pptr = ptr;
		ptr->mIdx = idx;
	}

	void MemoryManager::OnPtrDestroying(IPtr* ptr)
	{
		auto inst = instance;

		inst->mPointers[ptr->mIdx] = nullptr;
		inst->mFreePointers[inst->mFreePointersCount++] = ptr->mIdx;
	}

	void MemoryManager::CollectGarbage()
	{
		auto inst = instance;

		inst->mCurrentGCMark = !inst->mCurrentGCMark;

		inst->RebuildMemoryTree();

		for (auto ptr : inst->mRootPointers)
		{
			if (ptr->mObjectInfo)
				ptr->mObjectInfo->Mark(inst->mCurrentGCMark);
		}

		ObjectsInfosVec freeObjects;
		inst->FindFreeObjects(freeObjects);
		inst->PrintObjectsInfos(freeObjects);
		inst->FreeObjects(freeObjects);
	}

	void MemoryManager::ResetMemoryTree()
	{
		auto inst = instance;

		for (AllocObjectInfo* pinfo = inst->mAllocObjsInfos; pinfo < inst->mAllocObjsInfos + inst->mAllocObjsCount; pinfo++)
			if (pinfo->mObjectPtr)
				pinfo->mChildPointers.clear();
		
		inst->mRootPointers.clear();
	}

	void MemoryManager::RebuildMemoryTree()
	{
		auto inst = instance;

		ResetMemoryTree();

		for (IPtr** pptr = inst->mPointers; pptr < inst->mPointers + inst->mPointersCount; pptr++)
		{
			if (*pptr == nullptr)
				continue;

			void* objPtr = (*pptr)->ObjectPtr();
			if (objPtr)
				(*pptr)->mObjectInfo = GetObjectInfo(objPtr);
		}

		for (IPtr** pptr = inst->mPointers; pptr < inst->mPointers + inst->mPointersCount; pptr++)
		{
			if (*pptr == nullptr)
				continue;

			IPtr* ptr = *pptr;
			char* cptr = (char*)ptr;

			for (AllocObjectInfo* pinfo = inst->mAllocObjsInfos; pinfo < inst->mAllocObjsInfos + inst->mAllocObjsCount; pinfo++)
			{
				if (pinfo->mObjectPtr == nullptr)
					continue;

				char* beg = (char*)pinfo->mObjectPtr;
				char* end = beg + pinfo->mSize;

				if (cptr >= beg && cptr < end)
				{
					ptr->mMemTreePosition = IPtr::TreePosition::Child;
					pinfo->mChildPointers.push_back(ptr);
					break;
				}
			}
		}

		for (IPtr** pptr = inst->mPointers; pptr < inst->mPointers + inst->mPointersCount; pptr++)
		{
			if (*pptr == nullptr)
				continue;

			IPtr* ptr = *pptr;
			if (ptr->mMemTreePosition == IPtr::TreePosition::Root)
				mRootPointers.push_back(ptr);
		}
	}

	void MemoryManager::FindFreeObjects(ObjectsInfosVec& result)
	{
		auto inst = instance;

		result.clear();
		for (AllocObjectInfo* pinfo = inst->mAllocObjsInfos; pinfo < inst->mAllocObjsInfos + inst->mAllocObjsCount; pinfo++)
		{
			if (pinfo->mObjectPtr == nullptr)
				continue;

			if (pinfo->mMark != instance->mCurrentGCMark)
				result.push_back(pinfo);
		}
	}

	void MemoryManager::FreeObjects(const ObjectsInfosVec& infos)
	{
		auto inst = instance;

		for (auto info : infos)
		{
			//TODO: need to call destructor for destroying object!

			inst->mFreeAllocObjsInfos[inst->mFreeAllocObjsInfosCount++] = info->mIdx;
			info->mObjectPtr = nullptr;
		}
	}

	void MemoryManager::PrintObjectsInfos(const ObjectsInfosVec& infos)
	{
		for (auto info : infos)
			printf("Leaked object: %x %s:%i\n", (UInt)info, info->mAllocSrcFile, info->mAllocSrcFileLine);
	}

	AllocObjectInfo* MemoryManager::GetObjectInfo(void* object)
	{
		auto inst = instance;

		if (object == nullptr)
			return nullptr;

		AllocObjectInfo* info = *(AllocObjectInfo**)((char*)object - AllocObjInfoSize);

		if (info >= inst->mAllocObjsInfos && info < inst->mAllocObjsInfos + inst->mAllocObjsCount)
		{
			size_t diff = (char*)info - (char*)inst->mAllocObjsInfos;
			size_t off = diff%AllocObjInfoSize;
			if (off == 0)
			{
				if (info->mObjectPtr == object)
					return info;
			}
		}

		return nullptr;
	}

	AllocObjectInfo::AllocObjectInfo(const AllocObjectInfo& other):
		mObjectPtr(other.mObjectPtr), mSize(other.mSize), mMark(other.mMark), mChildPointers(other.mChildPointers),
		mPointers(other.mPointers), mAllocSrcFileLine(other.mAllocSrcFileLine), mIdx(other.mIdx)
	{
		strcpy(mAllocSrcFile, other.mAllocSrcFile);
	}

	AllocObjectInfo& AllocObjectInfo::operator=(const AllocObjectInfo& other)
	{
		mObjectPtr = other.mObjectPtr;
		mSize = other.mSize;
		mMark = other.mMark;
		mChildPointers = other.mChildPointers;
		mPointers = other.mPointers;
		mAllocSrcFileLine = other.mAllocSrcFileLine;
		mIdx = other.mIdx;
		strcpy(mAllocSrcFile, other.mAllocSrcFile);
		return *this;
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