#pragma once

namespace o2
{
	class Type;

	struct SearchPassedObject
	{
		void* object = nullptr;
		const Type* type = nullptr;

		SearchPassedObject() {}

		SearchPassedObject(void* object, const Type* type): object(object), type(type) {}

		bool operator==(const SearchPassedObject& other) const { return object == other.object && type == other.type; }
	};
}
