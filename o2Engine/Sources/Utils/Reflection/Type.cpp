#include "Type.h"

#include "Utils/Reflection/Reflection.h"
#include "Utils/IObject.h"

namespace o2
{
	Type::Type(const String& name, ISampleCreator* creator, int size):
		mId(0), mPointer(0), mPtrType(nullptr), mUnptrType(nullptr), mName(name),
		mSampleCreator(creator), mSize(size)
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

	int Type::Size() const
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

	const Type* Type::GetUnpointedType() const
	{
		if (mUnptrType)
			return mUnptrType;

		return this;
	}

	bool Type::IsPointer() const
	{
		return mPointer > 0;
	}

	const Type::TypesVec& Type::BaseTypes() const
	{
		return mBaseTypes;
	}

	const Type::FieldInfosVec& Type::Fields() const
	{
		return mFields;
	}

	Type::FieldInfosVec Type::AllFields() const
	{
		FieldInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType->AllFields();

		res += mFields;

		return res;
	}

	const Type::FunctionsInfosVec& Type::Functions() const
	{
		return mFunctions;
	}

	Type::FunctionsInfosVec Type::AllFunctions() const
	{
		FunctionsInfosVec res;

		for (auto baseType : mBaseTypes)
			res += baseType->AllFunctions();

		res += mFunctions;

		return res;
	}

	const FieldInfo* Type::Field(const String& name) const
	{
		for (auto field : mFields)
			if (field->Name() == name)
				return field;

		for (auto baseType : mBaseTypes)
			if (auto res = baseType->Field(name))
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

	Vector<const Type*> Type::DerivedTypes() const
	{
		Vector<const Type*> res;
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

	void Type::SetName(const String& name)
	{
		mName = name;

		if (mPtrType)
			mPtrType->SetName(name + "*");
	}

	bool Type::operator!=(const Type& other) const
	{
		return other.mId != mId;
	}

	bool Type::operator==(const Type& other) const
	{
		return other.mId == mId;
	}
}