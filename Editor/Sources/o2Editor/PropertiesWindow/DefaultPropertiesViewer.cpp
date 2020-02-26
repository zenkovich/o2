#include "o2Editor/stdafx.h"
#include "DefaultPropertiesViewer.h"

#include "o2Editor/Core/Properties/ObjectViewers/DefaultObjectViewer.h"

namespace Editor
{
	DefaultPropertiesViewer::DefaultPropertiesViewer()
	{
		mObjectViewer = mnew DefaultObjectViewer();

		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch());
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->name = "scroll area";
		mContentWidget = scrollArea;

		mContentWidget->AddChild(mObjectViewer->GetLayout());
		*mObjectViewer->GetLayout()->layout = WidgetLayout::BothStretch(5, 0, 5, 0);
	}

	DefaultPropertiesViewer::~DefaultPropertiesViewer()
	{
		delete mObjectViewer;
	}

	void DefaultPropertiesViewer::Refresh()
	{
		mObjectViewer->Refresh(mTargets.Select<Pair<IObject*, IObject*>>([](IObject* x) { 
			return Pair<IObject*, IObject*>(x, nullptr); }));
	}

	void DefaultPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		mTargets = targets;
		Refresh();
	}
}

DECLARE_CLASS(Editor::DefaultPropertiesViewer);
