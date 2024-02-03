#pragma once

#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"
#include <memory>

namespace Editor
{
    class IPropertyField;
    class Vec2FProperty;
    class FloatProperty;

    // --------------------------------------------
    // Default editor widget layer transform viewer
    // --------------------------------------------
    class DefaultWidgetLayerLayoutViewer : public IWidgetLayerLayoutViewer
    {
    public:
        // Default constructor. Initializes data widget
        DefaultWidgetLayerLayoutViewer();

        // Virtual destructor
        ~DefaultWidgetLayerLayoutViewer();

        // Sets target actors
        void SetTargetLayers(const Vector<Ref<WidgetLayer>>& layers) override;

        // Updates properties values
        void Refresh() override;

        IOBJECT(DefaultWidgetLayerLayoutViewer);

    protected:
        Vector<Ref<WidgetLayer>> mLayers;

        Ref<Vec2FProperty> mPositionProperty;
        Ref<Vec2FProperty> mSizeProperty;
        Ref<Vec2FProperty> mAnchorRightTopProperty;
        Ref<Vec2FProperty> mAnchorLeftBottomProperty;
        Ref<Vec2FProperty> moffsetRightTopProperty;
        Ref<Vec2FProperty> mOffsetLeftBottomProperty;

    protected:
        // Called when some property change completed, stores action for undo
        void OnPropertyChangeCompleted(const String& path, const Vector<DataDocument>& prevValue,
                                       const Vector<DataDocument>& newValue);

        // Called when some property changed, updates owner widgets
        void OnPropertyChanged(const Ref<IPropertyField>& field);
    };
}

// --- META ---

CLASS_BASES_META(Editor::DefaultWidgetLayerLayoutViewer)
{
    BASE_CLASS(Editor::IWidgetLayerLayoutViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerLayoutViewer)
{
    FIELD().PROTECTED().NAME(mLayers);
    FIELD().PROTECTED().NAME(mPositionProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
    FIELD().PROTECTED().NAME(mSizeProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
    FIELD().PROTECTED().NAME(mAnchorRightTopProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
    FIELD().PROTECTED().NAME(mAnchorLeftBottomProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
    FIELD().PROTECTED().NAME(moffsetRightTopProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
    FIELD().PROTECTED().NAME(mOffsetLeftBottomProperty).DEFAULT_VALUE(mmake<Vec2FProperty>());
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerLayoutViewer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetLayers, const Vector<Ref<WidgetLayer>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const Ref<IPropertyField>&);
}
END_META;
// --- END META ---