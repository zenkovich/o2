#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

// -------------------------------
// Editor property field interface
// -------------------------------
class IEditorPropertyField: public IObject
{
public:
	// Virtual destructor
	virtual ~IEditorPropertyField() {}

	// Sets targets pointers
	virtual void Setup(const Vector<void*>& targets, bool isProperty) {}

	// Checks common value and fill fields
	virtual void Update() {}

	// Returns control widget
	virtual UIWidget* GetWidget() const { return nullptr; }

	// Returns editing by this field type
	virtual const Type* GetFieldType() const { return nullptr; }

	IOBJECT(IEditorPropertyField);
};
