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

	DataNode IObject::Serialize()
	{
		DataNode res;
		for (auto fld : mFields)
			*res.AddNode(fld->Name()) = fld->Serialize();

		return res;
	}

	void IObject::Deserialize(DataNode& node)
	{
		for (auto fld : mFields)
			fld->Deserialize(*node.GetNode(fld->Name()));
	}

}