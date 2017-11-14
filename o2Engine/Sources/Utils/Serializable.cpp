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

CLASS_META(o2::ISerializable)
{
	BASE_CLASS(o2::IObject);


	PUBLIC_FUNCTION(DataNode, Serialize);
	PUBLIC_FUNCTION(void, Deserialize, const DataNode&);
	PUBLIC_FUNCTION(void, OnSerialize, DataNode&);
	PUBLIC_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeBasic, const IObject*, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeBasic, IObject*, const DataNode&);
}
END_META;
