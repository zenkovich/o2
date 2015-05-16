#include "Serialization.h"

namespace o2
{
	ClassFieldRegistrator::ClassFieldRegistrator(IObject* owner, const char* id, void* ptr) :
		mOwner(owner), mId(id), mPtr(ptr)
	{}

	SerializeHelper::SerializeHelper(IObject* owner) :
		mOwner(owner)
	{}
		
	IObject* SerializableTypesSamples::CreateSample(const String& type)
	{
		Assert(mObjectSamples.ContainsKey(type), "Failed to create type sample");
		return mObjectSamples.Get(type)->CreateSample();
	}
}