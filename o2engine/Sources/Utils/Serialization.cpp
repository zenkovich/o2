#include "Serialization.h"

namespace o2
{	
	DataNode ISerializable::Serialize()
	{
		DataNode res;
		auto fields = GetFields();
		for (auto fld : fields)
		{
			*res.AddNode(fld->Name()) = fld->Serialize();
			fld.Release();
		}

		return res;
	}

	void ISerializable::Deserialize(const DataNode& node)
	{
		auto fields = GetFields();
		for (auto fld : fields)
		{
			fld->Deserialize(*node.GetNode(fld->Name()));
			fld.Release();
		}
	}

	ISerializable::FieldsVec ISerializable::GetFields()
	{
		return FieldsVec();
	}

	ISerializable* SerializableTypesSamples::CreateSample(const String& type)
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