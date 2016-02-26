#include "DefaultEditorActorComponentViewer.h"

#include "Scene/Component.h"

DefaultEditorActorComponentViewer::DefaultEditorActorComponentViewer()
{

}

DefaultEditorActorComponentViewer::~DefaultEditorActorComponentViewer()
{

}

void DefaultEditorActorComponentViewer::SetTargetActors(const Vector<Actor*>& actors)
{

}

Type* DefaultEditorActorComponentViewer::GetComponentType() const
{
	return &Component::type;
}
