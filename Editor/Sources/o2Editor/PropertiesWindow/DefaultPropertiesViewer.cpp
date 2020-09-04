#include "o2Editor/stdafx.h"
#include "DefaultPropertiesViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/Properties/ObjectViewer.h"

namespace Editor
{
	DefaultPropertiesViewer::DefaultPropertiesViewer()
	{
		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch());
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->name = "scroll area";
		mContentWidget = scrollArea;

		mViewer = mnew ObjectViewer();
		*mViewer->layout = WidgetLayout::BothStretch(5, 0, 5, 5);
		mContentWidget->AddChild(mViewer);
	}

	DefaultPropertiesViewer::~DefaultPropertiesViewer()
	{
		delete mViewer;
	}

	void DefaultPropertiesViewer::Refresh()
	{
		if (mTargets.IsEmpty())
			return;

		mViewer->Refresh(mTargets);
	}

	void DefaultPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		mTargets = targets;
		Refresh();
	}
}

DECLARE_CLASS(Editor::DefaultPropertiesViewer);
