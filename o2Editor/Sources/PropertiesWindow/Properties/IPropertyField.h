#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	// -------------------------------
	// Editor property field interface
	// -------------------------------
	class IPropertyField: public IObject
	{
	public:
		Function<void()> onChanged; // Change value by user event

		// Virtual destructor
		virtual ~IPropertyField() {}

		// Sets targets pointers
		virtual void Setup(const Vector<void*>& targets, bool isProperty) {}

		// Checks common value and fill fields
		virtual void Refresh() {}

		// Returns control widget
		virtual UIWidget* GetWidget() const { return nullptr; }

		// Returns editing by this field type
		virtual const Type* GetFieldType() const { return &TypeOf(void); }

		// Specializes field type
		virtual void SpecializeType(const Type* type) {}

		IOBJECT(IPropertyField);
	};
}