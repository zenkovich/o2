#pragma once

#include "PropertiesWindow/ActorsViewer/IEditorActorAnimationViewer.h"

// -------------------------------------
// Default editor actor animation viewer
// -------------------------------------
class DefaultEditorActorAnimationViewer: public IEditorActorAnimationViewer
{
public:
	// Default constructor. Initializes data widget
	DefaultEditorActorAnimationViewer();

	// Virtual destructor
	~DefaultEditorActorAnimationViewer();

	// Sets target actors
	void SetTargetActors(const Vector<Actor*>& actors);

	IOBJECT(DefaultEditorActorAnimationViewer);
};
