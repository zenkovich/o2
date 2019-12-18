#include "stdafx.h"
#include "IWidgetLayerPropertiesViewer.h"

#include "Core/EditorScope.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/UIManager.h"

namespace Editor
{

	IWidgetLayerPropertiesViewer::IWidgetLayerPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderRight = 5;
		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IWidgetLayerPropertiesViewer::~IWidgetLayerPropertiesViewer()
	{}

	Widget* IWidgetLayerPropertiesViewer::GetWidget() const
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

	bool IWidgetLayerPropertiesViewer::IsEmpty() const
	{
		return true;
	}

}

DECLARE_CLASS(Editor::IWidgetLayerPropertiesViewer);
