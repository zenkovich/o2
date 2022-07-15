#pragma once

#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"

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
		void SetTargetLayers(const Vector<WidgetLayer*>& layers) override;

		// Updates properties values
		void Refresh() override;

		IOBJECT(DefaultWidgetLayerLayoutViewer);

	protected:
		Vector<WidgetLayer*> mLayers;

		Vec2FProperty* mPositionProperty = nullptr;
		Vec2FProperty* mSizeProperty = nullptr;
		Vec2FProperty* mAnchorRightTopProperty = nullptr;
		Vec2FProperty* mAnchorLeftBottomProperty = nullptr;
		Vec2FProperty* moffsetRightTopProperty = nullptr;
		Vec2FProperty* mOffsetLeftBottomProperty = nullptr;

	protected:
		// It is called when some property change completed, stores action for undo
		void OnPropertyChangeCompleted(const String& path, const Vector<DataDocument>& prevValue, 
									   const Vector<DataDocument>& newValue);

		// it is called when some property changed, updates owner widgets
		void OnPropertyChanged(IPropertyField* field);
	};
}

CLASS_BASES_META(Editor::DefaultWidgetLayerLayoutViewer)
{
	BASE_CLASS(Editor::IWidgetLayerLayoutViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerLayoutViewer)
{
	FIELD().PROTECTED().NAME(mLayers);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPositionProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSizeProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnchorRightTopProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnchorLeftBottomProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(moffsetRightTopProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOffsetLeftBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerLayoutViewer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, IPropertyField*);
}
END_META;
