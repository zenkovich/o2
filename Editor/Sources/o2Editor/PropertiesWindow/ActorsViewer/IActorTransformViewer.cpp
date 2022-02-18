#include "o2Editor/stdafx.h"
#include "IActorTransformViewer.h"

#include "o2/Render/Sprite.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon_white.png");

		mSpoiler->SetExpanded(true);
	}

	IActorTransformViewer::~IActorTransformViewer()
	{}

	Widget* IActorTransformViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorTransformViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorTransformViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorTransformViewer::Refresh()
	{}

	void IActorTransformViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IActorTransformViewer::IsEnabled() const
	{
		return mEnabled;
	}

}

DECLARE_CLASS(Editor::IActorTransformViewer);
