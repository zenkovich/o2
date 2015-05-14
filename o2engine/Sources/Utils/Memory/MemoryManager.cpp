#include "MemoryManager.h"

#include "Utils/Memory/IObject.h"
#include "Utils/Memory/IPtr.h"

namespace o2
{

	void MemoryManager::OnObjectCreating(IObject* object)
	{
		mInstance->mObjects.Add(object);
		object->mMark = mInstance->mCurrentMark;
	}

	void MemoryManager::OnObjectRemoving(IObject* object)
	{
		mInstance->mObjects.Remove(object);
	}

	void MemoryManager::OnPtrCreating(IPtr* ptr)
	{
		char* cptr = (char*)ptr;
		for (auto obj : mInstance->mObjects)
		{
			char* beg = (char*)obj;
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

	void MemoryManager::OnPtrRemoving(IPtr* ptr)
	{
		mInstance->mPointers.Remove(ptr);
	}

	void MemoryManager::CollectGarbage()
	{
		mInstance->mCurrentMark = !mInstance->mCurrentMark;

		for (auto ptr : mInstance->mPointers)
			ptr->mObject->Mark(mInstance->mCurrentMark);

		ObjectsArr freeObjects;

		for (auto obj : mInstance->mObjects)
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
}