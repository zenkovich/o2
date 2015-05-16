#include "ClassFieldInfo.h"

namespace o2
{
	IClassFieldInfo::IClassFieldInfo(const String& name) :
		mName(name)
	{ }

	const String& IClassFieldInfo::Name() const
	{
		return mName;
	}

	ClassFieldRegistrator::ClassFieldRegistrator(IObject* owner, const char* id, void* ptr) :
		mOwner(owner), mId(id), mPtr(ptr)
	{
	}

	SerializeHelper::SerializeHelper(IObject* owner) :
		mOwner(owner)
	{
		mObjectSamples.Add(typeid(*owner).name(), owner);
	}

	Dictionary<String, IObject*> SerializeHelper::mObjectSamples;
}