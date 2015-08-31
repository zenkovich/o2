#include "Types.h"

#include "Utils/IObject.h"

namespace o2
{
	UInt Types::mLastGivenTypeId = 0;

	const Vector<Type*>& Types::GetTypes()
	{
		return mTypes;
	}

	IObject* Types::CreateTypeSample(const String& typeName)
	{
		for (auto type : mTypes)
		{
			if (type->Name() == typeName)
				return type->Sample()->Clone();
		}

		return nullptr;
	}

}