#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Component;
	class UIWidget;
}

// ---------------------------------------
// Editor actor component viewer interface
// ---------------------------------------
class IEditorActorComponentViewer: public IObject
{
public:
	// Default constructor. Initializes data widget
	IEditorActorComponentViewer();

	// Virtual destructor
	virtual ~IEditorActorComponentViewer();

	// Sets target actors
	virtual void SetTargetComponents(const Vector<Component*>& components) {}

	// Returns viewing component type 
	virtual const Type* GetComponentType() const { return nullptr; }

	// Returns data widget
	virtual UIWidget* GetWidget() const;

	// Expands view
	void Expand();

	// Collapse view
	void Collapse();

	IOBJECT(IEditorActorComponentViewer);

protected:
	UIWidget* mDataView; // Data view widget
};
