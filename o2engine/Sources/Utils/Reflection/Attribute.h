#pragma once

namespace o2
{
	class FieldInfo;

	// ----------------------------------------
	// Attribute interface. Using in reflection
	// ----------------------------------------
	class IAttribute
	{
		friend class FieldInfo;

	protected:
		FieldInfo* mOwnerFieldInfo; // Pointer to owner field info object

	public:
		// Default constructor
		IAttribute(): mOwnerFieldInfo(nullptr) {}

		// Virtual destructor
		virtual ~IAttribute() {}

		// Returns copy of this
		virtual IAttribute* Clone() const = 0;
	};
}
