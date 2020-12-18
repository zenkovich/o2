#include "o2/stdafx.h"
#include "Serializable.h"

namespace o2
{
	String ISerializable::SerializeToString() const
	{
		DataDocument doc;
		Serialize(doc);
		return doc.SaveAsString();
	}

	void ISerializable::DeserializeFromString(const String& str)
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

	void SerializationTypeProcessor::BaseFieldProcessor::SetProtectSection(ProtectSection section)
	{}

	SerializationTypeProcessor::BaseFieldProcessor SerializationTypeProcessor::StartField()
	{
		return BaseFieldProcessor(node);
	}

}

DECLARE_CLASS(o2::ISerializable);
