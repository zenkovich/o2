#pragma once

#include "PropertiesWindow/ActorsViewer/IActorAnimationViewer.h"

namespace Editor
{
	// -------------------------------------
	// Default editor actor animation viewer
	// -------------------------------------
	class DefaultActorAnimationViewer: public IActorAnimationViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorAnimationViewer();

		// Virtual destructor
		~DefaultActorAnimationViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		IOBJECT(DefaultActorAnimationViewer);
	};
}