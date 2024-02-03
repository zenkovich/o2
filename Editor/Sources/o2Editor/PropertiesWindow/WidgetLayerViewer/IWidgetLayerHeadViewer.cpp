#include "o2Editor/stdafx.h"
#include "IWidgetLayerHeadViewer.h"

namespace Editor
{
    class IWidgetLayerHeaderViewer : public RefCounted<IWidgetLayerHeaderViewer>
    {
    public:
        void SetEnabled(bool enabled)
        {
            if (mEnabled == enabled)
                return;

            mEnabled = enabled;

            if (mEnabled)
                OnEnabled();
            else
                OnDisabled();
        }

        bool IsEnabled() const
        {
            return mEnabled;
        }

    private:
        Ref<bool> mEnabled;
    };

    // --- META ---

    DECLARE_CLASS(Editor::IWidgetLayerHeaderViewer, Editor__IWidgetLayerHeaderViewer);
    // --- END META ---
}