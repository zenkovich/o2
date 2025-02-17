#include "o2Editor/stdafx.h"
#include "DefaultPropertiesViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/Properties/ObjectViewer.h"

namespace Editor
{
    DefaultPropertiesViewer::DefaultPropertiesViewer()
    {
        auto scrollArea = o2UI.CreateScrollArea("backless");
        *scrollArea->layout = WidgetLayout::BothStretch(0, 0, 15, 0);
        scrollArea->SetViewLayout(Layout::BothStretch());
        scrollArea->SetClippingLayout(Layout::BothStretch());
        scrollArea->name = "scroll area";
        mContentWidget = scrollArea;

        mViewer = mmake<ObjectViewer>();
        *mViewer->layout = WidgetLayout::BothStretch(5, 0, 5, 5);
        mContentWidget->AddChild(mViewer);
    }

    DefaultPropertiesViewer::~DefaultPropertiesViewer()
    {}

    void DefaultPropertiesViewer::Refresh()
    {
        if (mTargets.IsEmpty())
            return;

        mViewer->Refresh(mTargets);
    }

    void DefaultPropertiesViewer::SetTargets(const Vector<IObject*>& targets)
    {
        mTargets = targets;
        Refresh();
    }

    void DefaultPropertiesViewer::OnPropertiesEnabled()
    {
        if (mViewer)
            mViewer->OnPropertiesEnabled();
    }

    void DefaultPropertiesViewer::OnPropertiesDisabled()
    {
        if (mViewer)
            mViewer->OnPropertiesDisabled();
    }

}
// --- META ---

DECLARE_CLASS(Editor::DefaultPropertiesViewer, Editor__DefaultPropertiesViewer);
// --- END META ---
