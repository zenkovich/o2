#include "PropertiesWindow/ActorsViewer/IEditorActorAnimationViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

IEditorActorAnimationViewer::IEditorActorAnimationViewer()
{
	mDataView = mnew UIWidget();
}

IEditorActorAnimationViewer::~IEditorActorAnimationViewer()
{
	delete mDataView;
}

UIWidget* IEditorActorAnimationViewer::GetWidget() const
{
	return mDataView;
}

void IEditorActorAnimationViewer::Expand()
{

}

void IEditorActorAnimationViewer::Collapse()
{

}

