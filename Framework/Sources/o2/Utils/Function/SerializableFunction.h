#pragma once
#include "o2/Utils/Serialization/DataValue.h"

namespace o2
{
	// --------------------------------------------------------------------------
	// The interface of the function to be serialized. This function can be saved
	// --------------------------------------------------------------------------
	class ISerializableFunction
	{
	public:
		// Serializes a function
		virtual void Serialize(DataValue& data) const {}

		// Deserializes a function
		virtual void Deserialize(const DataValue& data) {}
	};
}
