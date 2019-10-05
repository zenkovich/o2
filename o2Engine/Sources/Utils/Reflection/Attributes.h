#pragma once

namespace o2
{
	class FieldInfo;

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

	class ExcludePointerSearchAttribute : public o2::IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("EXCLUDE_POINTER_SEARCH");
		ATTRIBUTE_SHORT_DEFINITION("EXCLUDE_POINTER_SEARCH_ATTRIBUTE()");
	};

#define EXCLUDE_POINTER_SEARCH_ATTRIBUTE() \
    AddAttribute(new ExcludePointerSearchAttribute())
}
