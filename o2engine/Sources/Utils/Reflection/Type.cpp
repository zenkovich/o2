#include "Type.h"

#include "Utils/Reflection/Types.h"
#include "Utils/IObject.h"

namespace o2
{
	Type::Type():
		mId(0)
	{
	}

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
		for (auto type : Types::GetTypes())
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

	const IObject* Type::Sample() const
	{
		return mSample;
	}

	Ptr<IObject> Type::CreateSample() const
	{
		return mSample->Clone();
	}

	String Type::GetFieldPath(void* sourceObject, void *targetObject) const
	{
		if (sourceObject == targetObject)
			return "";

		String res;

		for (auto field : mFields)
		{
			char* fieldObject = field->GetValuePtr<char>(sourceObject);

			if (fieldObject == targetObject)
				return field->mName;

			if (field->SearchFieldPath(fieldObject, targetObject, field->mName, res))
				return res;
		}

		return res;
	}

	FieldInfo* Type::FindFieldInfo(void* sourceObject, void *targetObject, String &res) const
	{
		for (auto field : mFields)
		{
			char* fieldObject = field->GetValuePtr<char>(sourceObject);

			if (fieldObject == targetObject)
			{
				res = field->mName;
				return field;
			}

			if (!fieldObject)
				continue;

			FieldInfo* childField = field->SearchFieldPath(fieldObject, targetObject, field->mName, res);
			if (childField)
				return childField;
		}

		return nullptr;
	}

	void Type::Initialize(Type& type, const String& name, UInt id, IObject* sample)
	{
		type.mName = name;
		type.mId = id;
		type.mSample = sample;
	}

	void Type::SetupBaseType(Type& type, Type* baseType)
	{
		type.mBaseTypes.Add(baseType);

		for (auto field : baseType->mFields)
			type.mFields.Add(field->Clone());
	}

	bool Type::operator!=(const Type& other) const
	{
		return other.mId != mId;
	}

	bool Type::operator==(const Type& other) const
	{
		return other.mId == mId;
	}

	Ptr<Type> Type::Dummy::type;
}