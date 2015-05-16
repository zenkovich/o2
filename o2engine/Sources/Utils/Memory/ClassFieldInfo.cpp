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
}