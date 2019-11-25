#include "stdafx.h"
#include "IActorTransformViewer.h"

#include "Core/EditorScope.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->borderRight = 5;
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

}

DECLARE_CLASS(Editor::IActorTransformViewer);
