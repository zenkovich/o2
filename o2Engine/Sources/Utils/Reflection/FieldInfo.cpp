#include "FieldInfo.h"

#include "Utils/Reflection/Type.h"

namespace o2
{
	FieldInfo::FieldInfo()
	{}

	FieldInfo::FieldInfo(const String& name, UInt offset, const Type* type, 
						 ProtectSection sect, IFieldSerializer* serializer):
		mName(name), mOffset(offset), mType(type), mProtectSection(sect),
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
		return mName == other.mName && mOffset == other.mOffset && mAttributes == other.mAttributes && 
			mProtectSection == other.mProtectSection;
	}

	FieldInfo& FieldInfo::AddAttribute(IAttribute* attribute)
	{
		attribute->mOwnerFieldInfo = this;
		mAttributes.Add(attribute);
		return *this;
	}

	const String& FieldInfo::GetName() const
	{
		return mName;
	}

	ProtectSection FieldInfo::GetProtectionSection() const
	{
		return mProtectSection;
	}

	const Type* FieldInfo::GetType() const
	{
		return mType;
	}

	const FieldInfo::AttributesVec& FieldInfo::GetAttributes() const
	{
		return mAttributes;
	}

	void FieldInfo::SerializeFromObject(void* object, DataNode& data) const
	{
		mSerializer->Serialize(GetValuePtrStrong(object), data);
	}

	void FieldInfo::DeserializeFromObject(void* object, DataNode& data) const
	{
		mSerializer->Deserialize(GetValuePtrStrong(object), data);
	}

	void FieldInfo::Serialize(void* ptr, DataNode& data) const
	{
		mSerializer->Serialize(ptr, data);
	}

	void FieldInfo::Deserialize(void* ptr, DataNode& data) const
	{
		mSerializer->Deserialize(ptr, data);
	}

	bool FieldInfo::IsValueEquals(void* objectA, void* objectB) const
	{
		return mSerializer->Equals(GetValuePtrStrong(objectA), GetValuePtrStrong(objectB));
	}

	void FieldInfo::CopyValue(void* objectA, void* objectB) const
	{
		mSerializer->Copy(GetValuePtrStrong(objectA), GetValuePtrStrong(objectB));
	}

	FieldInfo* FieldInfo::SearchFieldPath(void* obj, void* target, const String& path, String& res,
										  Vector<void*>& passedObjects)
	{
		if (!mType)
			return nullptr;

		auto type = mType->GetUsage() == Type::Usage::Pointer ? ((PointerType*)mType)->GetUnpointedType() : mType;
		return type->SearchFieldPath(obj, target, path, res, passedObjects);
	}

	void* FieldInfo::SearchFieldPtr(void* obj, const String& path, FieldInfo*& fieldInfo)
	{
		if (!mType)
			return nullptr;

		if (mType->GetUsage() == Type::Usage::Pointer)
			return ((PointerType*)mType)->GetUnpointedType()->GetFieldPtr(obj, path, fieldInfo);

		return mType->GetFieldPtr(obj, path, fieldInfo);
	}

	void* FieldInfo::GetValuePtr(void* object) const
	{
		if (mType->GetUsage() == Type::Usage::Pointer)
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