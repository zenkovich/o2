#include "IEditorActorTransformViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

IEditorActorTransformViewer::IEditorActorTransformViewer()
{
	mDataView = mnew UIWidget();
}

IEditorActorTransformViewer::~IEditorActorTransformViewer()
{
	delete mDataView;
}

UIWidget* IEditorActorTransformViewer::GetWidget() const
{
	return mDataView;
}

void IEditorActorTransformViewer::Expand()
{

}

void IEditorActorTransformViewer::Collapse()
{

}
