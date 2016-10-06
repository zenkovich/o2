#include "FieldInfo.h"

#include "Utils/Reflection/Type.h"

namespace o2
{
	FieldInfo::FieldInfo():
		mOffset(0), mIsProperty(false), mType(nullptr), mIsPtr(false), mProtectSection(ProtectSection::Public)
	{}

	FieldInfo::FieldInfo(const String& name, UInt offset, bool isProperty, bool isPtr, const Type* type, 
						 ProtectSection sect, IFieldSerializer* serializer):
		mName(name), mOffset(offset), mIsProperty(isProperty), mType(type), mIsPtr(isPtr), mProtectSection(sect),
		mSerializer(serializer)
	{}

	FieldInfo::FieldInfo(const FieldInfo& other) :
		mName(other.mName), mOffset(other.mOffset), mIsProperty(other.mIsProperty), mType(other.mType),
		mIsPtr(other.mIsPtr), mProtectSection(other.mProtectSection), mSerializer(other.mSerializer->Clone())
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

		delete mSerializer;
	}

	FieldInfo& FieldInfo::operator=(const FieldInfo& other)
	{
		for (auto attr : mAttributes)
			delete attr;

		delete mSerializer;

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
		mProtectSection = other.mProtectSection;
		mSerializer = other.mSerializer->Clone();

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

	FieldInfo& FieldInfo::AddAttribute(IAttribute* attribute)
	{
		attribute->mOwnerFieldInfo = this;
		mAttributes.Add(attribute);
		return *this;
	}

	const String& FieldInfo::Name() const
	{
		return mName;
	}

	bool FieldInfo::IsProperty() const
	{
		return mIsProperty;
	}

	ProtectSection FieldInfo::GetProtectionSection() const
	{
		return mProtectSection;
	}

	const Type& FieldInfo::GetType() const
	{
		return *mType;
	}

	const FieldInfo::AttributesVec& FieldInfo::Attributes() const
	{
		return mAttributes;
	}

	void FieldInfo::SerializeObject(void* object, DataNode& data) const
	{
		mSerializer->Serialize(object, data);
	}

	void FieldInfo::DeserializeObject(void* object, DataNode& data) const
	{
		mSerializer->Deserialize(object, data);
	}

	FieldInfo* FieldInfo::SearchFieldPath(void* obj, void* target, const String& path, String& res,
										  Vector<void*>& passedObjects)
	{
		if (!mType)
			return nullptr;

		auto type = mIsPtr ? mType->GetUnpointedType() : mType;
		for (auto field : type->AllFields())
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

		if (mIsPtr)
			return mType->GetUnpointedType()->GetFieldPtr<char>(obj, path, fieldInfo);

		return mType->GetFieldPtr<char>(obj, path, fieldInfo);
	}

}