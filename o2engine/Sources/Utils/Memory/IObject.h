#pragma once

namespace o2
{
	struct ObjectInfo;

	class IObject
	{
		friend class MemoryManager;
		friend struct ObjectInfo;
		friend class IPtr;

	protected:
		ObjectInfo* mObjectInfo;
		virtual ~IObject() {}
	};
}