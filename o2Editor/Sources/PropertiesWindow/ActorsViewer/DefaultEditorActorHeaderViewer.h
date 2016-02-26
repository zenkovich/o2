#pragma once

#include "PropertiesWindow/ActorsViewer/IEditorActorHeaderViewer.h"

// ---------------------------
// Default actor header viewer
// ---------------------------
class DefaultEditorActorHeaderViewer: public IEditorActorHeaderViewer
{
public:
	// Default constructor
	DefaultEditorActorHeaderViewer();

	// Virtual destructor
	~DefaultEditorActorHeaderViewer();

	// Sets target actors
	void SetTargetActors(const Vector<Actor*>& actors);

	// Returns data widget
	UIWidget* GetWidget() const;

	IOBJECT(DefaultEditorActorHeaderViewer);

public:
	UIWidget* mDataView;
};
