#include "o2Editor/stdafx.h"
#include "IWidgetLayerLayoutViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	IWidgetLayerLayoutViewer::IWidgetLayerLayoutViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon_white.png");

		mSpoiler->SetExpanded(true);
	}

	IWidgetLayerLayoutViewer::~IWidgetLayerLayoutViewer()
	{}

	Ref<Widget> IWidgetLayerLayoutViewer::GetWidget() const
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

	void IWidgetLayerLayoutViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IWidgetLayerLayoutViewer::IsEnabled() const
	{
		return mEnabled;
	}

}
// --- META ---

DECLARE_CLASS(Editor::IWidgetLayerLayoutViewer, Editor__IWidgetLayerLayoutViewer);
// --- END META ---
