#pragma once

#include "PropertiesWindow/ActorsViewer/IEditorActorComponentViewer.h"

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
	void SetTargetActors(const Vector<Actor*>& actors);

	// Returns viewing component type 
	Type* GetComponentType() const;

	IOBJECT(DefaultEditorActorComponentViewer);
};
