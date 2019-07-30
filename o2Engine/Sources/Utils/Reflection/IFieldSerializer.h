#pragma once

#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	class DataNode;

	struct IFieldSerializer
	{
		virtual ~IFieldSerializer() { }

		virtual void Serialize(void* object, DataNode& data) const { }
		virtual void Deserialize(void* object, DataNode& data) const { }
		virtual bool Equals(void* objectA, void* objectB) const { return false; }
		virtual void Copy(void* objectA, void* objectB) const { }
		virtual IFieldSerializer* Clone() const { return mnew IFieldSerializer(); }
	};
}
