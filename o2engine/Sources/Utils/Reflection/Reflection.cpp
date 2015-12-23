#include "Reflection.h"

#include "Utils/IObject.h"

namespace o2
{

	Reflection::Reflection()
	{
		Type::Dummy::type = mnew Type();
		Type::Dummy::type.SetupAsRoot();
	}

	Reflection::~Reflection()
	{
		for (auto type : mTypes)
			type.Release();
	}

	const Vector<Ptr<Type>>& Reflection::GetTypes()
	{
		return instance->mTypes;
	}

	IObject* Reflection::CreateTypeSample(const String& typeName)
	{
		for (auto type : instance->mTypes)
		{
			if (type->Name() == typeName)
				return type->Sample()->Clone();
		}

		return nullptr;
	}

	Ptr<Type> Reflection::GetType(Type::Id id)
	{
		for (auto type : instance->mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}	
}