#include "o2/stdafx.h"
#include "Serializable.h"

namespace o2
{
	void ISerializable::SerializeBasic(const IObject& thisObject, DataNode& node) const
	{
		node.SetValue(thisObject);
	}

	void ISerializable::DeserializeBasic(IObject& thisObject, const DataNode& node)
	{
		node.GetValue(thisObject);
	}
}

DECLARE_CLASS(o2::ISerializable);
