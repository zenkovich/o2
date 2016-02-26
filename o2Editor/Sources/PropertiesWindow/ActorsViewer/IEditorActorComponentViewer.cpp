#include "IEditorActorComponentViewer.h"

#include "UI/Widget.h"

IEditorActorComponentViewer::IEditorActorComponentViewer()
{
	mDataView = mnew UIWidget();
}

IEditorActorComponentViewer::~IEditorActorComponentViewer()
{
	delete mDataView;
}

UIWidget* IEditorActorComponentViewer::GetWidget() const
{
	return mDataView;
}

void IEditorActorComponentViewer::Expand()
{

}

void IEditorActorComponentViewer::Collapse()
{

}
