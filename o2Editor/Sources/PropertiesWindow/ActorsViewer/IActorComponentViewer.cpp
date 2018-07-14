#include "stdafx.h"
#include "IActorComponentViewer.h"

#include "Core/UI/SpoilerWithHead.h"
#include "UI/Image.h"
#include "UI/UIManager.h"

namespace Editor
{
	IActorComponentViewer::IActorComponentViewer()
	{
		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI2_big_file_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IActorComponentViewer::~IActorComponentViewer()
	{
		if (mSpoiler)
			delete mSpoiler;
	}

	UIWidget* IActorComponentViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorComponentViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorComponentViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorComponentViewer::Refresh()
	{}

	void IActorComponentViewer::Rebuild()
	{}

	bool IActorComponentViewer::IsBuiltWithEmpty() const
	{
		return false;
	}

}

DECLARE_CLASS(Editor::IActorComponentViewer);
