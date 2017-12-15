#include "DefaultActorAnimationViewer.h"

namespace Editor
{
	DefaultActorAnimationViewer::DefaultActorAnimationViewer()
	{

	}

	DefaultActorAnimationViewer::~DefaultActorAnimationViewer()
	{

	}

	void DefaultActorAnimationViewer::SetTargetActors(const Vector<Actor*>& actors)
	{

	}
}

CLASS_META(Editor::DefaultActorAnimationViewer)
{
	BASE_CLASS(Editor::IActorAnimationViewer);


	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
}
END_META;
