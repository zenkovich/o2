#pragma once

namespace o2
{
	class ObjectInfo;

	class IObject
	{
		friend class MemoryManager;
		friend class IPtr;

	protected:
		ObjectInfo* mObjectInfo;
		virtual ~IObject() {}
	};
}