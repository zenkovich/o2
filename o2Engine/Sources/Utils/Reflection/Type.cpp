#include "stdafx.h"
#include "Type.h"

#include "Animation/Animation.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Serialization/DataNode.h"
#include "Utils/System/Time/Timer.h"

namespace o2
{
	Type::Type(const String& name, int size, ITypeSerializer* serializer):
		mId(0), mPtrType(nullptr), mName(name), mSize(size), mSerializer(serializer)
	{}

	Type::~Type()
	{
		for (auto field : mFields)
			delete field;
	}

	bool Type::operator!=(const Type& other) const
	{
		return other.mId != mId;
	}

	bool Type::operator==(const Type& other) const
	{
		return other.mId == mId;
	}

	const String& Type::GetName() const
	{
		return mName;
	}

	TypeId Type::ID() const
	{
		return mId;
	}

	int Type::GetSize() const
	{
		return mSize;
	}

	bool Type::IsBasedOn(const Type& other) const
	{
		if (mId == other.mId)
			return true;

		for (auto typeInfo : mBaseTypes)
		{
			if (typeInfo.type->mId == mId)
				return true;

			if (typeInfo.type->IsBasedOn(other))
				return true;
		}

		return false;
	}

	Type::Usage Type::GetUsage() const
	{
		return Usage::Regular;
	}

	const Type::BaseTypesVec& Type::GetBaseTypes() const
	{
		return mBaseTypes;
	}

	const Type::FieldInfosVec& Type::GetFields() const
	{
		return mFields;
	}

	Type::FieldInfosVec Type::GetFieldsWithBaseClasses() const
	{
		FieldInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType.type->GetFieldsWithBaseClasses();

		res += mFields;

		return res;
	}

	const Type::FunctionsInfosVec& Type::GetFunctions() const
	{
		return mFunctions;
	}

	const Type::StaticFunctionsInfosVec& Type::GetStaticFunctions() const
	{
		return mStaticFunctions;
	}

	Type::FunctionsInfosVec Type::GetFunctionsWithBaseClasses() const
	{
		FunctionsInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType.type->GetFunctionsWithBaseClasses();

		res += mFunctions;

		return res;
	}

	Type::StaticFunctionsInfosVec Type::GetStaticFunctionsWithBaseClasses() const
	{
		StaticFunctionsInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType.type->GetStaticFunctionsWithBaseClasses();

		res += mStaticFunctions;

		return res;
	}

	FieldInfo* Type::GetField(const String& name) const
	{
		for (auto field : mFields)
		{
			if (field->GetName() == name)
				return field;
		}

		for (auto baseType : mBaseTypes)
		{
			if (auto res = baseType.type->GetField(name))
				return res;
		}

		return nullptr;
	}

	const FunctionInfo* Type::GetFunction(const String& name) const
	{
		for (auto func : mFunctions)
		{
			if (func->mName == name)
				return func;
		}

		for (auto& base : mBaseTypes)
		{
			if (auto func = base.type->GetFunction(name))
				return func;
		}

		return nullptr;
	}

	const StaticFunctionInfo* Type::GetStaticFunction(const String& name) const
	{
		for (auto func : mStaticFunctions)
		{
			if (func->mName == name)
				return func;
		}

		for (auto& base : mBaseTypes)
		{
			if (auto func = base.type->GetStaticFunction(name))
				return func;
		}

		return nullptr;
	}

	Vector<const Type*> Type::GetDerivedTypes(bool deep /*= true*/) const
	{
		Vector<const Type*> res;
		for (auto type : Reflection::GetTypes())
		{
			auto baseTypes = type->GetBaseTypes();
			for (auto baseType : baseTypes)
			{
				if (baseType.type->mId == mId)
					res.Add(type);
			}
		}

		if (deep)
		{
			auto resCopy = res;
			for (auto type : resCopy)
				res += type->GetDerivedTypes(true);
		}

		return res;
	}

