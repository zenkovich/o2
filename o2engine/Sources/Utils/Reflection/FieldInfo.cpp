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

	FieldInfo::FieldInfo(const FieldInfo& other):
		mName(other.mName), mOffset(other.mOffset), mIsProperty(other.mIsProperty)
	{
		for (auto attr : other.mAttributes)
		{
			IAttribute* newAttr = attr->Clone();
			newAttr->mOwnerFieldInfo = this;
			mAttributes.Add(newAttr);
		}
	}

	FieldInfo::~FieldInfo()
	{
		for (auto attr : mAttributes)
			delete attr;
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