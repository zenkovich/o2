#include "AllocOperators.h"

#include "Utils/Memory/IObject.h"
#include "Utils/Memory/MemoryManager.h"

void* operator new(size_t size, bool managed, const char* location, int line)
{
	void* res = ::operator new(size);
	o2::IObject* resObj = (o2::IObject*)res;

	if (MEM_LEAKS_CHECK)
	{
		resObj->mSize = size;
		resObj->mManaged = managed;

		o2::MemoryManager::OnObjectCreating(resObj);
	}

	if (MEM_TRACE)
	{
		int ln = strlen(location);
		resObj->mAllocSrcFile = new char[ln + 1];
		strcpy(resObj->mAllocSrcFile, location);

		resObj->mAllocSrcFileLine = line;
	}

	return res;
}

void operator delete(void* obj, bool managed, const char* location, int line)
{
	o2::MemoryManager::OnObjectRemoving((o2::IObject*)obj);
	::operator delete(obj);
}