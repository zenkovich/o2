#include "stdafx.h"
#include "IActorTransformViewer.h"

#include "Core/EditorScope.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/UI/Button.h"
#include "Scene/UI/Image.h"
#include "Scene/UI/Label.h"
#include "Scene/UI/Spoiler.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/VerticalLayout.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		PushScopeEnterOnStack scope;

		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

		mSpoiler->borderBottom = 5;
		mSpoiler->borderRight = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI2_transform_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IActorTransformViewer::~IActorTransformViewer()
	{}

	UIWidget* IActorTransformViewer::GetWidget() const
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
