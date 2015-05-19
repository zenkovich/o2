#include "AllocOperators.h"

#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/IObject.h"

void* operator new(size_t size, bool managed, const char* location, int line)
{
	o2::IObject* res = (o2::IObject*)::operator new(size);
	o2::MemoryManager::OnObjectCreating(res, size, managed, location, line);
	return res;
}

void operator delete(void* obj, bool managed, const char* location, int line)
{
	o2::MemoryManager::OnObjectDestroying((o2::IObject*)obj);
	::operator delete(obj);
}