#include "IActorHeaderViewer.h"

#include "UI/Widget.h"

CLASS_META(Editor::IActorHeaderViewer)
{
	BASE_CLASS(o2::IObject);


	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
