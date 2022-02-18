#include "o2Editor/stdafx.h"
#include "IWidgetLayerPropertiesViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	IWidgetLayerPropertiesViewer::IWidgetLayerPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon_white.png");

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

	void IWidgetLayerPropertiesViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IWidgetLayerPropertiesViewer::IsEnabled() const
	{
		return mEnabled;
	}

}

DECLARE_CLASS(Editor::IWidgetLayerPropertiesViewer);
