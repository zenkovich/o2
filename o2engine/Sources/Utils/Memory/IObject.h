#pragma once

namespace o2
{
	struct ObjectInfo;

	class IObject
	{
		friend class MemoryManager;
		friend class IPtr;
		friend struct ObjectInfo;

	protected:
		ObjectInfo* mObjectInfo; /** Pointer to object info data. */

		/** virtual dtor. */
		virtual ~IObject() {}
	};
}