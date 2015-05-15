#include "IObject.h"

#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/ClassFieldInfo.h"
#include "Utils/Data/DataDoc.h"
#include "EngineSettings.h"

namespace o2
{
	IObject::IObject():
		mAllocSrcFile(nullptr)
	{}

	IObject::~IObject()
	{
		if (MEM_TRACE)
		{
			if (mAllocSrcFile) delete[] mAllocSrcFile;
		}

		for (auto fld : mFields)
			delete fld;
	}

	void IObject::Mark(bool mark)
	{
		mMark = mark;

		for (auto ptr : mChildPointers)
			ptr->mObject->Mark(mark);
	}

	IObject& IObject::operator=(DataNode& data)
	{
		for (auto fld : mFields)
			*fld = *data.GetNode(fld->Name());

		return *this;
	}

	IObject::operator DataNode()
	{
		DataNode res;
		for (auto fld : mFields)
			*res.AddNode(fld->Name()) = *fld;

		return res;
	}

}