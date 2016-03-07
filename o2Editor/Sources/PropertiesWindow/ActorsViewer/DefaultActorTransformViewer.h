#pragma once

#include "PropertiesWindow/ActorsViewer/IActorTransformViewer.h"

namespace Editor
{
	// -------------------------------------
	// Default editor actor transform viewer
	// -------------------------------------
	class DefaultActorTransformViewer: public IActorTransformViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorTransformViewer();

		// Virtual destructor
		~DefaultActorTransformViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		IOBJECT(DefaultActorTransformViewer);
	};
}