	void* Type::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto field : mFields)
		{
			if (field->mName == pathPart)
			{
				if (delPos == -1)
				{
					fieldInfo = field;
					return field->GetValuePtrStrong(object);
				}
				else
				{
					void* val = field->GetValuePtr(object);

					if (!val)
						return nullptr;

					return field->SearchFieldPtr(val, path.SubStr(delPos + 1), fieldInfo);
				}
			}
		}

		for (auto baseType : mBaseTypes)
		{
			if (auto res = baseType.type->Type::GetFieldPtr((*baseType.dynamicCastUpFunc)(object), path, fieldInfo))
				return res;
		}

		return nullptr;
	}

	void Type::Serialize(void* ptr, DataNode& data) const
	{
		mSerializer->Serialize(ptr, data);
	}

	void Type::Deserialize(void* ptr, const DataNode& data) const
	{
		mSerializer->Deserialize(ptr, data);
	}

	bool Type::IsValueEquals(void* objectA, void* objectB) const
	{
		return mSerializer->Equals(objectA, objectB);
	}

	void Type::CopyValue(void* objectA, void* objectB) const
	{
		mSerializer->Copy(objectA, objectB);
	}

	ITypeSerializer* Type::GetSerializer() const
	{
		return mSerializer;
	}

	VectorType::VectorType(const String& name, int size, ITypeSerializer* serializer) :
		Type(name, size, serializer)
	{}

	Type::Usage VectorType::GetUsage() const
	{
		return Usage::Vector;
	}

	const Type* VectorType::GetElementType() const
	{
		return mElementType;
	}

	void* VectorType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		String pathPart = path.SubStr(0, delPos);

		if (pathPart == "count")
		{
			fieldInfo = mCountFieldInfo;
			return object;
		}

		int count = GetObjectVectorSize(object);
		int idx = (int)pathPart;

		if (idx >= count || idx < 0)
			return nullptr;

		if (delPos < 0)
		{
			fieldInfo = mElementFieldInfo;
			return GetObjectVectorElementPtr(object, idx);
		}

		return mElementType->GetFieldPtr(GetObjectVectorElementPtr(object, idx), path.SubStr(delPos + 1), fieldInfo);
	}

	FieldInfo* VectorType::GetElementFieldInfo() const
	{
		return mElementFieldInfo;
	}

	FieldInfo* VectorType::GetCountFieldInfo() const
	{
		return mCountFieldInfo;
	}

	MapType::MapType(const Type* keyType, const Type* valueType, int size, ITypeSerializer* serializer):
		Type((String)"o2::Dictionary<" + keyType->GetName() + ", " + valueType->GetName() + ">", size, serializer),
		mKeyType(keyType), mValueType(valueType)
	{}

	Type::Usage MapType::GetUsage() const
	{
		return Usage::Map;
	}

	const Type* MapType::GetKeyType() const
	{
		return mKeyType;
	}

	const Type* MapType::GetValueType() const
	{
		return mValueType;
	}

	int MapType::GetObjectDictionarySize(void* object) const
	{
		return mGetDictionaryObjectSizeFunc(object);
	}

	void* MapType::GetObjectDictionaryKeyPtr(void* object, int idx) const
	{
		return mGetObjectDictionaryKeyPtrFunc(object, idx);
	}

	void* MapType::GetObjectDictionaryValuePtr(void* object, int idx) const
	{
		return mGetObjectDictionaryValuePtrFunc(object, idx);
	}

	void* MapType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
// 		int delPos = path.Find("/");
// 		String pathPart = path.SubStr(0, delPos);
// 		DataNode data;
// 		data = pathPart;
// 
// 		int count = GetObjectDictionarySize(object);
// 		int idx = (int)pathPart;
// 
// 		if (idx < count)
// 			return mElementType->GetFieldPtr(GetObjectVectorElementPtr(object, idx), path.SubStr(delPos + 1), fieldInfo);

		return nullptr;
	}

	EnumType::EnumType(const String& name, int size, ITypeSerializer* serializer):
		Type(name, size, serializer)
	{}

	Type::Usage EnumType::GetUsage() const
	{
		return Usage::Enumeration;
	}

	const Map<int, String>& EnumType::GetEntries() const
	{
		return mEntries;
	}

	PointerType::PointerType(const Type* unptrType, ITypeSerializer* serializer):
		Type(unptrType->GetName() + "*", sizeof(void*), serializer),
		mUnptrType(unptrType)
	{}

	Type::Usage PointerType::GetUsage() const
	{
		return Usage::Pointer;
	}

	const Type* PointerType::GetUnpointedType() const
	{
		return mUnptrType;
	}

	void* PointerType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		return mUnptrType->GetFieldPtr(*(void**)object, path, fieldInfo);
	}

	void* PointerType::CreateSample() const
	{
		return mUnptrType->CreateSample();
	}

	IAbstractValueProxy* PointerType::GetValueProxy(void* object) const
	{
		if (auto objectUnptrType = dynamic_cast<const ObjectType*>(mUnptrType))
			return objectUnptrType->GetValueProxy(*(void**)object);

		return mnew PointerValueProxy<void*>((void**)object);
	}

	PropertyType::PropertyType(const String& name, int size, ITypeSerializer* serializer):
		Type(name, size, serializer)
	{}

	Type::Usage PropertyType::GetUsage() const
	{
		return Usage::Property;
	}

	const Type* PropertyType::GetValueType() const
	{
		return mValueType;
	}

	ObjectType::ObjectType(const String& name, int size, void*(*castFromFunc)(void*), void*(*castToFunc)(void*),
						   ITypeSerializer* serializer):
		Type(name, size, serializer), mCastToFunc(castToFunc), mCastFromFunc(castFromFunc)
	{}

	Type::Usage ObjectType::GetUsage() const
	{
		return Usage::Object;
	}

	void* ObjectType::DynamicCastFromIObject(IObject* object) const
	{
		return (*mCastFromFunc)(object);
	}

	IObject* ObjectType::DynamicCastToIObject(void* object) const
	{
		return (IObject*)(*mCastToFunc)(object);
	}

	void* ObjectType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		IObject* iobject = DynamicCastToIObject(object);
		const Type* realType = &iobject->GetType();
		if (realType == this)
			return Type::GetFieldPtr(object, path, fieldInfo);

		return realType->GetFieldPtr(dynamic_cast<const ObjectType*>(realType)->DynamicCastFromIObject(iobject), path, fieldInfo);
	}

	StringPointerAccessorType::StringPointerAccessorType(const String& name, int size, ITypeSerializer* serializer) :
		Type(name, size, serializer)
	{}

	Type::Usage StringPointerAccessorType::GetUsage() const
	{
		return Usage::StringAccessor;
	}

}

ENUM_META_(o2::Type::Usage, Usage)
{
	ENUM_ENTRY(Map);
	ENUM_ENTRY(Enumeration);
	ENUM_ENTRY(Pointer);
	ENUM_ENTRY(Property);
	ENUM_ENTRY(Regular);
	ENUM_ENTRY(StringAccessor);
	ENUM_ENTRY(Vector);
}
END_ENUM_META;
