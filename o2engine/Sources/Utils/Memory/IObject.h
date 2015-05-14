#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/CommonTypes.h"
#include "Utils/Memory/AllocOperators.h"

namespace o2
{
	class IPtr;

	class IObject
	{
		friend class MemoryManager;
		friend class IPtr;
		friend void* ::operator new(size_t size, bool managed, const char* location, int line);

	public:
		typedef Array<IPtr*> PointersArr;

	protected:
		PointersArr mPointers;
		PointersArr mChildPointers;
		UInt        mSize;
		bool        mMark;
		bool        mManaged;
		char*       mAllocSrcFile;
		int         mAllocSrcFileLine;

		IObject();
		virtual ~IObject();

		void Mark(bool mark);
	};
}