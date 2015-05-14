#include "IObject.h"

#include "Utils/Memory/MemoryManager.h"
#include "EngineSettings.h"

namespace o2
{
	IObject::IObject()
	{}

	IObject::~IObject()
	{
		if (MEM_TRACE)
			delete[] mAllocSrcFile;
	}

	void IObject::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
			ptr->mObject->Mark(mark);
	}
}