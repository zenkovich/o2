#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Actor;
	class UIWidget;
}

// ---------------------------------------
// Editor actor transform viewer interface
// ---------------------------------------
class IEditorActorTransformViewer: public IObject
{
public:
	// Default constructor. Initializes data widget
	IEditorActorTransformViewer();

	// Virtual destructor
	virtual ~IEditorActorTransformViewer();

	// Sets target actors
	virtual void SetTargetActors(const Vector<Actor*>& actors) {}

	// Returns data widget
	virtual UIWidget* GetWidget() const;

	// Expands view
	void Expand();

	// Collapse view
	void Collapse();

	IOBJECT(IEditorActorTransformViewer);

protected:
	UIWidget* mDataView; // Data view widget
};