#include "stdafx.h"
#include "IActorTransformViewer.h"

#include "Core/UI/SpoilerWithHead.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "UI/Button.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

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
