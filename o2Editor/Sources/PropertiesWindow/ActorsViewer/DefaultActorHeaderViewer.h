#pragma once

#include "PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"

namespace Editor
{
	// ---------------------------
	// Default actor header viewer
	// ---------------------------
	class DefaultActorHeaderViewer: public IActorHeaderViewer
	{
	public:
		// Default constructor
		DefaultActorHeaderViewer();

		// Virtual destructor
		~DefaultActorHeaderViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		// Returns data widget
		UIWidget* GetWidget() const;

		IOBJECT(DefaultActorHeaderViewer);

	public:
		UIWidget* mDataView;
	};
}