#include "Reflection.h"

namespace o2
{
	IFieldInfo::IFieldInfo()
	{}

	IFieldInfo::IFieldInfo(const String& name, const void* owner):
		mName(name), mOwner(owner)
	{}


	ReflectionFieldsProcessor::ReflectionFieldsProcessor(const void* owner):
		mOwner(owner)
	{}
}