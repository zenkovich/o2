#include "stdafx.h"
#include "IWidgetLayerLayoutViewer.h"

#include "Core/UI/SpoilerWithHead.h"
#include "UI/UIManager.h"
#include "UI/Image.h"

namespace Editor
{
	IWidgetLayerLayoutViewer::IWidgetLayerLayoutViewer()
	{
		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->borderRight = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI2_transform_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IWidgetLayerLayoutViewer::~IWidgetLayerLayoutViewer()
	{}

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
	{}

}

DECLARE_CLASS(Editor::IWidgetLayerLayoutViewer);
