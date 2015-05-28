#include "Serialization.h"

namespace o2
{	
	DataNode Serializable::Serialize()
	{
		DataNode res;
		auto fields = GetFields();
		for (auto fld : fields)
		{
			*res.AddNode(fld->Name()) = fld->Serialize();
		}

		return res;
	}

	void Serializable::Deserialize(const DataNode& node)
	{
		auto fields = GetFields();
		for (auto fld : fields)
		{
			fld->Deserialize(*node.GetNode(fld->Name()));
		}
	}

	Serializable::FieldsArr Serializable::GetFields()
	{
		return FieldsArr();
	}

	Serializable* SerializableTypesSamples::CreateSample(const String& type)
	{
		Assert(mObjectSamples.ContainsKey(type), "Failed to create type sample");
		return mObjectSamples.Get(type)->CreateSample();
	}

	IClassFieldInfo::IClassFieldInfo(void* owner, const String& name):
		mName(name), mOwner(owner)
	{}

	const String& IClassFieldInfo::Name() const
	{
		return mName;
	}
}