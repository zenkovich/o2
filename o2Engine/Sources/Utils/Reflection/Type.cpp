#include "Type.h"

#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	Type::Type(const String& name, ISampleCreator* creator, int size):
		mId(0), mPtrType(nullptr), mName(name),
		mSampleCreator(creator), mSize(size)
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

	Type::Id Type::ID() const
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

		for (auto type : mBaseTypes)
		{
			if (type->mId == mId)
				return true;

			if (type->IsBasedOn(other))
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

	const Type::TypesVec& Type::GetBaseTypes() const
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
			res += baseType->GetFieldsWithBaseClasses();

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
			res += baseType->GetFunctionsWithBaseClasses();

		res += mFunctions;

		return res;
	}

	const FieldInfo* Type::GetField(const String& name) const
	{
		for (auto field : mFields)
			if (field->GetName() == name)
				return field;

		for (auto baseType : mBaseTypes)
			if (auto res = baseType->GetField(name))
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
			for (auto btype : baseTypes)
			{
				if (btype->mId == mId)
					res.Add(type);
			}
		}
		return res;
	}

	void* Type::CreateSample() const
	{
		return mSampleCreator->CreateSample();
	}

	String Type::GetFieldPath(void* sourceObject, void *targetObject, FieldInfo*& fieldInfo) const
	{
		if (sourceObject == targetObject)
			return "";

		String res;

		for (auto baseType : mBaseTypes)
		{
			auto baseRes = baseType->GetFieldPath(sourceObject, targetObject, fieldInfo);
			if (fieldInfo)
				return baseRes;
		}

		for (auto field : mFields)
		{
			void* fieldObject = field->GetValuePtr(sourceObject);

			if (fieldObject == nullptr)
				continue;

			if (fieldObject == targetObject)
			{
				fieldInfo = field;
				return field->mName;
			}

			Vector<void*> passedObjects;
			passedObjects.Add(sourceObject);
			passedObjects.Add(fieldObject);

			FieldInfo* info = field->SearchFieldPath(fieldObject, targetObject, field->mName, res, passedObjects);
			if (info)
			{
				fieldInfo = info;
				return res;
			}
		}

		return res;
	}

	FieldInfo* Type::SearchFieldPath(void* obj, void* target, const String& path, String& res,
									 Vector<void*>& passedObjects) const
	{
		auto allFields = GetFieldsWithBaseClasses();
		for (auto field : allFields)
		{
			void* fieldObj = field->GetValuePtr(obj);

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

	void* Type::GetFieldPtr(void* object, const String& path, FieldInfo*& fieldInfo) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto baseType : mBaseTypes)
		{
			if (auto res = baseType->GetFieldPtr(object, path, fieldInfo))
				return res;
		}

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

		return nullptr;
	}

	VectorType::VectorType(const String& name, ISampleCreator* creator, int size):
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

		int count = GetObjectVectorSize(object);
		int idx = (int)pathPart;

		if (idx < count)
			return mElementType->GetFieldPtr(GetObjectVectorElementPtr(object, idx), path.SubStr(delPos + 1), fieldInfo);

		return nullptr;
	}

	FieldInfo* VectorType::SearchFieldPath(void* obj, void* target, const String& path, String& res,
										   Vector<void*>& passedObjects) const
	{
		int count = GetObjectVectorSize(obj);

		auto allFields = mElementType->GetFieldsWithBaseClasses();
		for (int i = 0; i < count; i++)
		{
			void* elementPtr = GetObjectVectorElementPtr(obj, i);
			for (auto field : allFields)
			{
				void* fieldObj = field->GetValuePtr(elementPtr);

				if (fieldObj == nullptr)
					continue;

				if (passedObjects.Contains(fieldObj))
					continue;

				passedObjects.Add(fieldObj);

				String newPath = path + "/" + (String)i + "/" + field->GetName();
				if (fieldObj == target)
				{
					res = newPath;
					return field;
				}

				FieldInfo* childField = field->SearchFieldPath(fieldObj, target, newPath, res, passedObjects);
				if (childField)
					return childField;
			}
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
											   Vector<void*>& passedObjects) const
	{
		return nullptr;
	}

	EnumType::EnumType(const String& name, ISampleCreator* creator, int size):
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
		Type(unptrType->GetName() + "*", new SampleCreator<void*>(), sizeof(void*)), mUnptrType(unptrType)
	{}

	Type::Usage PointerType::GetUsage() const
	{
		return Usage::Pointer;
	}

	const Type* PointerType::GetUnpointedType() const
	{
		return mUnptrType;
	}

	PropertyType::PropertyType(const String& name, ISampleCreator* creator, int size):
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

}
 
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
 