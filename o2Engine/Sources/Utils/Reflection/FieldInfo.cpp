#include "FieldInfo.h"

#include "Utils/Reflection/Type.h"

namespace o2
{
	FieldInfo::FieldInfo():
		mOffset(0), mIsProperty(false), mType(nullptr), mIsPointer(false), mProtectSection(ProtectSection::Public)
	{}

	FieldInfo::FieldInfo(const String& name, UInt offset, bool isProperty, bool isPtr, const Type* type, 
						 ProtectSection sect, IFieldSerializer* serializer):
		mName(name), mOffset(offset), mIsProperty(isProperty), mType(type), mIsPointer(isPtr), mProtectSection(sect),
		mSerializer(serializer)
	{}

	FieldInfo::~FieldInfo()
	{
		for (auto attr : mAttributes)
			delete attr;

		delete mSerializer;
	}

	bool FieldInfo::operator==(const FieldInfo& other) const
	{
		return mName == other.mName && mOffset == other.mOffset && mIsProperty == other.mIsProperty &&
			mAttributes == other.mAttributes && mIsPointer == other.mIsPointer && mProtectSection == other.mProtectSection;
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

	bool FieldInfo::IsPointer() const
	{
		return mIsPointer;
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

		auto type = mIsPointer ? mType->GetUnpointedType() : mType;
		return type->SearchFieldPath(obj, target, path, res, passedObjects);
	}

	void* FieldInfo::SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo)
	{
		if (!mType)
			return nullptr;

		if (mIsPointer)
			return mType->GetUnpointedType()->GetFieldPtr(obj, path, fieldInfo);

		return mType->GetFieldPtr(obj, path, fieldInfo);
	}

	void* FieldInfo::GetValuePtr(void* object) const
	{
		if (mIsPointer)
			return *(void**)((char*)object + mOffset);

		return (void*)((char*)object + mOffset);
	}

	const void* FieldInfo::GetValuePtrStrong(const void* object) const
	{
		return (void*)((char*)object + mOffset);
	}

	void* FieldInfo::GetValuePtrStrong(void* object) const
	{
		return (void*)((char*)object + mOffset);
	}
}