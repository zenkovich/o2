#include "o2/stdafx.h"
#include "Serializable.h"

namespace o2
{
	WString ISerializable::SerializeToString() const
	{
		DataNode data;
		data = Serialize();
		return data.SaveAsWString();
	}

	void ISerializable::DeserializeFromString(const WString& str)
	{
		DataNode data;
		data.LoadFromData(str);
		Deserialize(data);
	}

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
