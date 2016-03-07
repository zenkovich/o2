#include "IActorTransformViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		mDataView = mnew UIWidget();
	}

	IActorTransformViewer::~IActorTransformViewer()
	{
		delete mDataView;
	}

	UIWidget* IActorTransformViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorTransformViewer::Expand()
	{

	}

	void IActorTransformViewer::Collapse()
	{

	}
}