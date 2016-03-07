#include "DefaultActorHeaderViewer.h"

#include "UI/Widget.h"

namespace Editor
{
	DefaultActorHeaderViewer::DefaultActorHeaderViewer()
	{
		mDataView = mnew UIWidget();
	}

	DefaultActorHeaderViewer::~DefaultActorHeaderViewer()
	{
		delete mDataView;
	}

	void DefaultActorHeaderViewer::SetTargetActors(const Vector<Actor*>& actors)
	{

	}

	UIWidget* DefaultActorHeaderViewer::GetWidget() const
	{
		return mDataView;
	}
}