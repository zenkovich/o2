#include "Reflection.h"

#include "Utils/IObject.h"

namespace o2
{

	Reflection::Reflection():
		mLastGivenTypeId(0)
	{
		Type::Dummy::type = mnew Type();
	}

	Reflection::~Reflection()
	{
		for (auto type : mTypes)
			delete type;
	}

	const Vector<Type*>& Reflection::GetTypes()
	{
		return instance->mTypes;
	}

	IObject* Reflection::CreateTypeSample(const String& typeName)
	{
		for (auto type : instance->mTypes)
		{
			if (type->Name() == typeName)
				return type->CreateSample();
		}

		return nullptr;
	}

	Type* Reflection::GetType(Type::Id id)
	{
		for (auto type : instance->mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}

	Type* IObject::type = new Type();
}