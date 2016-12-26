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
}
