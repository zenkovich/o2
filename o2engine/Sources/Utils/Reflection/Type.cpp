#include "Type.h"

#include "Utils/Reflection/Reflection.h"
#include "Utils/IObject.h"

namespace o2
{
	Type::Type():
		mId(0)
	{}

	Type::~Type()
	{
		for (auto field : mFields)
			delete field;
	}

	const String& Type::Name() const
	{
		return mName;
	}

	Type::Id Type::ID() const
	{
		return mId;
	}

	const Type::TypesVec& Type::BaseTypes() const
	{
		return mBaseTypes;
	}

	const Type::FieldInfosVec& Type::Fields() const
	{
		return mFields;
	}

	const FieldInfo* Type::Field(const String& name) const
	{
		for (auto field : mFields)
			if (field->Name() == name)
				return field;

		return nullptr;
	}

	Type::TypesVec Type::DerivedTypes() const
	{
		TypesVec res;
		for (auto type : Reflection::GetTypes())
		{
			auto baseTypes = type->BaseTypes();
			for (auto btype : baseTypes)
			{
				if (btype->mId == mId)
					res.Add(type);
			}
		}
		return res;
	}

	IObject* Type::CreateSample() const
	{
		return mTypeCreator->Create();
	}

	String Type::GetFieldPath(void* sourceObject, void *targetObject, FieldInfo*& fieldInfo) const
	{
		if (sourceObject == targetObject)
			return "";

		String res;

		for (auto field : mFields)
		{
			char* fieldObject = field->GetValuePtr<char>(sourceObject);

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

	void Type::AddBaseType(Type* baseType)
	{
		mBaseTypes.Add(baseType);

		for (auto field : baseType->mFields)
			mFields.Insert(field->Clone(), 0);
	}

	bool Type::operator!=(const Type& other) const
	{
		return other.mId != mId;
	}

	bool Type::operator==(const Type& other) const
	{
		return other.mId == mId;
	}

	Type* Type::Dummy::type;
}