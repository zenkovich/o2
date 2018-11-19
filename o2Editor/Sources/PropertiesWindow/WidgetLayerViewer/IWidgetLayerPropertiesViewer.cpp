#include "stdafx.h"
#include "IWidgetLayerPropertiesViewer.h"

#include "Core/UI/SpoilerWithHead.h"
#include "UI/Image.h"
#include "UI/UIManager.h"

namespace Editor
{

	IWidgetLayerPropertiesViewer::IWidgetLayerPropertiesViewer()
	{
		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderRight = 5;
		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI2_transform_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IWidgetLayerPropertiesViewer::~IWidgetLayerPropertiesViewer()
	{}

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
	{}

	void IWidgetLayerPropertiesViewer::Rebuild()
	{}

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
