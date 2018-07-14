#include "stdafx.h"
#include "IWidgetLayerPropertiesViewer.h"

#include "Core/UI/SpoilerWithHead.h"

namespace Editor
{

	IWidgetLayerPropertiesViewer::IWidgetLayerPropertiesViewer()
	{

	}

	IWidgetLayerPropertiesViewer::~IWidgetLayerPropertiesViewer()
	{

	}

	UIWidget* IWidgetLayerPropertiesViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IWidgetLayerPropertiesViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IWidgetLayerPropertiesViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IWidgetLayerPropertiesViewer::Refresh()
	{

	}

	void IWidgetLayerPropertiesViewer::Rebuild()
	{

	}

	bool IWidgetLayerPropertiesViewer::IsEmpty() const
	{
		return true;
	}

	bool IWidgetLayerPropertiesViewer::IsBuiltWithHiddenFields() const
	{
		return false;
	}

}

DECLARE_CLASS(Editor::IWidgetLayerPropertiesViewer);
