#pragma once

#include "PropertiesWindow/ActorsViewer/IEditorActorComponentViewer.h"
#include "PropertiesWindow/Properties/IPropertyField.h"

// ------------------------------
// Default actor component viewer
// ------------------------------
class DefaultEditorActorComponentViewer: public IEditorActorComponentViewer
{
public:
	// Default constructor. Initializes data widget
	DefaultEditorActorComponentViewer();

	// Virtual destructor
	~DefaultEditorActorComponentViewer();

	// Sets target actors
	void SetTargetComponents(const Vector<Component*>& components);

	// Returns viewing component type 
	const Type* GetComponentType() const;

	IOBJECT(DefaultEditorActorComponentViewer);

protected:
	Vector<Component*>            mTargetComponents;   // Target components
	Vector<IEditorPropertyField*> mUsedPropertyFields; // Used property fields
};
