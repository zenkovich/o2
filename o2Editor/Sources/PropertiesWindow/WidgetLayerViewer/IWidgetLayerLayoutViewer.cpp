#include "stdafx.h"
#include "IWidgetLayerLayoutViewer.h"

#include "Core/UI/SpoilerWithHead.h"

namespace Editor
{

	IWidgetLayerLayoutViewer::IWidgetLayerLayoutViewer()
	{

	}

	IWidgetLayerLayoutViewer::~IWidgetLayerLayoutViewer()
	{

	}

	UIWidget* IWidgetLayerLayoutViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IWidgetLayerLayoutViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IWidgetLayerLayoutViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IWidgetLayerLayoutViewer::Refresh()
	{

	}

}

DECLARE_CLASS(Editor::IWidgetLayerLayoutViewer);
