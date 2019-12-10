#pragma once

namespace o2
{
	class FieldInfo;
	class Type;

	// ----------------------------------------
	// Attribute interface. Using in reflection
	// ----------------------------------------
	class IAttribute
	{
	public:
		// Default constructor
		IAttribute(): mOwnerFieldInfo(nullptr) {}

		// Virtual destructor
		virtual ~IAttribute() {}

	protected:
		FieldInfo* mOwnerFieldInfo; // Pointer to owner field info object

		friend class FieldInfo;
	};

#define ATTRIBUTE_COMMENT_DEFINITION(X)
#define ATTRIBUTE_SHORT_DEFINITION(X)
}
