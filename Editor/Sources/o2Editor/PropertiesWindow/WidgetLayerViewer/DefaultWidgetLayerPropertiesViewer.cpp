#include "o2/Utils/SmartPointers/Ref.h"
#include "o2/Utils/SmartPointers/WeakRef.h"
#include "o2/Utils/SmartPointers/mmake.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2Editor/Core/Actions/Transform.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{

    class DefaultWidgetLayerPropertiesViewer
    {
    public:
        DefaultWidgetLayerPropertiesViewer();
        ~DefaultWidgetLayerPropertiesViewer();
        void SetTargetLayers(const Vector<Ref<WidgetLayer>>& layers);
        const Type* GetDrawableType() const;
        void Refresh();
        bool IsEmpty() const;
        void FitLayerByDrawable();
    private:
        Ref<IObjectPropertiesViewer> mViewer;
        Ref<SpoilerWithHead> mSpoiler;
        Ref<Button> mFitSizeButton;
        Vector<Ref<WidgetLayer>> mLayers;
        const Type* mDrawableType;
    };

    DefaultWidgetLayerPropertiesViewer::DefaultWidgetLayerPropertiesViewer()
    {
        PushEditorScopeOnStack scope;
        mFitSizeButton = o2UI.CreateButton("Fit size by drawable", THIS_FUNC(FitLayerByDrawable));
    }

    DefaultWidgetLayerPropertiesViewer::~DefaultWidgetLayerPropertiesViewer()
    {}

    void DefaultWidgetLayerPropertiesViewer::SetTargetLayers(const Vector<Ref<WidgetLayer>>& layers)
    {
        mLayers = layers;
        Refresh();
    }

    const Type* DefaultWidgetLayerPropertiesViewer::GetDrawableType() const
    {
        return mDrawableType;
    }

    void DefaultWidgetLayerPropertiesViewer::Refresh()
    {
        if (!mViewer)
        {
            mViewer = o2EditorProperties.CreateObjectViewer(Ref<Type>::MakeShared<WidgetLayer>(), "");
            mViewer->SetHeaderEnabled(false);
            mSpoiler->AddChild(mViewer->GetSpoiler());
            mFitSizeButton->SetParent(mSpoiler);
        }

        if (mViewer)
        {
            mViewer->Refresh(mLayers.Convert<Pair<Ref<IObject>, Ref<IObject>>>([](const Ref<WidgetLayer>& x) {
                return Pair<Ref<IObject>, Ref<IObject>>(Ref<IObject>::DynamicCast(x), nullptr);
            }));
        }
    }

    bool DefaultWidgetLayerPropertiesViewer::IsEmpty() const
    {
        return mSpoiler->GetChildren().Count() == 0;
    }

    void DefaultWidgetLayerPropertiesViewer::FitLayerByDrawable()
    {
        Ref<TransformAction> action = Ref<TransformAction>::MakeShared(mLayers.Convert<Ref<SceneEditableObject>>([](const Ref<WidgetLayer>& layer) { return Ref<SceneEditableObject>::DynamicCast(layer); }));

        for (const Ref<WidgetLayer>& layer : mLayers)
        {
            if (Ref<Sprite> sprite = Ref<Sprite>::DynamicCast(layer->GetDrawable()))
                layer->layout.size = sprite->GetImageAsset()->GetSize();

            if (Ref<Text> text = Ref<Text>::DynamicCast(layer->GetDrawable()))
                layer->layout.size = text->GetRealSize();
        }

        action->Completed();
        o2EditorApplication.DoneAction(action);
    }

}
// --- META ---

DECLARE_CLASS(Editor::DefaultWidgetLayerPropertiesViewer, Editor__DefaultWidgetLayerPropertiesViewer);
// --- END META ---