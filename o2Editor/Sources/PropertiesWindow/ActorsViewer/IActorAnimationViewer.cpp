#include "stdafx.h"
#include "PropertiesWindow/ActorsViewer/IActorAnimationViewer.h"

#include "Scene/Actor.h"
#include "UI/Widget.h"

namespace Editor
{
	IActorAnimationViewer::IActorAnimationViewer()
	{
		mDataView = mnew UIWidget();
		mDataView->name = "animation viewer";
	}

	IActorAnimationViewer::~IActorAnimationViewer()
	{
		if (mDataView)
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

DECLARE_CLASS(Editor::IActorAnimationViewer);
