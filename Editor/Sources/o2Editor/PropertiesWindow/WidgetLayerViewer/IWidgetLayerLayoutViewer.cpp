#include "o2Editor/stdafx.h"
#include "IWidgetLayerLayoutViewer.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
    IWidgetLayerLayoutViewer::IWidgetLayerLayoutViewer()
        : mSpoiler(mmake<SpoilerWithHead>())
    {
        PushEditorScopeOnStack scope;

        mSpoiler->borderBottom = 5;
        mSpoiler->SetCaption("Transform");
        mSpoiler->GetIcon()->SetImageName("ui/UI4_transform_icon_white.png");

        mSpoiler->SetExpanded(true);
    }

    IWidgetLayerLayoutViewer::~IWidgetLayerLayoutViewer()
    {}

    Widget* IWidgetLayerLayoutViewer::GetWidget() const
    {
        return mSpoiler.get();
    }

    void IWidgetLayerLayoutViewer::Expand()
    {
        mSpoiler->Expand();
    }

    void IWidgetLayerLayoutViewer::Collapse()
    {
        mSpoiler->Collapse();
    }

    void IWidgetLayerLayoutViewer::Refresh()
    {}

    void IWidgetLayerLayoutViewer::SetEnabled(const Ref<bool>& enabled)
    {
        if (!mEnabled && enabled->value)
        {
            mEnabled = enabled;
            OnEnabled();
        }
        else if (mEnabled && !enabled->value)
        {
            mEnabled = enabled;
            OnDisabled();
        }
    }

    bool IWidgetLayerLayoutViewer::IsEnabled() const
    {
        return mEnabled->value;
    }

    void IWidgetLayerLayoutViewer::SetEnabled(bool enabled)
    {
        SetEnabled(make_ref<bool>(enabled));
    }

}

// --- META ---

DECLARE_CLASS(Editor::IWidgetLayerLayoutViewer, Editor__IWidgetLayerLayoutViewer);
// --- END META ---