#include "ClassFieldInfo.h"

namespace o2
{
	IClassFieldInfo::IClassFieldInfo(void* owner, const String& name) :
		mName(name), mOwner(owner)
	{}

	const String& IClassFieldInfo::Name() const
	{
		return mName;
	}
}