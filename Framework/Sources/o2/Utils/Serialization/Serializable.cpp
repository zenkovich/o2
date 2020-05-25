#include "o2/stdafx.h"
#include "Serializable.h"

namespace o2
{
	WString ISerializable::SerializeToString() const
	{
		DataDocument doc;
		Serialize(doc);
		return doc.SaveAsWString();
	}

	void ISerializable::DeserializeFromString(const WString& str)
	{
		DataDocument doc;
		doc.LoadFromData(str);
		Deserialize(doc);
	}

	void ISerializable::SerializeBasic(const IObject& thisObject, DataValue& node) const
	{
		node.Set(thisObject);
	}

	void ISerializable::DeserializeBasic(IObject& thisObject, const DataValue& node)
	{
		node.Get(thisObject);
	}
}

DECLARE_CLASS(o2::ISerializable);
