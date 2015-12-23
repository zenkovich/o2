#include "FieldInfo.h"

#include "Utils/Reflection/Type.h"

namespace o2
{
	FieldInfo::FieldInfo():
		mOffset(0), mIsProperty(false), mType(nullptr), mIsPtr(false)
	{
	}

	FieldInfo::FieldInfo(const String& name, UInt offset, bool isProperty, bool isPtr, Ptr<Type> type):
		mName(name), mOffset(offset), mIsProperty(isProperty), mType(type), mIsPtr(isPtr)
	{
	}

	FieldInfo::FieldInfo(const FieldInfo& other):
		mName(other.mName), mOffset(other.mOffset), mIsProperty(other.mIsProperty), mType(other.mType),
		mIsPtr(other.mIsPtr)
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

	FieldInfo& FieldInfo::operator=(const FieldInfo& other)
	{
		for (auto attr : mAttributes)
			delete attr;

		for (auto attr : other.mAttributes)
		{
			IAttribute* newAttr = attr->Clone();
			newAttr->mOwnerFieldInfo = this;
			mAttributes.Add(newAttr);
		}

		mName = other.mName;
		mOffset = other.mOffset;
		mIsProperty = other.mIsProperty;
		mIsPtr = other.mIsPtr;
		mType = other.mType;

		return *this;
	}

	bool FieldInfo::operator==(const FieldInfo& other) const
	{
		return mName == other.mName && mOffset == other.mOffset && mIsProperty == other.mIsProperty &&
			mAttributes == other.mAttributes;
	}

	FieldInfo* FieldInfo::Clone() const
	{
		return mnew FieldInfo(*this);
	}

	const String& FieldInfo::Name() const
	{
		return mName;
	}

	bool FieldInfo::IsProperty() const
	{
		return mIsProperty;
	}

	const FieldInfo::AttributesVec& FieldInfo::Attributes() const
	{
		return mAttributes;
	}

	FieldInfo* FieldInfo::SearchFieldPath(void* obj, void* target, const String& path, String& res, 
										  Vector<void*>& passedObjects)
	{
		if (!mType)
			return nullptr;

		for (auto field : mType->mFields)
		{
			char* fieldObj = field->GetValuePtr<char>(obj);

			if (fieldObj == nullptr)
				continue;

			if (passedObjects.Contains(fieldObj))
				continue;

			passedObjects.Add(fieldObj);

			if (fieldObj == target)
			{
				res = path + "/" + field->mName;
				return field;
			}

			FieldInfo* childField = field->SearchFieldPath(fieldObj, target, path + "/" + field->mName, res, passedObjects);
			if (childField)
				return childField;
		}

		return nullptr;
	}

	void* FieldInfo::SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo)
	{
		if (!mType)
			return nullptr;

		return mType->GetFieldPtr<char>(obj, path, fieldInfo);
	}

}