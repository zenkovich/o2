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
// Editor actor animation viewer interface
// ---------------------------------------
class IEditorActorAnimationViewer: public IObject
{
public:
	// Default constructor. Initializes data widget
	IEditorActorAnimationViewer();

	// Virtual destructor
	virtual ~IEditorActorAnimationViewer();

	// Sets target actors
	virtual void SetTargetActors(const Vector<Actor*>& actors) {}

	// Returns data widget
	virtual UIWidget* GetWidget() const;

	// Expands view
	void Expand();

	// Collapse view
	void Collapse();

	IOBJECT(IEditorActorAnimationViewer);

protected:
	UIWidget* mDataView; // Data view widget
};