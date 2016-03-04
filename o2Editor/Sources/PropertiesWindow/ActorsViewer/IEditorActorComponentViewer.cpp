#include "IEditorActorComponentViewer.h"

#include "UI/Widget.h"
#include "UI/VerticalLayout.h"

IEditorActorComponentViewer::IEditorActorComponentViewer()
{
	auto layout = mnew UIVerticalLayout();
	mDataView = layout;

	layout->spacing = 5.0f;
	layout->border = RectF();
	layout->expandHeight = false;
	layout->expandWidth = true;
	layout->fitByChildren = true;
	layout->baseCorner = BaseCorner::Top;
	layout->layout = UIWidgetLayout::BothStretch();
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
