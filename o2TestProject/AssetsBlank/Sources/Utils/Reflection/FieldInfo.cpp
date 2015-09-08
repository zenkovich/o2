#include "FieldInfo.h"

namespace o2
{
	FieldInfo::FieldInfo():
		mOffset(0), mIsProperty(false)
	{
	}

	FieldInfo::FieldInfo(const String& name, UInt offset, bool isProperty):
		mName(name), mOffset(offset), mIsProperty(isProperty)
	{
	}

	bool FieldInfo::operator==(const FieldInfo& other) const
	{
		return mName == other.mName && mOffset == other.mOffset && mIsProperty == other.mIsProperty &&
			mAttributes == other.mAttributes;
	}

	const String& FieldInfo::Name() const
	{
		return mName;
	}

	bool FieldInfo::IsProperty() const
	{
		return mIsProperty;
	}

	const Vector<IAttribute*>& FieldInfo::Attributes() const
	{
		return mAttributes;
	}

}