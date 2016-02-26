#include "DefaultEditorActorHeaderViewer.h"

#include "UI/Widget.h"

DefaultEditorActorHeaderViewer::DefaultEditorActorHeaderViewer()
{
	mDataView = mnew UIWidget();
}

DefaultEditorActorHeaderViewer::~DefaultEditorActorHeaderViewer()
{
	delete mDataView;
}

void DefaultEditorActorHeaderViewer::SetTargetActors(const Vector<Actor*>& actors)
{

}

UIWidget* DefaultEditorActorHeaderViewer::GetWidget() const
{
	return mDataView;
}
