#include "o2Editor/stdafx.h"
#include "IWidgetLayerLayoutViewer.h"

#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/UIManager.h"

namespace Editor
{
	IWidgetLayerLayoutViewer::IWidgetLayerLayoutViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->borderRight = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IWidgetLayerLayoutViewer::~IWidgetLayerLayoutViewer()
	{}

	Widget* IWidgetLayerLayoutViewer::GetWidget() const
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
