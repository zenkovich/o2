#include "stdafx.h"
#include "Type.h"

#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Animation/Animation.h"

namespace o2
{
	Type::Type(const String& name, ITypeSampleCreator* creator, int size):
		mId(0), mPtrType(nullptr), mName(name), mSampleCreator(creator), mSize(size)
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

	const Type* Type::GetPointerType() const
	{
		if (!mPtrType)
			Reflection::InitializePointerType(this);

		return mPtrType;
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

	Type::FunctionsInfosVec Type::GetFunctionsWithBaseClasses() const
	{
		FunctionsInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType.type->GetFunctionsWithBaseClasses();

		res += mFunctions;

		return res;
	}

	const FieldInfo* Type::GetField(const String& name) const
	{
		for (auto field : mFields)
			if (field->GetName() == name)
				return field;

		for (auto baseType : mBaseTypes)
			if (auto res = baseType.type->GetField(name))
				return res;

		return nullptr;
	}

	const FunctionInfo* Type::GetFunction(const String& name) const
	{
		for (auto func : mFunctions)
			if (func->mName == name)
				return func;

		return nullptr;
	}

	Vector<const Type*> Type::GetDerivedTypes() const
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
		return res;
	}

	void* Type::CreateSample() const
	{
		return mSampleCreator->CreateSample();
	}

	String Type::GetFieldPath(void* object, void *targetObject, FieldInfo*& fieldInfo) const
	{
		if (object == targetObject)
			return "";

		String res;
		Vector<SearchPassedObject> passedObjects;

		FieldInfo* info = SearchFieldPath(object, targetObject, "", res, passedObjects);
		if (info)
		{
			fieldInfo = info;
			res.Erase(0, 1);
		}

		return res;
	}

	FieldInfo* Type::SearchFieldPath(void* obj, void* target, const String& path, String& res,
									 Vector<SearchPassedObject>& passedObjects) const
	{
		for (auto field : mFields)
		{
			if (field->HasAttribute<ExcludePointerSearchAttribute>())
				continue;

			void* fieldObj = field->GetValuePtr(obj);

			if (fieldObj == nullptr)
				continue;

			if (passedObjects.Contains(SearchPassedObject(fieldObj, field->GetType())))
				continue;

			passedObjects.Add(SearchPassedObject(fieldObj, field->GetType()));
			//printf((path + "/" + field->mName + "\n").Data());

			if (fieldObj == target)
			{
				res = path + "/" + field->mName;
				return field;
			}

			FieldInfo* childField = field->SearchFieldPath(fieldObj, target, path + "/" + field->mName, res, passedObjects);
			if (childField)
				return childField;
		}

		for (auto baseType : mBaseTypes)
		{
			auto baseRes = baseType.type->SearchFieldPath((*baseType.dynamicCastFunc)(obj), target, path, res, passedObjects);
			if (baseRes)
				return baseRes;
		}

		return nullptr;
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
					return field->GetValuePtr(object);
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
			if (auto res = baseType.type->GetFieldPtr((*baseType.dynamicCastFunc)(object), path, fieldInfo))
				return res;
		}

		return nullptr;
	}

	VectorType::VectorType(const String& name, ITypeSampleCreator* creator, int size):
		Type(name, creator, size)
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

	FieldInfo* VectorType::SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<SearchPassedObject>& passedObjects) const
	{
		int count = GetObjectVectorSize(obj);

		for (int i = 0; i < count; i++)
		{
			void* elementPtr = GetObjectVectorElementPtr(obj, i);
			FieldInfo* fieldInfo = mElementType->SearchFieldPath(elementPtr, target, path + "/" + (String)i, res, passedObjects);
			if (fieldInfo)
				return fieldInfo;
		}

		return nullptr;
	}

	Type::Usage DictionaryType::GetUsage() const
	{
		return Usage::Dictionary;
	}

	const Type* DictionaryType::GetKeyType() const
	{
		return mKeyType;
	}

	const Type* DictionaryType::GetValueType() const
	{
		return mValueType;
	}

	int DictionaryType::GetObjectDictionarySize(void* object) const
	{
		return mGetDictionaryObjectSizeFunc(object);
	}

	void DictionaryType::SetObjectDictionarySize(void* object, int size) const
	{
		mSetDictionaryObjectSizeFunc(object, size);
	}

	void* DictionaryType::GetObjectDictionaryKeyPtr(void* object, int idx) const
	{
		return mGetObjectDictionaryKeyPtrFunc(object, idx);
	}

	void* DictionaryType::GetObjectDictionaryValuePtr(void* object, int idx) const
	{
		return mGetObjectDictionaryValuePtrFunc(object, idx);
	}

	void* DictionaryType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
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

	FieldInfo* DictionaryType::SearchFieldPath(void* obj, void* target, const String& path, String& res,
											   Vector<SearchPassedObject>& passedObjects) const
	{
		return nullptr;
	}

	EnumType::EnumType(const String& name, ITypeSampleCreator* creator, int size):
		Type(name, creator, size)
	{}

	Type::Usage EnumType::GetUsage() const
	{
		return Usage::Enumeration;
	}

	const Dictionary<int, String>& EnumType::GetEntries() const
	{
		return mEntries;
	}

	PointerType::PointerType(const Type* unptrType):
		Type(unptrType->GetName() + "*", new TypeSampleCreator<void*>(), sizeof(void*)),
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

	FieldInfo* PointerType::SearchFieldPath(void* obj, void* target, const String& path, String& res,
											Vector<SearchPassedObject>& passedObjects) const
	{
		return mUnptrType->SearchFieldPath(*(void**)obj, target, path, res, passedObjects);
	}

	void* PointerType::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		return mUnptrType->GetFieldPtr(*(void**)object, path, fieldInfo);
	}

	PropertyType::PropertyType(const String& name, ITypeSampleCreator* creator, int size):
		Type(name, creator, size)
	{}

	Type::Usage PropertyType::GetUsage() const
	{
		return Usage::Property;
	}

	const Type* PropertyType::GetValueType() const
	{
		return mValueType;
	}

	ObjectType::ObjectType(const String& name, ITypeSampleCreator* creator, int size,
						   void*(*castFromFunc)(void*), void*(*castToFunc)(void*)):
		Type(name, creator, size), mCastToFunc(castToFunc), mCastFromFunc(castFromFunc)
	{}

	Type::Usage ObjectType::GetUsage() const
	{
		return Usage::Regular;
	}

	void* ObjectType::DynamicCastFromIObject(IObject* object) const
	{
		return (*mCastFromFunc)(object);
	}

	IObject* ObjectType::DynamicCastToIObject(void* object) const
	{
		return (IObject*)(*mCastToFunc)(object);
	}
}

ENUM_META_(o2::Type::Usage, Usage)
{
	ENUM_ENTRY(Dictionary);
	ENUM_ENTRY(Enumeration);
	ENUM_ENTRY(Pointer);
	ENUM_ENTRY(Property);
	ENUM_ENTRY(Regular);
	ENUM_ENTRY(StringAccessor);
	ENUM_ENTRY(Vector);
}
END_ENUM_META;
