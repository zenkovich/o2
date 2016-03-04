#include "Serializer.h"

#include "Scene/Actor.h"

namespace o2
{
	void Serializer::Serialize(Actor* actor, DataNode& data)
	{
		*data.AddNode("Type") = TypeOf(Actor).Name();
		*data.AddNode("Value") = actor ? actor->Serialize() : (String)"null";
		printf("Actor pointer serialization");
	}

	void Serializer::Deserialize(Actor*& actor, DataNode& data)
	{
		String type;
		ISerializable* value = nullptr;

		if (auto typeNode = data.GetNode("Type"))
		{
			type = *typeNode;

			if (auto valueNode = data.GetNode("Value"))
			{
				if (valueNode->Data() == "null")
					actor = nullptr;
				else
				{
					actor = static_cast<Actor*>(Reflection::CreateTypeSample(type));
					actor->Deserialize(*valueNode);
				}
			}
		}
	}
}