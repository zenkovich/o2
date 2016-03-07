#include "IActorComponentViewer.h"

#include "UI/Widget.h"
#include "UI/VerticalLayout.h"

namespace Editor
{
	IActorComponentViewer::IActorComponentViewer()
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

	IActorComponentViewer::~IActorComponentViewer()
	{
		delete mDataView;
	}

	UIWidget* IActorComponentViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorComponentViewer::Expand()
	{

	}

	void IActorComponentViewer::Collapse()
	{

	}

}