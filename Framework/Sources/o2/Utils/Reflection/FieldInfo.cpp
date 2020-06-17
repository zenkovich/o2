#include "o2/stdafx.h"
#include "FieldInfo.h"

#include "o2/Utils/Reflection/Type.h"

namespace o2
{
	FieldInfo::FieldInfo()
	{}

	FieldInfo::FieldInfo(const Type* ownerType, const String& name, GetValuePointerFuncPtr pointerGetter, const Type* type,
						 ProtectSection section, ITypeSerializer* serializer):
		mOwnerType(ownerType), mName(name), mPointerGetter(pointerGetter), mType(type), mProtectSection(section),
		mSerializer(serializer ? serializer : mType->GetSerializer())
	{}

	FieldInfo::FieldInfo(FieldInfo&& other):
		mProtectSection(other.mProtectSection), mName(other.mName), mType(other.mType), mOwnerType(other.mOwnerType),
		mAttributes(other.mAttributes), mSerializer(other.mSerializer), mPointerGetter(other.mPointerGetter)
	{
		other.mAttributes.Clear();
		other.mSerializer = nullptr;
	}

	FieldInfo::~FieldInfo()
	{
		for (auto attr : mAttributes)
			delete attr;

		if (mSerializer)
			delete mSerializer;
	}

	bool FieldInfo::operator==(const FieldInfo& other) const
	{
		return mName == other.mName && mPointerGetter == other.mPointerGetter && mAttributes == other.mAttributes && 
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

	const Type* FieldInfo::GetOwnerType() const
	{
		return mOwnerType;
	}

	const Vector<IAttribute*>& FieldInfo::GetAttributes() const
	{
		return mAttributes;
	}

	bool FieldInfo::CheckSerializable(void* object) const
	{
		return mSerializer->CheckSerializable(GetValuePtrStrong(object));
	}

	void FieldInfo::SerializeFromObject(void* object, DataValue& data) const
	{
		mSerializer->Serialize(GetValuePtrStrong(object), data);
	}

	void FieldInfo::DeserializeFromObject(void* object, const DataValue& data) const
	{
		mSerializer->Deserialize(GetValuePtrStrong(object), data);
	}

	void FieldInfo::Serialize(void* ptr, DataValue& data) const
	{
		mSerializer->Serialize(ptr, data);
	}

	void FieldInfo::Deserialize(void* ptr, const DataValue& data) const
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

	void* FieldInfo::SearchFieldPtr(void* obj, const String& path, const FieldInfo*& fieldInfo) const
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
			return *(void**)GetValuePtrStrong(object);

		return GetValuePtrStrong(object);
	}

	const void* FieldInfo::GetValuePtrStrong(const void* object) const
	{
		return (*mPointerGetter)(const_cast<void*>(object));
	}

	void* FieldInfo::GetValuePtrStrong(void* object) const
	{
		return (*mPointerGetter)(object);
	}
}
