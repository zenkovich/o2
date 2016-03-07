#include "PropertiesWindow/ActorsViewer/IActorAnimationViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

namespace Editor
{
	IActorAnimationViewer::IActorAnimationViewer()
	{
		mDataView = mnew UIWidget();
	}

	IActorAnimationViewer::~IActorAnimationViewer()
	{
		delete mDataView;
	}

	UIWidget* IActorAnimationViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorAnimationViewer::Expand()
	{

	}

	void IActorAnimationViewer::Collapse()
	{

	}

}