#include "stdafx.h"
#include "IActorPropertiesViewer.h"

#include "Core/UI/SpoilerWithHead.h"
#include "UI/Image.h"
#include "UI/UIManager.h"

namespace Editor
{
	IActorPropertiesViewer::IActorPropertiesViewer()
	{
		mSpoiler = o2UI.CreateWidget<UISpoilerWithHead>();

		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI2_big_file_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IActorPropertiesViewer::~IActorPropertiesViewer()
	{
		if (mSpoiler)
			delete mSpoiler;
	}

	UIWidget* IActorPropertiesViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorPropertiesViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorPropertiesViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorPropertiesViewer::Refresh()
	{}

	void IActorPropertiesViewer::Rebuild()
	{}

	bool IActorPropertiesViewer::IsEmpty() const
	{
		return true;
	}

	bool IActorPropertiesViewer::IsBuiltWithHiddenFields() const
	{
		return false;
	}

}

DECLARE_CLASS(Editor::IActorPropertiesViewer);
