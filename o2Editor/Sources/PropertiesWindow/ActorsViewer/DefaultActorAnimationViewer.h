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

CLASS_BASES_META(Editor::DefaultActorAnimationViewer)
{
	BASE_CLASS(Editor::IActorAnimationViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorAnimationViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorAnimationViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
}
END_META;
