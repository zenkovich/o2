#include "Serialization.h"

namespace o2
{
	ISerializable* SerializableTypesSamples::CreateSample(const String& type)
	{
		Assert(mObjectSamples.ContainsKey(type), "Failed to create type sample");
		return mObjectSamples.Get(type)->CreateSample();
	}

	SerializationFieldsProcessor::SerializationFieldsProcessor(DataNode& dataNode):
		mDataNode(dataNode)
	{}

	DeserializationFieldsProcessor::DeserializationFieldsProcessor(const DataNode& dataNode):
		mDataNode(dataNode)
	{}
}