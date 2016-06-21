#include "Serializable.h"

namespace o2
{
	DataNode ISerializable::SerializeBasic(const void* thisObject) const
	{
		DataNode res;
		OnSerialize(res);
		char* thisPtr = (char*)thisObject;
		for (auto field : GetType().Fields())
		{
			auto srlzAttribute = field->Attribute<SerializableAttribute>();
			if (srlzAttribute)
			{
				field->SerializeObject((void*)field->GetValuePtrStrong<char>(thisPtr), *res.AddNode(field->Name()));
			}
		}

		return res;
	}

	void ISerializable::DeserializeBasic(const DataNode& node, const void* thisObject)
	{
		char* thisPtr = (char*)thisObject;
		for (auto field : GetType().Fields())
		{
			auto srlzAttribute = field->Attribute<SerializableAttribute>();
			if (srlzAttribute)
			{
				auto fldNode = node.GetNode(field->Name());
				if (fldNode)
				{
					field->DeserializeObject((void*)field->GetValuePtrStrong<char>(thisPtr), *fldNode);
				}
			}
		}
		OnDeserialized(node);
	}
}
