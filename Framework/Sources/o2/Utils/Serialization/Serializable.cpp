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

	void SerializeTypeProcessor::BaseFieldProcessor::SetProtectSection(ProtectSection section)
	{}

	SerializeTypeProcessor::BaseFieldProcessor SerializeTypeProcessor::StartField()
	{
		return BaseFieldProcessor(node);
	}

}

DECLARE_CLASS(o2::ISerializable);
