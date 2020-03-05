#include "o2Editor/stdafx.h"
#include "DefaultPropertiesViewer.h"

#include "o2Editor/Core/Properties/ObjectViewers/DefaultObjectViewer.h"
#include "../Core/Properties/Properties.h"

namespace Editor
{
	DefaultPropertiesViewer::DefaultPropertiesViewer()
	{
		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch());
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->name = "scroll area";
		mContentWidget = scrollArea;
	}

	DefaultPropertiesViewer::~DefaultPropertiesViewer()
	{
		delete mViewer;
	}

	void DefaultPropertiesViewer::Refresh()
	{
		const Type* objectType = !mTargets.IsEmpty() ? &mTargets[0]->GetType() : nullptr;

		bool requiredNewViewer = mViewer ? !objectType->IsBasedOn(*mViewer->GetViewingObjectType()) : objectType != nullptr;
		if (requiredNewViewer)
		{
			if (mViewer)
				o2EditorProperties.FreeObjectViewer(mViewer);

			mViewer = o2EditorProperties.CreateObjectViewer(objectType, "");


			mContentWidget->AddChild(mViewer->GetLayout());
			*mViewer->GetLayout()->layout = WidgetLayout::BothStretch(5, 0, 5, 0);
		}

		if (mViewer)
		{
			mViewer->Refresh(mTargets.Select<Pair<IObject*, IObject*>>([](IObject* x) {
				return Pair<IObject*, IObject*>(x, nullptr); }));
		}
	}

	void DefaultPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		mTargets = targets;
		Refresh();
	}
}

DECLARE_CLASS(Editor::DefaultPropertiesViewer);
