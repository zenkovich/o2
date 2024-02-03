#include "o2Editor/stdafx.h"
#include "IPropertiesViewer.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Serialization/DataValue.h"

namespace Editor
{
    IPropertiesViewer::~IPropertiesViewer()
    {
        delete mContentWidget;
    }

    const Type* IPropertiesViewer::GetViewingObjectType() const
    {
        return nullptr;
    }

    void IPropertiesViewer::Refresh()
    {}

    void IPropertiesViewer::SetEnabled(const bool& enabled)
    {
        if (mEnabled == enabled)
            return;

        mEnabled = enabled;

        if (mEnabled)
            OnEnabled();
        else
            OnDisabled();
    }

    bool IPropertiesViewer::IsEnabled() const
    {
        return mEnabled;
    }    
}

// --- META ---

DECLARE_CLASS(Editor::IPropertiesViewer, Editor__IPropertiesViewer);
// --- END META ---