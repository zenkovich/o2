#pragma once

#include "PropertiesWindow/ActorsViewer/IEditorActorTransformViewer.h"

// -------------------------------------
// Default editor actor transform viewer
// -------------------------------------
class DefaultEditorActorTransformViewer: public IEditorActorTransformViewer
{
public:
	// Default constructor. Initializes data widget
	DefaultEditorActorTransformViewer();

	// Virtual destructor
	~DefaultEditorActorTransformViewer();

	// Sets target actors
	void SetTargetActors(const Vector<Actor*>& actors);

	IOBJECT(DefaultEditorActorTransformViewer);
};
