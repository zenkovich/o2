#include "Serializable.h"

namespace o2
{
	void ISerializable::SerializeBasic(const IObject* thisObject, DataNode& node) const
	{
		node.SetValueRaw(*thisObject);
	}

	void ISerializable::DeserializeBasic(IObject* thisObject, const DataNode& node)
	{
		node.GetValueRaw(*thisObject);
	}
}

DECLARE_CLASS(o2::ISerializable);
