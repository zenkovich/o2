#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/CommonTypes.h"
#include "Utils/Memory/AllocOperators.h"

namespace o2
{
	class IPtr;
	class XSerializer;
	class IClassFieldInfo;
	class DataNode;

	class IObject
	{
		friend class MemoryManager;
		friend class IPtr;
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;
		friend void* ::operator new(size_t size, bool managed, const char* location, int line);

	public:
		typedef Array<IPtr*> PointersArr;
		typedef Array<IClassFieldInfo*> FieldsArr;

	protected:
		PointersArr mPointers;
		PointersArr mChildPointers;
		FieldsArr   mFields;
		UInt        mSize;
		bool        mMark;
		bool        mManaged;
		char*       mAllocSrcFile;
		int         mAllocSrcFileLine;

		IObject();
		virtual ~IObject();

		void Mark(bool mark);

	public:
		virtual DataNode Serialize();
		virtual void     Deserialize(DataNode& node);
	};
}