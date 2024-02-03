#pragma once

#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"
#include <memory>

using namespace o2;

namespace o2
{
    class Button;
    class WidgetDragHandle;
}

namespace Editor
{
    class AssetProperty;
    class BorderIProperty;
    class ImageSlicesEditorWidget;

    class ImageAssetViewer : public DefaultObjectPropertiesViewer
    {
    public:
        const Type* GetViewingObjectType() const override;
        static const Type* GetViewingObjectTypeStatic();

        IOBJECT(ImageAssetViewer);

    private:
        ImageSlicesEditorWidget* mSlicesEditor = nullptr;
        Vector<IAbstractValueProxy*> mAtlasProxies;
        AssetProperty* mAtlasProperty = nullptr;

    private:
        void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
        void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
    };
}

// --- META ---

CLASS_BASES_META(Editor::ImageAssetViewer)
{
    BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;

CLASS_FIELDS_META(Editor::ImageAssetViewer)
{
    FIELD().PRIVATE().DEFAULT_VALUE(mmake<ImageSlicesEditorWidget>()).NAME(mSlicesEditor);
    FIELD().PRIVATE().NAME(mAtlasProxies);
    FIELD().PRIVATE().NAME(mAtlasProperty);
}
END_META;

CLASS_METHODS_META(Editor::ImageAssetViewer)
{
    typedef const Vector<Pair<IObject*, IObject*>>& Args;

    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetViewingObjectTypeStatic);
    FUNCTION().PRIVATE().SIGNATURE(void, RebuildProperties, const Ref<const Vector<Pair<IObject*, IObject*>>>&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnRefreshed, const Ref<const Vector<Pair<IObject*, IObject*>>>&);
}
END_META;